package gov.nih.nlm.ncbi.ngs.error;

public class LibraryLoadError extends ExceptionInInitializerError {
    public LibraryLoadError(String msg) {
        super(msg);
    }
}
