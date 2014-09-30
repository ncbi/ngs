################################################################################
sub PACKAGE      { "ngs-python" }
sub VERSION      { "1.0.0" }
sub PACKAGE_TYPE { 'B' }
sub PACKAGE_NAME { "NGS-PYTHON" }
sub PACKAGE_NAMW { "NGS_PYTHON" }
sub CONFIG_OUT   { '.' }
sub PKG { ( LNG  => 'PYTHON',
            OUT  => 'ncbi-outdir',
            PATH  => '/usr/local/ngs/ngs-python',
            UPATH =>      '$HOME/ngs/ngs-python',
        ) }
sub REQ { ( { name    => 'ngs-sdk',
              namew   => 'NGS',
              option  => 'with-ngs-sdk-prefix',
              type    => 'I',
              srcpath => '../ngs-sdk',
              pkgpath => '/usr/local/ngs/ngs-sdk',
              usrpath =>      '$HOME/ngs/ngs-sdk',
              bldpath => '$HOME/ncbi-outdir/ngs-sdk/$OS',
              include => 'ngs/itf/Refcount.h'
        } ) }

1
