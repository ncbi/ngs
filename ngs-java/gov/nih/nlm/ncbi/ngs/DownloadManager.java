package gov.nih.nlm.ncbi.ngs;

class DownloadManager {
    enum DownloadResult {
        SUCCESS,
        FAILED,
        UNSUPPORTED_OS
    }

    DownloadManager(LMProperties properties) {
        this.properties = properties;
    }

    String getLatestVersion(String libname) {
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

    /** Fetches the library from NCBI and writes it using fileCreator */
    DownloadResult downloadLib(FileCreator fileCreator, String libname, String version) {
        String request = "cmd=lib&version=1.0&libname=" + libname;

        try {
            request += "&" + osProperties();
        } catch (Exception e) {
            Logger.warning("Cannot download library: " + e.getMessage());
            return DownloadResult.FAILED;
        }

        for (SratoolkitCgis cgis = new SratoolkitCgis(properties); ; ) {
            String spec = cgis.nextSpec();
            if (spec == null) {
                break;
            }
            int code = HttpManager.post(spec, request, fileCreator, libname);
            if (code == 200) {
                return DownloadResult.SUCCESS;
            } else if (code == 412) {
                Logger.warning("Cannot download library: " + code);
                return DownloadResult.UNSUPPORTED_OS;
            } else {
                Logger.warning("Cannot download library: " + code);
            }
        }
        return DownloadResult.FAILED;
    }

    private String osProperties()
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

    private LMProperties properties;
}
