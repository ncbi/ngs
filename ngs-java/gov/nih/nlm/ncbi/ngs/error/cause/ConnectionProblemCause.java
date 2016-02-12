package gov.nih.nlm.ncbi.ngs.error.cause;

public class ConnectionProblemCause extends LibraryLoadCause {
    public ConnectionProblemCause() {
        super("connection problem, check your network connection");
    }
}
