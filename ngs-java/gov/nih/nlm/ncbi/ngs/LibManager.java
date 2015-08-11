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
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


/** This class is responsible for JNI dynamic library load
    and download from NCBI - when it cannot be found locally */
class LibManager implements FileCreator
{

    /** Force it in force-majeure situations.
        It also could be set without recompiling
        by setting vdb.System.loadLibrary java system property */
    private static boolean JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY = false;


    /** Possible location to search for library to load.
        The order of enum elements defines library location search order. */
    enum Location
    {
/*KNOWN_PATH should be the first entry here
   if you want it to be loaded right after download.
   Otherwise the manager will try to the search previous location entries first.
   May be you want it to test something (e.g. a bad library file).
  And LATEST_PATH should be before KNOWN_PATH: it makes sure
   you will first download the latest version from NCBI if it was released */
        LATEST_PATH,   /* path to the latest version of the library:
                                  found in the system or downloaded from NCBI */
        KNOWN_PATH,    // from config or file downloaded from NCBI
        NCBI_HOME,     // ~/.ncbi/lib64|32
        LIBPATH,       // iterate "java.library.path" - extended LD_LIBRARY_PATH
        NCBI_NGS_JAR_DIR, // directory where ncbi-ngs.jar is
        CLASSPATH,     // iterate "java.class.path" - where java classes are
        CWD,           // "."
        TMP            // Temporary folder
    }


    enum Bits {
        b32,
        b64,
        bUNKNOWN,
    }    


    private class SratoolkitCgis {
        private SratoolkitCgis() {
            String path = LibPathIterator.ncbiHome();
            if (path != null) {
                path +=
                    LibPathIterator.fileSeparator() + "LibManager.properties";
                try {
                    FileInputStream inStream = new FileInputStream(path);

                    Properties properties = new Properties();
                    properties.load(inStream);

                    spec = properties.getProperty("/servers/sratookit-cgi");

                    inStream.close();

                    Logger.warning("Use " + spec + " from " + path);
                } catch (java.io.IOException e) {}
            }

            if (spec == null) {
              spec =
               "http://trace.ncbi.nlm.nih.gov/Traces/sratoolkit/sratoolkit.cgi";
            }
            done = false;
        }

        private String nextSpec() {
            if (!done) {
                done = true;
                return spec;
            } else {
                return null;
            }
        }

        private String spec;
        private boolean done;
    }


//TODO check out of space condition


////////////////////////////////////////////////////////////////////////////////


    LibManager ()
    {
        this ( null, null );
    }


    LibManager ( String [] libs )
    {
        this ( null, libs );
    }

    private LibManager ( Location [] locations, String [] libs )
    {
        latestLibPaths = new HashMap<String, String>();

//      if (locations == null) locations = getLocationProperty ();

        if (locations != null)
            this.location = locations;
        else
            this.location = Location.values ();

        if (System.getProperty("vdb.System.loadLibrary") != null)
        {
            System.err.println ( "Smart DLL search was turned off" );
            JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY = true;
        }

        if (System.getProperty("vdb.deleteLibraries") != null)
        {
            //System.err.println ( "Deleting all JNI libraries..." );
            //LibPathIterator.deleteLibraries(this, libname);
        }

        if (JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY || libs == null) {
            return;
        }

        /* make sure we have the latest version of ngs-sdk & ncbi-vdb dll-s */
        for (String lib : libs) {
            launchLibCheck(lib);
        }
    }


//////////////////////////// FileCreator interface /////////////////////////////


    /** Creates a file by finding directory by iterating the location array
        and using libname to generate the file name */
    public BufferedOutputStream create ( String libname )
    {
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
                    System.err.println
                        (pathname + " : cannot getAbsolutePath " + e);
                    continue;
                }
                if (file.exists()) {
                    String dathname = pathname + ".bak";
                    File dest = new File(dathname);
                    {
                        String name = System.getProperty("os.name");
//                      Logger.finest("os.name = " + name);
                        if (name != null && name.substring(0, 3).equals("Win"))
                        {
                            if (dest.exists()) {
                                Logger.fine
                                    ("Trying to remove " + dathname + " ...");
                                dest.delete();
                            }
                        } else {
//                          Logger.finest("substrng(0,2)="+name.substring(0,2));
                        }
                    }
                    Logger.fine("Trying to rename " + pathname
                        + " to " + dathname + " ...");
                    if (!file.renameTo(dest)) {
                        Logger.fine
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
                    System.err.println("Cannot open " + pathname);
                    continue;
                }

                updateKnownLibPath(pathname);
                createdFileName = pathname;

                Logger.fine("Opened " + pathname);
                return new BufferedOutputStream(s, HttpManager.BUF_SZ);
            }
        }
        return null;
    }


    public void done(boolean success)
    {   if (!success) { createdFileName = null; } }


////////////////////////////////////////////////////////////////////////////////


    /** Loads the system library by finding it by iterating the location array.
        Try to download it from NCBI if not found. */
    boolean loadLibrary( String libname )
    {
        Logger.fine("Loading " + libname + " library...");
        if (load(libname) != null) {
            Logger.fine("Loaded " + libname + " library");
            return true;
        }

        System.err.println("Failed to load " + libname + " library");
        if (JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY) {
            return false;
        }

/* Here we try do download the library from NCBI always
   when we were not able to System.load Library it. */

        System.err.println("Downloading " + libname + " from NCBI...");
        if (download( libname )) {
            System.err.println("Downloaded " + libname + " from NCBI");
            Logger.fine("Loading " + libname + " library...");
            String path = load(libname);
            Logger.fine(path == null ? "Loaded " : "Failed to load "
                + libname + " library");
            return path != null;
        }
        else {
            System.err.println("Failed to download " + libname + " from NCBI");
            return false;
        }
    }


    Location[] locations()
    {
        return location;
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


    static Bits DetectJVM()
    {
        final String keys [] = {
            "sun.arch.data.model",
            "com.ibm.vm.bitmode",
            "os.arch",
        };
        for (String key : keys ) {
            String property = System.getProperty(key);
            Logger.fine(key + "=" + property);
            if (property != null) {
                int errCode = (property.indexOf("64") >= 0) ? 64 : 32;
                Logger.fine(errCode + "-bit JVM");
                return errCode == 64 ? Bits.b64 : Bits.b32;
            }
        }
        Logger.fine("Unknown-bit JVM");
        return Bits.bUNKNOWN;
    }


    static String osProperties()
        throws Exception
    {
        String request = "os_name=";
        String name = System.getProperty("os.name");
        if (name == null) {
            throw new Exception("Cannot detect OS");
        }
        request += name;
        request += "&bits=";
        switch (DetectJVM()) {
            case b64:
                request += "64";
                break;
            case b32:
                request += "32";
                break;
            case bUNKNOWN:
                request += "unknown";
        }
        String arch = System.getProperty("os.arch");
        if (arch != null) {
            request += "&os_arch=" + arch;
        }
        String version = System.getProperty("os.version");
        if (version != null) {
            request += "&os_version=" + version;
        }
        return request;
    }


//////////////////////////// static private methods ////////////////////////////


    /** Add 32- or 64-bit data model suffix */
    private static String libnameWithDataModel(String libname)
    {
        String m = null;
        switch (DetectJVM()) {
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
            if ("CJKLNTW".indexOf(p.charAt(i)) >= 0) {
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
                case 'C':
                    locations[n] = Location.CLASSPATH;
                    break;
                case 'J':
                    locations[n] = Location.NCBI_NGS_JAR_DIR;
                    break;
                case 'K':
                    locations[n] = Location.KNOWN_PATH;
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
                default:
                    continue;
            }
            ++n;
        }

        return locations;
    }


    private static boolean prepareToLoad(String filename) {
        File file = new File(filename);
        if (file.exists()) {
            return true;
        } else {
            Logger.fine(filename + " does not exist");
            return false;
        }
    }


    private static void printLoadingMsg(Location l, String libname)
    {
        if (l == Location.LIBPATH) {
            Logger.fine("LoadingLibrary " + libname + "...");
        } else {
            Logger.fine("Loading " + libname + " from " + l + "...");
        }
    }


////////////////////////////////////////////////////////////////////////////////


    /** Tries to load the library by searching it using location array.
        If JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY = true
        then just call plain System.LoadLibrary(libname) */
    private String load(String libname)
    {
        for (Location l : location) {
            if (JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY) {
                l = Location.LIBPATH;
            }

            printLoadingMsg(l, libname);

            if (l == Location.LIBPATH) {
// System.loadLibrary is using java.library.path to find the library
                Logger.finest("java.library.path = "
                    + System.getProperty("java.library.path"));
                Logger.fine(l + ": System.loadLibrary(" + libname + ")...");
                Logger.finest("System.mapLibraryName(" + libname + ") = "
                    + System.mapLibraryName(libname));
                try {
                    System.loadLibrary(libname);
                    Logger.fine("Loaded library " + libname);
                    return libname;
                } catch (UnsatisfiedLinkError e) {
                    Logger.fine("cannot load library: " + e);
                } catch (Throwable e) {
                    System.err.println("Cannot load library: " + e);
                }
                if (JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY) {
                    return null;
                }
                String libnameWithDataModel = libnameWithDataModel(libname);
                if (libnameWithDataModel != null) {
                    try {
                        System.loadLibrary(libnameWithDataModel);
                        Logger.fine("Loaded library " + libnameWithDataModel);
                        return
                            "System.loadLibrary(" + libnameWithDataModel + ")";
                    } catch (UnsatisfiedLinkError e) {
                        Logger.fine("cannot load library: " + e);
                    } catch (Throwable e) {
                        System.err.println("Cannot load library: " + e);
                    }
                }
            } else if (l == Location.LATEST_PATH) {
                if (latestLibPaths == null) {
                    continue;
                }
                String filename = latestLibPaths.get(libname);
                if (filename == null) {
                    continue;
                }
                if (filename.equals(libname)) {
                    Logger.finest("java.library.path = "
                        + System.getProperty("java.library.path"));
                    Logger.fine(l + ": System.loadLibrary(" + libname + ")...");
                    Logger.finest("System.mapLibraryName(" + libname + ") = "
                        + System.mapLibraryName(libname));
                    try {
                        System.loadLibrary(libname);
                        Logger.fine("Loaded library " + libname);
                        return libname;
                    } catch (UnsatisfiedLinkError e) {
                        Logger.fine("cannot load library: " + e);
                    } catch (Throwable e) {
                        System.err.println("Cannot load library: " + e);
                    }
                } else {
                    if (prepareToLoad(filename)) {
                        Logger.fine("System.load(" + filename + ")...");
                        try {
                            System.load(filename);
                            Logger.fine("Loaded library " + filename);
                            return filename;
                        } catch (UnsatisfiedLinkError e) {
                            Logger.fine("error: " + e);
                        } catch (Throwable e) {
                            System.err.println("Cannot load library: " + e);
                        }
                    }
                }
            } else {
                String name[] = null;
                if (l == Location.KNOWN_PATH) {
                    if (knownLibPath == null) {
                        continue;
                    } else {
                        for (int i = 0;
                            i < knownLibPath.length && knownLibPath[i] != null;
                            ++i)
                        {
                            if (knownLibPath[i].contains(libname)) {
                                name = new String[1];
                                name[0] = knownLibPath[i];
                                break;
                            }
                        }
                    }
                }
                if (name == null) {
                    name = mapLibraryName(libname);
                }
                Logger.finest("System.mapLibraryName(" + libname + ") = "
                    + name[0]);

                LibPathIterator it = new LibPathIterator(l, name);
                while (true) {
                    String filename = it.nextName();
                    if (filename == null) {
                        break;
                    }

                    if (prepareToLoad(filename)) {
                        Logger.fine("System.load(" + filename + ")...");
                        try {
                            System.load(filename);
                            Logger.fine("Loaded library " + filename);
                            return filename;
                        } catch (UnsatisfiedLinkError e) {
                            Logger.fine("error: " + e);
                        } catch (Throwable e) {
                            System.err.println("Cannot load library: " + e);
                        }
                    }
                }
            }
        }

        return null;
    }


////////////////////////////////////////////////////////////////////////////////


    /** Downloads the library and default configuration from NCBI.
        Save them where it can be found by LibManager.loadLibrary() */
    private boolean download(String libname) {
        int i = -1;
        if (knownLibPath != null) {
            i = 0;
            while(knownLibPath[i] != null) {
                ++i;
            }
            --i;
        }

        if (downloadLib(libname) == null) {
            return false;
        }

        if (knownLibPath == null || knownLibPath.length < 1) {
            Logger.finest("cannot find downloaded library path: "
                + "skipping configuration download");
            return true;
        }
        int j = 0;
        while(knownLibPath[j] != null) {
            ++j;
        }
        --j;
        if (i != j - 1) {
            Logger.finest("cannot find downloaded library path[]: "
                + "skipping configuration download");
            return true;
        }

        return downloadKfg(knownLibPath[i + 1]);
    }


    /** Fetches the library from NCBI and writes it to where it can be found by
        LibManager.loadLibrary() */
    private String downloadLib(String libname) {
        String request = "cmd=lib&version=1.0&libname=" + libname;

        try {
            request += "&" + osProperties();
        } catch (Exception e) {
            System.err.println("Cannot download library: " + e.getMessage());
            return null;
        }

        for (SratoolkitCgis cgis = new SratoolkitCgis(); ; ) {
            String spec = cgis.nextSpec();
            if (spec == null) {
                break;
            }
            int code = HttpManager.post(spec, request, this, libname);
            if (code == 200) {
                String r = createdFileName;
                createdFileName = null;
                return r;
            } else {
                System.err.println("Cannot download library: " + code);
            }
        }
        return null;
    }


    /** Fetches the configuration from NCBI */
    private boolean downloadKfg(String libpath) {
        Logger.finest("configuration download is disabled");
/*
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
                    } catch (java.io.IOException e) {
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


    /** Check the version of local dll,
        compare it with the latest available;
        download the latest if it is more recent */
    private String checkLib(String libname) {
        Logger.finest("> Checking the version of " + libname + " library...");

        Logger.finest(">> Checking the latest version of "
            + libname + " library...");
        String request = "cmd=vers&libname=" + libname;
        String latest = null;
        for (SratoolkitCgis cgis = new SratoolkitCgis(); ; ) {
            String spec = cgis.nextSpec();
            if (spec == null) {
                break;
            }
            try {
                latest = HttpManager.post(spec, request);
                latest = latest.trim();
                Logger.info
                    ("The latest version of " + libname + " = " + latest);
                break;
            } catch (HttpException e) {
                Logger.finest(e);
            }
        }

        Logger.finest(">> Checking the current version of "
            + libname + " library...");
        String path = load(libname);
        boolean download = false;
        String current = null;
        try {
            if (libname.equals("ncbi-vdb")) {
                current = Manager.getPackageVersion();
            } else if (libname.equals("ngs-sdk")) {
                current = ngs.Package.getPackageVersion();
            } else {
                Logger.warning("It is not known how to check "
                    + "the version of " + libname + " library");
                return null;
            }
        } catch (ngs.ErrorMsg e) {
            Logger.finest(e);
        } catch (UnsatisfiedLinkError e) {
            Logger.finest(e);
        }
        Logger.info("The current version of " + libname + " = " + current);

        if (new Version(current).compareTo(new Version(latest)) < 0) {
            Logger.info("Will download " + libname + " library");
            path = downloadLib(libname);
        } else {
            Logger.info("Will not download " + libname + " library");
        }

        Logger.finest
            ("< ...Done checking the version of " + libname + " library");

        return path;
    }


    /** Execute a process to check the version of dll,
        and download it if it is out of date */
    private void launchLibCheck(String libname) {
        String[] cmdarray = new String[5];
        int i = 0;
        String property = System.getProperty("java.home");
        if (property != null) {
            cmdarray[i] = property + LibPathIterator.fileSeparator()
                   + "bin" + LibPathIterator.fileSeparator() + "java";
            if (!tryJava(cmdarray)) {
                cmdarray[i] = null;
            }
        }
        if (cmdarray[i] == null) {
            cmdarray[i] = "java";
            if (!tryJava(cmdarray)) {
                return;
            }
        }

        cmdarray[++i] = addProperty("java.library.path");
        cmdarray[++i] = addProperty("vdb.log"); 
        cmdarray[++i] = "gov.nih.nlm.ncbi.ngs.LibManager";
        cmdarray[++i] = libname;
        Logger.info(">>> RUNNING CHILD ...");
        try {
            Logger.finest(cmdarray);
            Process p = Runtime.getRuntime().exec(cmdarray);
            BufferedReader bri =
                 new BufferedReader(new InputStreamReader(p.getInputStream()));
            BufferedReader bre =
                new BufferedReader(new InputStreamReader(p.getErrorStream()));
            String line = null;
            while ((line = bre.readLine()) != null)
            {   System.err.println(line); }
            bre.close();
            while ((line = bri.readLine()) != null) {
                String found = null;
                Pattern pattern =  Pattern.compile
                    ("^LibManager: libname='(.*)' filename='(.*)'$");
                Matcher matcher = pattern.matcher(line);
                while (matcher.find()) {
                    found = matcher.group(1);
                    if (!libname.equals(found)) {
                        continue;
                    }
                    String filename = matcher.group(2);
                    latestLibPaths.put(libname, filename);
                    break;
                }
                if (found == null) {
                    System.out.println(line);
                }
            }
            bri.close();
            p.waitFor();
        } catch (Exception e) { Logger.finest(e); }
        Logger.info("<<< Done CHILD");
    }


    /** Make sure we can execute java */
    private boolean tryJava(String[] cmdarray) {
        try {
            Process p = Runtime.getRuntime().exec(cmdarray[0] + " -?");
            if (p.waitFor() == 0) {
                return true;
            }
        } catch (Exception e) {}
        return false;
    }


    /** Create java property option */
    private String addProperty(String key) {
        String property = System.getProperty(key);
        if (property != null) {
            return "-D" + key + "=" + property + "";
        } else {
            return "";
        }
    }


    /** Add the pathname to knownLibPath array */
    private void updateKnownLibPath(String pathname) {
        int l = 9;

        if (knownLibPath == null) {
            knownLibPath = new String[l];
        } else {
            l = knownLibPath.length;
        }

        int i = 0;
        for (i = 0; i < l; ++i) {
            if (knownLibPath[i] == null) {
                break;
            }
        }

        if (i >= l) {
            String tmp[] = knownLibPath;
            l *= 2;
            knownLibPath = new String[l];
            for (i = 0; i < tmp.length; ++i) {
                knownLibPath[i] = tmp[i];
            }
        }

        knownLibPath[i] = pathname;
    }


////////////////////////////////////////////////////////////////////////////////


    private String[] knownLibPath; // location where library was downloaded to

    /** Possible location to search for library to load.
        The order of elements defines library location search order. */
    private Location[] location;

    /** Locations where the latest libraries were found */
    private HashMap<String, String> latestLibPaths;

    /** Is updated by FileCreator methods called by HttpManager */
    private String createdFileName;


////////////////////////////////////////////////////////////////////////////////


    /** Call checkLib for every argument to the version of local dll,
        compare it with the latest available
        and download the latest if it is more recent */
    public static void main(String[] args) {
        LibManager l = new LibManager();
        for (int i = 0; i < args.length; i++) {
            String libname = args[i];
            String path = l.checkLib(libname);
            if (path != null) {
                System.out.println("LibManager: libname='"
                    + libname + "' filename='" + path + "'");
            }
        }
    }


/*******************************************************************************
-Djava.library.path
-Dvdb.log
off if JUST_DO_REGULAR_JAVA_SYSTEM_LOAD_LIBRARY

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
