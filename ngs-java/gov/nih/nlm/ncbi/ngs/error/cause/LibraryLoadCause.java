package gov.nih.nlm.ncbi.ngs.error.cause;

public abstract class LibraryLoadCause extends Exception {
    public LibraryLoadCause(String msg) {
        super(msg);
    }

    public abstract String getRecommendation();
}
