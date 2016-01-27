package gov.nih.nlm.ncbi.ngs;


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

    String get(String libname, String latest)
    {   return get(libname, latest, Logger.Level.FINE); }

    String getLoud(String libname, String latest)
    {   return get(libname, latest, Logger.Level.INFO); }

    String cfgFilePath() { return path; }

    void notLoaded(String libname) {
        String node = "/dll/" + libname + "/" + bits + "/loaded/";
        remove(node + "path");
        remove(node + "version");
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

    private String get(String libname, String latest, Logger.Level level) {
        String path = get(libname, "loaded", latest, level);
        if (path == null) {
            path = get(libname, "saved" , latest, level);
        }
        return path;
    }

    private String get
            (String libname, String name, String latest, Logger.Level level)
    {
        String node = "/dll/" + libname + "/" + bits + "/" + name +"/";
        String version = getProperty(node + "version");
        if (version != null) {
            String path = getProperty(node + "path");
            if (path != null) {
                File f = new File(path);
                if (f.exists() &&
                        new Version(version).compareTo(new Version(latest)) < 0)
                {
                    remove(node + "path");
                    remove(node + "version");
                    dirty = true;
                } else {//new Exception().printStackTrace();
                    Logger.log(level, "The version of the most recently"
                            + " loaded " + libname + " = " + version);
                    return path;
                }
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
