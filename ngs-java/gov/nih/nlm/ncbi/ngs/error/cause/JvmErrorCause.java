package gov.nih.nlm.ncbi.ngs.error.cause;

public class JvmErrorCause extends LibraryLoadCause {

    public JvmErrorCause(Throwable e) {
        super("JVM error");
        initCause(e);
    }

    @Override
    public String getRecommendation() {
        return null;
    }
}
