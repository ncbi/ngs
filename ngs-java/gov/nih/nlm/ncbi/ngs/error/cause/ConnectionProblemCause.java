package gov.nih.nlm.ncbi.ngs.error.cause;

public class ConnectionProblemCause extends LibraryLoadCause {
    public ConnectionProblemCause() {
        super("auto-download failed - connection problem");
    }

    @Override
    public String getRecommendation() {
        return "Please check your network connection";
    }
}
