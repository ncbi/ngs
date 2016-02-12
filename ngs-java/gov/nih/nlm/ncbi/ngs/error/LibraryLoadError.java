package gov.nih.nlm.ncbi.ngs.error;

import gov.nih.nlm.ncbi.ngs.error.cause.LibraryLoadCause;

public class LibraryLoadError extends ExceptionInInitializerError {
    LibraryLoadCause cause;

    public LibraryLoadError(String msg, LibraryLoadCause cause) {
        super(generateMsg(msg, cause));

        this.cause = cause;
    }

    @Override
    public LibraryLoadCause getCause() {
        return cause;
    }

    private static String generateMsg(String msg, LibraryLoadCause cause) {
        String result = msg;
        if (cause != null) {
            result += " - " + cause.getMessage();
        }

        return result;
    }
}
