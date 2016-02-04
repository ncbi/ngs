package gov.nih.nlm.ncbi.ngs.error;

import java.util.List;

public class LibraryTooOldError extends LibraryLoadError {
    private List<String> outdatedLibPaths;

    public LibraryTooOldError(String msg, List<String> outdatedLibPaths) {
        super(generateMsg(msg, outdatedLibPaths));
        this.outdatedLibPaths = outdatedLibPaths;
    }

    public List<String> getOutdateLibPaths() {
        return outdatedLibPaths;
    }

    private static String generateMsg(String msg, List<String> outdatedLibPaths) {
        String result = msg + ". Located libraries are too old:";
        for (String libpath : outdatedLibPaths) {
            result += "\n" + libpath;
        }
        return result;
    }
}
