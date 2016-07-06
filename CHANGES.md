# NCBI External Developer Release:

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
