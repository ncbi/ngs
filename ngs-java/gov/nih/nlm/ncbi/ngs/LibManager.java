/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ==============================================================================
*
*/


package gov.nih.nlm.ncbi.ngs;


import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;


/** This class is responsible for JNI dynamic library load
    and download from NCBI - when it cannot be found locally */
class LibManager implements FileCreator
{

    /** Force it in force-majeure situations.
        It also could be set without recompiling
        by setting vdb.System.loadLibrary java system property */
    private static boolean JUST_DO_SIMPLE_LOAD_LIBRARY = false;

    /**
     * Will search for latest library version among all installed
     */
    private static boolean SEARCH_FOR_LATEST_INSTALLED_LIBRARY = true;

    /**
     * Will check what is the latest version available online
     */
    private static boolean CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION = true;


    /** Possible location to search for library to load.
        The order of enum elements defines library location search order. */
    enum Location
    {
        CACHE,        // from ~/.ncbi/LibManager.properties
        NCBI_HOME,     // ~/.ncbi/lib64|32
        LIBPATH,       // iterate "java.library.path" - extended LD_LIBRARY_PATH
        NCBI_NGS_JAR_DIR, // directory where ncbi-ngs.jar is
        CLASSPATH,     // iterate "java.class.path" - where java classes are
        CWD,           // "."
        TMP,            // Temporary folder
        DOWNLOAD
    }


    enum Bits {
        b32,
        b64,
        bUNKNOWN;

        String intString()
        {   switch (this) { case b32: return "32"; default: return "64";} }
    }

////////////////////// TODO check out of space condition ///////////////////////

    LibManager ( String [] libs, String [] versions )
    {
        if (versions == null || libs == null) {
            throw new RuntimeException("Neither libs nor versions can't be null");
        }

        if (versions.length != libs.length) {
            throw new RuntimeException("Invalid library versions: should match number of libraries");
        }

        checkSystemProperties();
        locations = generateLocations();

        properties = new LMProperties(detectJVM().intString());
        if (CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION) {
            downloadManager = new DownloadManager(properties);
        }

        libraryVersions = new HashMap<String, String>();
        foundLibraryByVersions = new TreeMap<String, TreeMap<Version, String>>();
        for (int i = 0; i < libs.length; ++i) {
            libraryVersions.put(libs[i], versions[i]);
            foundLibraryByVersions.put(libs[i], new TreeMap<Version, String>());
        }


        if (!JUST_DO_SIMPLE_LOAD_LIBRARY && System.getProperty("vdb.deleteLibraries") != null) {
            /* make sure we have the latest version of ngs-sdk & ncbi-vdb dll-s */
            for (String libname : libs) {
                Logger.warning( "Deleting all JNI libraries...");
                LibPathIterator.deleteLibraries(this, libname);
            }
        }
    }

    private void checkSystemProperties() {
        if (System.getProperty("vdb.System.loadLibrary") != null) {
            Logger.warning ( "Smart DLL search and library download was disabled" );
            JUST_DO_SIMPLE_LOAD_LIBRARY = true;
            CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION = false;
            SEARCH_FOR_LATEST_INSTALLED_LIBRARY = false;
            return;
        }

        if (System.getProperty("vdb.System.noLibraryDownload") != null) {
            Logger.warning ( "DLL download was disabled" );
            CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION = false;
        }

        if (System.getProperty("vdb.System.noLatestLibrarySearch") != null) {
            Logger.warning ( "Search of latest installed DLL was disabled" );
            SEARCH_FOR_LATEST_INSTALLED_LIBRARY = false;
        }
    }

    private Location[] generateLocations() {
        Location[] allLocations = Location.values();
        Location[] result;
        Set<Location> disabledLocations = new TreeSet<Location>();
        if (JUST_DO_SIMPLE_LOAD_LIBRARY) {
            disabledLocations.addAll(Arrays.asList(allLocations));
            disabledLocations.remove(Location.LIBPATH);
        }

        if (!CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION) {
            disabledLocations.add(Location.DOWNLOAD);
        }

        Logger.info("Disabled locations: " + Arrays.toString(disabledLocations.toArray()));


        result = new Location[allLocations.length - disabledLocations.size()];
        int i = 0;
        for (Location location : allLocations) {
            if (disabledLocations.contains(location)) {
                continue;
            }

            result[i++] = location;
        }

        assert i == result.length;

        return result;
    }

    Location[] locations()
    {
        return locations;
    }


//////////////////////////// FileCreator interface /////////////////////////////


    /** Creates a file by finding directory by iterating the location array
        and using libname to generate the file name */
    public BufferedOutputStream create ( String libname ) {
        createdFileName = null;
        for (int i = 0; i < 2; ++i) {
            Location location = null;
            boolean model = true;
            switch (i) {
                case 0:
                    location = Location.NCBI_HOME;
                    model = false;
                    break;
                case 1:
                    break;
            }
            LibPathIterator it = new LibPathIterator
                (this, location, mapLibraryName(libname, model), true);

            while (true) {
                String pathname = it.nextName();
                if (pathname == null) {
                    return null;
                }

                Logger.fine("Trying to create " + pathname + "...");
                File file = new File(pathname);
                try {
                    pathname = file.getAbsolutePath();
                } catch (SecurityException e) {
                    Logger.warning(pathname + " : cannot getAbsolutePath " + e);
                    continue;
                }
                if (file.exists()) {
                    String dathname = pathname + ".bak";
                    File dest = new File(dathname);
                    {
                        String name = System.getProperty("os.name");
                        if (name != null && name.startsWith("Win")) {
                     /*    On Windows delete the file we are going to rename to.
                           Otherwise renaming will fail. */
                            if (dest.exists()) {
                                Logger.fine
                                    ("Trying to remove " + dathname + " ...");
                                dest.delete();
                            }
                        }
                    }
                    Logger.finest("Trying to rename " + pathname
                        + " to " + dathname + " ...");
                    if (!file.renameTo(dest)) {
                        Logger.warning
                            (pathname + ".renameTo(" + dathname + ") failed");
                    }
                }
                FileOutputStream s = null;
                try {
                    s = new FileOutputStream(pathname);
                } catch (FileNotFoundException e) {
/* e.message = pathname (Permission denied):
could be because pathname is not writable
or pathname not found and its directory is not writable */
                    Logger.warning("Cannot open " + pathname);
                    continue;
                }

                createdFileName = pathname;

                Logger.fine("Opened " + pathname);
                return new BufferedOutputStream(s, HttpManager.BUF_SZ);
            }
        }
        return null;
    }


    public void done(boolean success) {
        if (!success) {
            createdFileName = null;
        }
    }


////////////////////////////////////////////////////////////////////////////////


    /** Loads the system library by finding it by iterating the location array.
        Try to download it from NCBI if not found. */
    boolean loadLibrary( String libname ) {
        boolean ok = false;

        Logger.fine("Loading " + libname + " library...");
        if (searchAndLoad(libname) != null) {
            Logger.fine("Loaded " + libname + " library");
            ok = true;
        } else {
            Logger.warning("Failed to load " + libname + " library");
        }

        properties.store();

        return ok;
    }

//////////////////////////// static package methods ////////////////////////////


    static String[] mapLibraryName(String libname)
    {
        return mapLibraryName(libname, true);
    }


    static String[] mapLibraryName(String libname, boolean withDataModel)
    {
        String m = libname;
        if (withDataModel) {
            m = libnameWithDataModel(libname);
        }
        String name = System.getProperty("os.name");
        int dup = 1;
        if (name != null  && name.equals("Mac OS X")) {
            dup = 2;
        }
        int n = (m == null ? 1 : 2) * dup;
        String[] ns = new String[n];
        int i = 0;
        if (m != null) {
            ns[i++] = System.mapLibraryName(m);
        }
        ns[i++] = System.mapLibraryName(libname);
        if (dup == 2) {
            if (m != null) {
                ns[i++] = m + ".dylib";;
            }
            ns[i++] = libname + ".dylib";;
        }
        return ns;
    }


    static Bits detectJVM()
    {
        final String keys [] = {
            "sun.arch.data.model",
            "com.ibm.vm.bitmode",
            "os.arch",
        };
        for (String key : keys ) {
            String property = System.getProperty(key);
            Logger.finest(key + "=" + property);
            if (property != null) {
                int errCode = (property.indexOf("64") >= 0) ? 64 : 32;
                Logger.finest(errCode + "-bit JVM");
                return errCode == 64 ? Bits.b64 : Bits.b32;
            }
        }
        Logger.fine("Unknown-bit JVM");
        return Bits.bUNKNOWN;
    }

//////////////////////////// private static methods ////////////////////////////


    /** Add 32- or 64-bit data model suffix */
    private static String libnameWithDataModel(String libname)
    {
        String m = null;
        switch (detectJVM()) {
            case b64:
                m = "-64";
                break;
            case b32:
                m = "-32";
                break;
        }
        if (m != null) {
            m = libname + m;
        }
        return m;
    }


    private static Location[] getLocationProperty()
    {
        String p = System.getProperty("vdb.loadLibraryLocations");
        if (p == null) {
            return null;
        }

        int n = 0;
        for (int i = 0; i < p.length(); ++i) {
            if ("PJCLNTWD".indexOf(p.charAt(i)) >= 0) {
                ++n;
            }
        }

        if (n == 0) {
            return null;
        }
                
        Location locations[] = new Location[n];
        n = 0;
        for (int i = 0; i < p.length(); ++i) {
            switch (p.charAt(i)) {
                case 'P':
                    locations[n] = Location.CLASSPATH;
                    break;
                case 'J':
                    locations[n] = Location.NCBI_NGS_JAR_DIR;
                    break;
                case 'C':
                    locations[n] = Location.CACHE;
                    break;
                case 'L':
                    locations[n] = Location.LIBPATH;
                    break;
                case 'N':
                    locations[n] = Location.NCBI_HOME;
                    break;
                case 'T':
                    locations[n] = Location.TMP;
                    break;
                case 'W':
                    locations[n] = Location.CWD;
                    break;
                case 'D':
                    locations[n] = Location.DOWNLOAD;
                    break;
                default:
                    continue;
            }
            ++n;
        }

        return locations;
    }

    private boolean systemLoadByFile(String filepath, String libname, boolean continueUnlessLatest) {
        if (!fileExists(filepath)) {
            Logger.finer("File " + filepath + " not found");
            return false;
        }

        Logger.finer("System.load(" + filepath + ")...");

        return systemLoadLibrary(libname, filepath, false, continueUnlessLatest);
    }


    private boolean systemLoadByJava(String libnameWithDataModl, Location l,
                                     String libname, boolean continueUnlessLatest) {
        // System.loadLibrary is using java.library.path to find the library
        Logger.finest("java.library.path = " + System.getProperty("java.library.path"));
        Logger.finer(l + ": System.loadLibrary(" + libnameWithDataModl + ")...");
        Logger.finest("System.mapLibraryName(" + libnameWithDataModl + ") = " +
                System.mapLibraryName(libnameWithDataModl));

        return systemLoadLibrary(libname, libnameWithDataModl, true, continueUnlessLatest);
    }

    private boolean systemLoadLibrary(String libname, String libpath,
                                      boolean useLoadLibrary, boolean continueUnlessLatest) {
        // check for version
        String requiredVersion = getMinimalVersion(libname);

        try {
            String version = LibVersionChecker.getVersion(libname, libpath, useLoadLibrary);
            if (version == null) {
                Logger.fine("Cannot load or get library version: " + libpath);
                return false;
            }

            Version v = new Version(version);
            if (v.compareTo(new Version(requiredVersion)) < 0) {
                Logger.info("Found library: " + libpath + " version (" +
                        version + ") is less than minimal required (" + requiredVersion + ")");
                return false;
            }

            if (continueUnlessLatest) {
                String latestVersion = getLatestVersion(libname);
                if (latestVersion == null || v.compareTo(new Version(latestVersion)) <= 0) {
                    TreeMap<Version, String> libraryByVersionCurrent = foundLibraryByVersions.get(libname);
                    if (libraryByVersionCurrent == null) {
                        throw new RuntimeException("Cannot find library entry in array for: " + libname);
                    }
                    if (!libraryByVersionCurrent.containsKey(v)) {
                        libraryByVersionCurrent.put(v, libpath);
                    }
                    return false;
                }
            }

            if (useLoadLibrary) {
                System.loadLibrary(libpath);
            } else {
                System.load(libpath);
            }
            Logger.fine("Loaded library " + libpath);
            properties.loaded(libname, version, libpath);
            return true;
        } catch (UnsatisfiedLinkError e) {
            Logger.fine("cannot load library: " + e);
        } catch (Throwable e) {
            Logger.warning("Cannot load library: " + e);
        }
        return false;
    }

    private static boolean fileExists(String filename) {
        File file = new File(filename);
        if (file.exists()) {
            return true;
        } else {
            Logger.finest(filename + " does not exist");
            return false;
        }
    }

    private static void printLoadingMsg(Location l, String libname) {
        if (l == Location.LIBPATH) {
            Logger.info("LoadingLibrary " + libname + "...");
        } else {
            Logger.info("Loading " + libname + " from " + l + "...");
        }
    }


////////////////////////////////////////////////////////////////////////////////

    private String getMinimalVersion(String libname) {
        String minimalVersion = libraryVersions.get(libname);
        if (libname == null) {
            throw new RuntimeException("Library '" + libname + "' version was not specified");
        }
        return minimalVersion;
    }

    private String getLatestVersion(String libname) {
        if (!CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION) {
            return null;
        }
        return downloadManager.getLatestVersion(libname);
    }

    /** Tries to load the library by searching it using allowed locations array.
        If JUST_DO_SIMPLE_LOAD_LIBRARY = true
        then just call plain System.LoadLibrary(libname) */
    private String searchAndLoad(String libname) {
        if (SEARCH_FOR_LATEST_INSTALLED_LIBRARY) {
            Logger.fine("Starting a search for the latest installed library: " + libname);
        }
        for (Location l : locations) {

            printLoadingMsg(l, libname);

            switch (l) {
            case LIBPATH: {
                if (systemLoadByJava(libname, l, libname, SEARCH_FOR_LATEST_INSTALLED_LIBRARY)) {
                    return libname;
                }
                if (JUST_DO_SIMPLE_LOAD_LIBRARY) {
                    break;
                }
                String libnameWithDataModel = libnameWithDataModel(libname);
                if (libnameWithDataModel != null) {
                    if (systemLoadByJava(libnameWithDataModel, l, libname, SEARCH_FOR_LATEST_INSTALLED_LIBRARY)) {
                        return libnameWithDataModel;
                    }
                }
                break;
            }
            case CACHE: {
                // TODO: check if we can trust cache (depending on SEARCH_FOR_LATEST_INSTALLED_LIBRARY)
                boolean searchEvenAfterFound = true;
                String filename = properties.get(libname, getMinimalVersion(libname));
                if (filename == null) {
                    continue;
                }
                if (filename.startsWith(libname)) {
                    if (systemLoadByJava(filename, l, libname, searchEvenAfterFound)) {
                        return filename;
                    }
                } else {
                    if (systemLoadByFile(filename, libname, searchEvenAfterFound)) {
                        return filename;
                    }
                }

                properties.notLoaded(libname);
                break;
            }
            case DOWNLOAD: {
                Logger.info("Downloading " + libname + " from NCBI...");
                String filename = download(libname);
                if (filename == null) {
                    Logger.warning("Failed to download " + libname + " from NCBI");
                    continue;
                }
                Logger.info("Downloaded " + libname + " from NCBI");
                Logger.fine("Loading " + libname + " library...");
                if (systemLoadByFile(filename, libname, SEARCH_FOR_LATEST_INSTALLED_LIBRARY)) {
                    return filename;
                }
            }
            default: {
                String name[] = null;
                name = mapLibraryName(libname);
                Logger.finest("System.mapLibraryName(" + libname + ") = " + name[0]);

                LibPathIterator it = new LibPathIterator(l, name);
                while (true) {
                    String filename = it.nextName();
                    if (filename == null) {
                        break;
                    }

                    if (systemLoadByFile(filename, libname, SEARCH_FOR_LATEST_INSTALLED_LIBRARY)) {
                        return filename;
                    }
                }
                break;
              }
            }
        }

        if (SEARCH_FOR_LATEST_INSTALLED_LIBRARY && foundLibraryByVersions.size() > 0) {
            TreeMap<Version, String> libraryByVersionCurrent = foundLibraryByVersions.get(libname);
            if (libraryByVersionCurrent == null) {
                throw new RuntimeException("Cannot find library entry in array for: " + libname);
            }
            String libpath = libraryByVersionCurrent.lastEntry().getValue();
            String version = libraryByVersionCurrent.lastEntry().getKey().toString();
            Logger.fine("Loading latest installed library: " + libpath + " " + version);
            if (libpath.startsWith(libname)) {
                if (systemLoadByJava(libpath, Location.LIBPATH, libname, false)) {
                    return libpath;
                }
            } else {
                if (systemLoadByFile(libpath, libname, false)) {
                    return libpath;
                }
            }
        }

        return null;
    }


////////////////////////////////////////////////////////////////////////////////


    /** Downloads the library and default configuration from NCBI.
        Save them where it can be found by LibManager.loadLibrary() */
    private String download(String libname) {
        if (!CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION) {
            throw new RuntimeException("CHECK_AND_DOWNLOAD_LATEST_LIBRARY_VERSION is disabled. This method should not be called");
        }

        String latestVersion = getLatestVersion(libname);
        if (!downloadManager.downloadLib(this, libname, latestVersion)) {
            return null;
        }

        String r = createdFileName;
        createdFileName = null;
        properties.saved(libname, latestVersion, r);

        return r;


        // TODO: what to do with downloadKfg?
//        return downloadKfg(knownLibPath[i + 1]);
    }

    /** Fetches the configuration from NCBI */
    private boolean downloadKfg(String libpath) {
        Logger.finest("configuration download is disabled");
/*
        // this is broken. if enabled, move download part to a DownloadManager
        File l = new File(libpath);
        String d = l.getParent();
        if (d == null) {
            Logger.finest("cannot get parent path of " + libpath);
            return true;
        }
        String n = d + File.separatorChar + "ncbi";
        File fn = new File(n);
        if (fn.exists()) {
            if (fn.isDirectory()) {
                Logger.finest("configuration directory '" + n + "' exists");
            } else {
                Logger.finest("'" + n + "' is not a directory");
                return true;
            }
        } else {
            Logger.finest("configuration directory '" + n + "' does not exist");
            try {
                if (!fn.mkdir()) {
                    Logger.finest("cannot mkdir '" + n + "'");
                    return true;
                }
            } catch (SecurityException e) {
                Logger.finest(e);
                return true;
            }
        }
        try {
            fn.setExecutable(true, true);
        } catch (SecurityException e) {
            Logger.finest(e);
        }
        try {
            fn.setReadable(true, true);
        } catch (SecurityException e) {
            Logger.finest(e);
        }
        try {
            fn.setWritable(true, true);
        } catch (SecurityException e) {
            Logger.finest(e);
        }
        String k = n + File.separatorChar + "default.kfg";
        File fk = new File(k);
        if (fk.exists()) {
            Logger.finest("'" + fk + "' exists");
            return true;
        }
        String request = "cmd=lib&libname=kfg";
        for (SratoolkitCgis cgis = new SratoolkitCgis(); ; ) {
            String spec = cgis.nextSpec();
            if (spec == null) {
                break;
            }
            try {
                String f = HttpManager.post(spec, request);
                try {
                    FileOutputStream out = new FileOutputStream(fk);
                    try {
                        out.write(f.getBytes());
                        out.close();
                    } catch (IOException e) {
                        Logger.finest(e);
                        continue;
                    }
                    Logger.finest("created '" + fk + "'");
                    return true;
                } catch (FileNotFoundException e) {
                    Logger.finest(e);
                }
            } catch (HttpException e) {
                Logger.finest(e);
            }
        }
        Logger.finest("cannot create '" + fk + "'");
*/
        return true;
    }

////////////////////////////////////////////////////////////////////////////////

    /** Possible location to search for library to load.
        The order of elements defines library location search order. */
    private Location[] locations;

    private HashMap<String, String> libraryVersions;

    private TreeMap<String, TreeMap<Version, String>> foundLibraryByVersions;

    /** Knows how to check and download latest libraries versions */
    private DownloadManager downloadManager;

    /** Is updated by FileCreator methods called by HttpManager */
    private String createdFileName;

    private LMProperties properties; // to keep dll path/version-s

////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
-Djava.library.path
-Dvdb.log
off if JUST_DO_SIMPLE_LOAD_LIBRARY

TODO save location where library was found
(try to use load instead of loadLibrary even for LIBPATH);
add which() method to return this location(?);
try to load the library if LibManager.loadLibrary() was never called.
What if loadLibrary() is called several times? load()
      http://docs.oracle.com/javase/6/docs/api/java/util/logging/Level.html
      http://en.wikipedia.org/wiki/Log4j
      http://logging.apache.org/log4j/1.2/apidocs/org/apache/log4j/Level.html
-Dlog=0   1
-Dlog=OFF SEVERE/FATAL
-Dlog=O   S/F
*******************************************************************************/
}
