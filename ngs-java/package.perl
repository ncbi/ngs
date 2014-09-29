################################################################################
sub PACKAGE      { "ngs-java" }
sub PACKAGE_NAME { "NGS-JAVA" }
sub PACKAGE_NAMW { "NGS_JAVA" }
sub PKG { ( LNG => 'JAVA',
            OUT => 'ncbi-outdir',
            PATH => '/usr/local/ngs/ngs-java',
            NGS_SDK_SRC => 1 ) }
sub REQ { ( { name    => 'ngs-sdk',
              namew   => 'NGS',
              option  => 'with-ngs-sdk-prefix',
              srcpath => '../ngs-sdk',
              pkgpath => '/usr/local/ngs/ngs-sdk',
        } ) }

=pod
=cut

1
