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
        ) }
sub REQ {}
1
