################################################################################
sub PACKAGE      { "ngs-java" }
sub VERSION      { "1.0.0" }
sub PACKAGE_TYPE { 'B' }
sub PACKAGE_NAME { "NGS-JAVA" }
sub PACKAGE_NAMW { "NGS_JAVA" }
sub DEPENDS      {}
sub CONFIG_OUT   { '.' }
sub PKG { ( LNG   => 'JAVA',
            OUT   => 'ncbi-outdir',
            PATH  => '/usr/local/ngs/ngs-java',
            UPATH =>      '$HOME/ngs/ngs-java',
            EXAMP => '1'
#           NGS_SDK_SRC => 1,
        ) }
sub REQ {}

=pod
( { name    => 'ngs-sdk',
              aname   => 'NGS',
              option  => 'with-ngs-sdk-sources',
              origin  => 'I',
              type    => 'SI',
              srcpath => '../ngs-sdk',
              pkgpath => '/usr/local/ngs/ngs-sdk',
              usrpath =>      '$HOME/ngs/ngs-sdk',
              bldpath => '$HOME/ncbi-outdir/ngs-sdk/$OS',
              include => 'ngs/itf/Refcount.h',
              lib     => 'libngs-sdk.so',
              ilib    => 'libngs-bind-c++.a',
        } ) }
=cut

1
