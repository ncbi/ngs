ngs
===

## ANNOUNCEMENTS:

### This repository is now frozen. The NGS project has moved to https://github.com/ncbi/sra-tools/ngs. See the full anouncement below.

February 10, 2022

NCBI's SRA changed the source build system to use CMake in toolkit release 3.0.0. This change is an important step to improve developers' productivity as it provides unified cross platform access to support multiple build systems. This change affects developers building NCBI SRA tools from source. Old makefiles and build systems are no longer supported.

This change also includes the structure of GitHub repositories, which underwent consolidation to provide an easier environment for building tools and libraries (NGS libs and dependencies are consolidated). Consolidation of NGS libraries and dependencies provides better usage scope isolation and makes building more straightforward.

#### **Affected repositories**

1) [ncbi/ngs](https://github.com/ncbi/ngs) (This repository)

   This repository is frozen. All future development will take place in GitHub repository ncbi/sra-tools (this repository), under subdirectory `ngs/`.

2) [ncbi/ncbi-vdb](https://github.com/ncbi/ncbi-vdb)

   This project's build system is based on CMake. The libraries providing access to SRA data in VDB format via the NGS API have moved to GitHub repository
   [ncbi/sra-tools](https://github.com/ncbi/ncbi-vdb).

   | Old (base URL: https://github.com/ncbi/ncbi-vdb) | New (base URL: https://github.com/ncbi/sra-tools) |
   | -------------------------------------------------| ------------------------------------------------- |
   | `libs/ngs`        | `ngs/ncbi/ngs`     |
   | `libs/ngs-c++`    | `ngs/ncbi/ngs-c++` |
   | `libs/ngs-jni`    | `ngs/ncbi/ngs-jni` |
   | `libs/ngs-py`     | `ngs/ncbi/ngs-py`  |
   | `libs/vdb-sqlite` | `libs/vdb-sqlite`  |
   | `test/ngs-java`   | `test/ngs-java`    |
   | `test/ngs-python` | `test/ngs-python`  |


3) [ncbi/sra-tools](https://github.com/ncbi/sra-tools)

   This project's build system is based on CMake. The project acquired some new components, as listed in the table above.

----

# NGS Language Bindings

NGS is a new, domain-specific API for accessing reads, alignments and pileups produced from Next Generation Sequencing. The API itself is independent from any particular back-end implementation, and supports use of multiple back-ends simultaneously. It also provides a library for building new back-end "engines". The engine for accessing SRA data is contained within the sister repository [ncbi-vdb](https://github.com/ncbi/ncbi-vdb).

The API is currently expressed in C++, Java and Python languages. The design makes it possible to maintain a high degree of similarity between the code in one language and code in another - especially between C++ and Java.

# Distribution of NGS APIs and Examples

You can find information about building and running examples in README file located in [tar archives](https://github.com/ncbi/ngs/wiki/Downloads).

# Documentation

Further documentation is available on the [wiki](https://github.com/ncbi/ngs/wiki).
