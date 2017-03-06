# NCBI External Developer Release:

## NGS SDK 1.3.1
**March 6, 2017**

  **build**: configure detects location of ngs libraries  
  **build**: configure was fixed to skip options unrecognized by gcc 4.4.7  
  **build**: created sra-toolkit Debian package  
  **build**: fixed a bug in 'configure' when in could not find source files in repository saved with non-standard name  

## NGS SDK 1.3.0
**October 7, 2016**

### HTTPS-ENABLED RELEASE

  **build, ngs-tools**: Now ngs-tools look for its dependencies using their normal build paths and does not reconfigure them  
  **build, ngs-tools**: Now ngs-tools use CMAKE_INSTALL_PREFIX for installation path  
  **kns**: All tools and libraries now support https  
  **ngs**: Fixed all crashes when using null as string in ngs-java APIs  
  **ngs**: NGS_ReferenceGetChunk() will now return chunks potentially exceeding 5000 bases  
  **ngs**: fixed potential concurrency issues at exit, when called from Java  
  **ngs**: ngs-java and ngs-python auto-download (of native libraries) now works through HTTPS  
  **ngs**: read fragments of length 0 are now ignored  
  **ngs, ngs-tools, ref-variation**: added class ngs-vdb::VdbAlignment, featuring method IsFirst()  
  **ngs-engine**: improved diagnostic messages  
  **ngs-tools**: Fixed Makefiles to keep supporting "./configure; make" build of sra-search, alongside CMake-based build.  


## NGS SDK 1.2.5
**July 12, 2016**

  **blast, kfg, ncbi-vdb, sra-tools, vfs**: restored possibility to disable local caching  
  **htsjdk**: Several JVM crashes related to a number of open files were fixed. New property to disable auto-download was integrated into HTSJDK  
  **kfg**: When loading configuration files on Windows USERPROFILE environment variable is used before HOME  
  **ngs, search, sra-search**: sra-search was modified to support multiple threads.  
  **ngs-engine, ngs-tools, sra-tools, vfs**: The "auxiliary" nodes in configuration are now ignored  
  **ngs-engine**: Added support for blob-by-blob access to SEQUENCE table  
  **ngs-engine**: removed a potential memory leak in NGS_CursorMake()  
  **ngs**: Fixed a bug in ngs::Alignment::getMateReferenceSpec() affecting pre-March 2015 runs  
  **ngs**: now supports parallel compilation with "make -j N"  
  **vfs**: environment variable VDB_PWFILE is no longer used  


## NGS SDK 1.2.4
**May 25, 2016**

  **build**: MSVS 2013 toolset (12.0) is now supported across all repositories  
  **doc, ngs**: updated javadoc to include throws and other missing tags  
  **examples, ngs**: added DumpReferenceFASTA.py example  
  **htsjdk**: added code to HTS-JDK  to avoid involving NGS unless we are sure that it is being requested  
  **ngs, ngs-engine**: Added filtering to NGS of secondary alignments that do not have primary alignments  
  **ngs, test, ngs-python**: fixed bug in String processing for Python 3.x  
  **ngs-engine**: ncbi-ngs engine was updated - fixed a bug that made NGS read iterator return 0 reads on WGS accessions.  
  **ngs**: Improved native library load in ngs-java, enhanced its error reporting and added a mode to disable auto-download of native libraries  
  **ngs**: Python code will check for the latest version of the libraries and update if newer ones are available  
  **ngs**: check for the latest version of the libraries and update if newer ones are available  
  **ngs**: simplified ngs-python bindings  
