package gov.nih.nlm.ncbi.ngs;


import java.util.Date;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

class LMProperties extends java.util.Properties {
    LMProperties(String bitsStr) {
        bits = bitsStr;

        path = LibPathIterator.ncbiHome();

        if (path != null) {
            path +=
                    LibPathIterator.fileSeparator() + "LibManager.properties";

            try {
                FileInputStream inStream = new FileInputStream(path);
                load(inStream);
                inStream.close();
            } catch (IOException e) {}
        }
    }

    public Object setProperty(String key, String value) {
        String saved = getProperty(key);
        if (saved != null && saved.equals(value)) {
            return saved;
        } else {
            dirty = true;
            return super.setProperty(key, value);
        }
    }

    String get(String libname, String minimalVersion)
    {   return get(libname, minimalVersion, Logger.Level.FINE); }

    String getLoud(String libname, String minimalVersion)
    {   return get(libname, minimalVersion, Logger.Level.INFO); }

    String cfgFilePath() { return path; }

    void setLastSearch(String libname) {
        setProperty("/dll/" + libname + "/" + bits + "/last-search", Long.toString(new Date().getTime()));
    }

    Date getLastSeach(String libname) {
        String dateLong = getProperty("/dll/" + libname + "/" + bits + "/last-search");
        if (dateLong == null) {
            return null;
        }

        return new Date(Long.valueOf(dateLong));
    }

    void notLoaded(String libname) {
        String node = "/dll/" + libname + "/" + bits + "/";
        remove(node + "loaded/path");
        remove(node + "loaded/version");
        remove(node + "last-search");
        dirty = true;
    }

    void loaded(String libname, String version, String path)
    {   set(libname, "loaded", version, path); }

    void saved(String libname, String version, String path)
    {   set(libname, "saved", version, path); }

    void store() {
        try {
            if (!dirty) {
                return;
            }

            File file = new File(cfgFilePath());
            File parent = file.getParentFile();
            if (parent == null) {
                Logger.finest
                        ("Cannot find parent directory to store properties");
                return;
            } else if (!parent.exists()) {
                if (!parent.mkdir()) {
                    Logger.finest("Cannot create " + parent.getName());
                    return;
                }
                parent.setExecutable(false, false);
                parent.setReadable(false, false);
                parent.setWritable(false, false);
                parent.setExecutable(true, true);
                parent.setReadable(true, true);
                parent.setWritable(true, true);
            }
            FileOutputStream fileOut = new FileOutputStream(file);
            store(fileOut, null);
            fileOut.close();

            dirty = false;
        } catch (IOException e) {
            Logger.finest(e);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    private void set(String libname, String name, String version,
                     String path)
    {
        String node = "/dll/" + libname + "/" + bits + "/" + name +"/";

        setProperty(node + "path"   , path);
        setProperty(node + "version", version);
    }

    private String get(String libname, String minimalVersion, Logger.Level level) {
        String path = get(libname, "loaded", minimalVersion, level);
        if (path == null) {
            path = get(libname, "saved" , minimalVersion, level);
        }
        return path;
    }

    private String get
            (String libname, String name, String minimalVersion, Logger.Level level)
    {
        String node = "/dll/" + libname + "/" + bits + "/" + name +"/";
        String version = getProperty(node + "version");
        if (version != null) {
            String path = getProperty(node + "path");
            if (path != null) {
                File f = new File(path);
                // TODO: check with Kurt whether we should remove old version from config file
                /*if (f.exists() &&
                        new Version(version).compareTo(new Version(minimalVersion)) < 0)
                {
                    remove(node + "path");
                    remove(node + "version");
                    dirty = true;
                } else {*///new Exception().printStackTrace();
                    Logger.log(level, "The version of the most recently"
                            + " loaded " + libname + " = " + version);
                    return path;
                /*}*/
            } else {
                remove(node + "version");
                dirty = true;
            }
        }
        return null;
    }

    private String path;
    private String bits;
    private boolean dirty;
}
