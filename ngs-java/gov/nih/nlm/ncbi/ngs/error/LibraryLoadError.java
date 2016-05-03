package gov.nih.nlm.ncbi.ngs.error;

import gov.nih.nlm.ncbi.ngs.error.cause.LibraryLoadCause;

public class LibraryLoadError extends ExceptionInInitializerError {
    LibraryLoadCause cause;
    String errorMessage;

    public LibraryLoadError(String msg, LibraryLoadCause cause) {
        super(generateMsg(msg, cause, cause.getRecommendation()));

        this.errorMessage = generateMsg(msg, cause, null);
        this.cause = cause;
    }

    public LibraryLoadError(String msg, LibraryLoadCause cause, String recommendation) {
        super(generateMsg(msg, cause, recommendation));

        this.cause = cause;
    }

    @Override
    public LibraryLoadCause getCause() {
        return cause;
    }

    /**
     * @return error message, without recommendation
     */
    public String getErrorMessage() {
        return errorMessage;
    }

    /**
     * @return recommendation for the user regarding error, can be null
     */
    public String getRecommendation() {
        return cause.getRecommendation();
    }

    private static String generateMsg(String msg, LibraryLoadCause cause, String recommendation) {
        String result = msg;
        if (cause != null) {
            result += ", " + cause.getMessage();
            if (recommendation != null) {
                result += "\n" + recommendation;
            }
        }

        return result;
    }
}
