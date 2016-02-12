package gov.nih.nlm.ncbi.ngs.error.cause;

public class DownloadDisabledCause extends LibraryLoadCause {
    public DownloadDisabledCause() {
        super("auto-download is disabled," +
                " please enable auto-download or install sra-toolkit manually");
    }
}
