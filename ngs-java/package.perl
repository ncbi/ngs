################################################################################
sub PACKAGE      { "ngs-java" }
sub PACKAGE_NAME { "NGS-JAVA" }
sub PKG { ( LNG => 'JAVA',
            OUT => 'ncbi-outdir',
            PATH => '/usr/local/ngs/ngs-java',
            NGS_SDK_SRC => 1 ) }
sub REQ {}
=pod
 ( { name   =>      'ngs-sdk',
              option => 'with-ngs-sdk-prefix',
              path   => '../' } ) }
=cut
1
