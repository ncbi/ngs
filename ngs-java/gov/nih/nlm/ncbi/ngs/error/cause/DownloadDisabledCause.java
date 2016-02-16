package gov.nih.nlm.ncbi.ngs.error.cause;

public class DownloadDisabledCause extends LibraryLoadCause {
    public DownloadDisabledCause() {
        super("auto-download is disabled");
    }

    @Override
    public String getRecommendation() {
        return "Please enable auto-download or install ngs and ncbi-vdb manually:" +
                " https://github.com/ncbi/ngs/wiki/Downloads";
    }
}
