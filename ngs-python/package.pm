################################################################################
sub PACKAGE      { "ngs-python" }
sub VERSION      { "1.0.0" }
sub PACKAGE_TYPE { 'B' }
sub PACKAGE_NAME { "NGS-PYTHON" }
sub PACKAGE_NAMW { "NGS_PYTHON" }
sub DEPENDS      {}
sub CONFIG_OUT   { '.' }
sub PKG { ( LNG  => 'PYTHON',
            OUT  => 'ncbi-outdir',
            PATH  => '/usr/local/ngs/ngs-python',
            UPATH =>      '$HOME/ngs/ngs-python',
            EXAMP => '1', ) }
sub REQ { (
            { name    => 'ngs-sdk',
              namew   => 'NGS',
              option  => 'with-ngs-sdk-prefix',
              origin  => 'I',
              type    => 'L',
              srcpath => '../ngs-sdk',
              pkgpath => '/usr/local/ngs/ngs-sdk',
              usrpath =>      '$HOME/ngs/ngs-sdk',
              bldpath => '$HOME/ncbi-outdir/ngs-sdk',
              include => 'ngs/itf/Refcount.h',
              lib     => 'libngs-sdk.so',
              ilib    => 'libngs-bind-c++.a',
            },
            { name    => 'ncbi-vdb',
              namew   => 'VDB',
              option  => 'with-ncbi-vdb-prefix',
              origin  => 'I',
              type    => 'L',
              srcpath => '../../ncbi-vdb',
              pkgpath => '/usr/local/ncbi/ncbi-vdb',
              usrpath =>      '$HOME/ncbi/ncbi-vdb',
              bldpath => '$HOME/ncbi-outdir/ncbi-vdb',
              include => 'klib/rc.h',
              lib     => 'libncbi-vdb.so',
              ilib    => 'libkapp.a',
             } ) }

1
