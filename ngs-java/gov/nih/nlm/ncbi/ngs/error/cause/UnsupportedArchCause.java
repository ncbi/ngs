package gov.nih.nlm.ncbi.ngs.error.cause;

public class UnsupportedArchCause extends LibraryLoadCause {
    public UnsupportedArchCause() {
        super("download failed - there is no build for your OS/version available,"+
                " try building sra-toolkit from sources");
    }
}
