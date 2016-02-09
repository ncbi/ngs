package gov.nih.nlm.ncbi.ngs.error;

import java.util.List;

public class LibraryTooOldError extends LibraryLoadError {
    private List<String> outdatedLibPaths;

    public LibraryTooOldError(String msg, List<String> outdatedLibPaths) {
        super(msg + ". Located libraries are too old");
        this.outdatedLibPaths = outdatedLibPaths;
    }

    public List<String> getOutdateLibPaths() {
        return outdatedLibPaths;
    }

    @Override
    public String toString() {
        return generateMsg(getMessage(), getOutdateLibPaths());
    }

    private static String generateMsg(String msg, List<String> outdatedLibPaths) {
        String result = msg + ":";
        for (String libpath : outdatedLibPaths) {
            result += "\n" + libpath;
        }
        return result;
    }
}
