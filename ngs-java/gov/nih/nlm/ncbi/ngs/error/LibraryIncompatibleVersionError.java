package gov.nih.nlm.ncbi.ngs.error;

import gov.nih.nlm.ncbi.ngs.error.cause.LibraryLoadCause;

public class LibraryIncompatibleVersionError extends LibraryLoadError {
    private String outdatedLibPath;

    public LibraryIncompatibleVersionError(String msg, String outdatedLibPath, LibraryLoadCause cause) {
        super(msg, cause);
        this.outdatedLibPath = outdatedLibPath;
    }

    public String getOutdateLibPath() {
        return outdatedLibPath;
    }

    @Override
    public String toString() {
        return generateMsg(getMessage(), getOutdateLibPath());
    }

    private static String generateMsg(String msg, String outdatedLibPath) {
        String result = msg + "\n";
        result += "Library path: " + outdatedLibPath;
        return result;
    }
}
