package gov.nih.nlm.ncbi.ngs;

import java.util.HashMap;

class DownloadManager {
    DownloadManager(LMProperties properties) {
        this.properties = properties;
        latest = new Latest(properties);
    }

    String getLatestVersion(String libname) {
        return latest.get(libname);
    }

    /** Fetches the library from NCBI and writes it using fileCreator */
    boolean downloadLib(FileCreator fileCreator, String libname, String version) {
        String request = "cmd=lib&version=1.0&libname=" + libname;

        try {
            request += "&" + osProperties();
        } catch (Exception e) {
            Logger.warning("Cannot download library: " + e.getMessage());
            return false;
        }

        for (SratoolkitCgis cgis = new SratoolkitCgis(properties); ; ) {
            String spec = cgis.nextSpec();
            if (spec == null) {
                break;
            }
            int code = HttpManager.post(spec, request, fileCreator, libname);
            if (code == 200) {
                return true;
            } else {
                Logger.warning("Cannot download library: " + code);
            }
        }
        return false;
    }

    static String osProperties()
            throws Exception
    {
        String request = "os_name=";
        String name = System.getProperty("os.name");
        if (name == null) {
            throw new Exception("Cannot detect OS");
        }
        request += name + "&bits=" + LibManager.detectJVM().intString();
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

    private  static class SratoolkitCgis {
        private SratoolkitCgis(LMProperties properties) {
            spec = properties.getProperty("/servers/sratookit-cgi");
            if (spec != null) {
                Logger.warning("Use " + spec + " from " + properties.cfgFilePath());
            } else {
                spec = "http://trace.ncbi.nlm.nih.gov/Traces/sratoolkit/sratoolkit.cgi";
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

    private static class Latest extends HashMap<String, String> {
        private Latest(LMProperties properties) {
            this.properties = properties;
        }

        private String get(String libname) {
            String value = super.get(libname);
            if (value == null) {
                value = latest(libname);
                if (value != null) {
                    put(libname, value);
                }
            }
            return value;
        }

        private String latest(String libname) {
            Logger.finest(
                    ">> Checking the latest version of " + libname + " library...");

            String request = "cmd=vers&libname=" + libname;

            for (SratoolkitCgis cgis = new SratoolkitCgis(properties); ; ) {
                String spec = cgis.nextSpec();
                if (spec == null) {
                    break;
                }

                try {
                    String latest = HttpManager.post(spec, request);
                    latest = latest.trim();
                    Logger.info
                            ("The latest version of " + libname + " = " + latest);
                    return latest;
                } catch (HttpException e) {
                    Logger.finest(e);
                }
            }

            Logger.info("Cannot check the latest version of " + libname);
            return null;
        }

        private LMProperties properties;
    }

    private LMProperties properties;
    private Latest latest;
}
