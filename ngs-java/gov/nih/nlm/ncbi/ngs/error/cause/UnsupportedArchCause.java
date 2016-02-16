package gov.nih.nlm.ncbi.ngs.error.cause;

public class UnsupportedArchCause extends LibraryLoadCause {
    public UnsupportedArchCause() {
        super("download failed - there is no build for your OS/version available");
    }

    @Override
    public String getRecommendation() {
        return "Please try building ngs and ncbi-vdb from sources: https://github.com/ncbi/ngs/wiki/Building-and-Installing-from-Source";
    }
}
