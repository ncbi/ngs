package gov.nih.nlm.ncbi.ngs.error;

import gov.nih.nlm.ncbi.ngs.error.cause.LibraryLoadCause;

public class LibraryNotFoundError extends LibraryLoadError {
    public LibraryNotFoundError(String msg, LibraryLoadCause cause) {
        super(msg, cause);
    }
}
