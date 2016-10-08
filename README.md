ngs
===

# NGS Language Bindings

NGS is a new, domain-specific API for accessing reads, alignments and pileups produced from Next Generation Sequencing. The API itself is independent from any particular back-end implementation, and supports use of multiple back-ends simultaneously. It also provides a library for building new back-end "engines". The engine for accessing SRA data is contained within the sister repository [ncbi-vdb](https://github.com/ncbi/ncbi-vdb).

The API is currently expressed in C++, Java and Python languages. The design makes it possible to maintain a high degree of similarity between the code in one language and code in another - especially between C++ and Java.

### IMPORTANT ANNOUNCEMENT
As was announced in [OMB memorandum M-15-13](https://www.whitehouse.gov/sites/default/files/omb/memoranda/2015/m-15-13.pdf) on June 8, 2015, NCBI and all Federal systems will be transitioning to using HTTPS-only protocols before the end of 2016. This change will affect any software that uses NCBI APIs such as the E-utilities or NCBI software toolkits such as `sra-tools`, `ncbi-vdb` or `ngs`.

The NLM and NCBI may implement the switch to HTTPS-only as early as September 30, 2016.

In particular, software products that depend on `sra-tools`, `ncbi-vdb` or `ngs` may not function as expected after September 30 unless they are properly updated from this site or by the software provider.

If you use software that accesses NCBI SRA data in any way, your software will likely be affected by this change. Please check with your software provider for recent udpates or patches, and be sure to acquire these before September 30.
 
If you develop software that relies on `sra-tools`, `ncbi-vdb` or `ngs` in any way, you will likely need to update your code so that it accesses NCBI using HTTPS.

We have released new tools with version 1.3.0 that are HTTPS compatible and `M-15-13` compliant as of October 7, 2016. Please be certain to [update all of your binaries](https://github.com/ncbi/sra-tools/wiki/Downloads) and configuration files.

# Documentation

Further documentation is available on the [wiki](https://github.com/ncbi/ngs/wiki).
