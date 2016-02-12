package gov.nih.nlm.ncbi.ngs.error.cause;

public class DownloadInvalidLibraryCause extends LibraryLoadCause {
    public DownloadInvalidLibraryCause() {
        super("invalid/corrupt library was downloaded, please check your network connection or contact sra-toolkit team");
    }
}
