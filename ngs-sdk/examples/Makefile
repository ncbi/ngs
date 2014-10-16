# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#               National Center for Biotechnology Information
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government have not placed any restriction on its use or reproduction.
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
#  Please cite the author in any work or product based on this material.
#
# ===========================================================================


default: std

# All of this section is to provide semi-automatic detection of
# required libraries. They SHOULD have been built and installed
# already before building these examples.

NCBI_VDB_LIBDIR = $(dir $(firstword $(shell find $(subst :, ,$(LD_LIBRARY_PATH) -maxdepth 1 -name libncbi-vdb.a) 2>/dev/null) ) )
NGS_LIBDIR = $(dir $(firstword $(shell find $(subst :, ,$(LD_LIBRARY_PATH) -maxdepth 1 -name libngs-c++.a) 2>/dev/null) ) )

$(NCBI_VDB_LIBDIR)/libncbi-vdb.a:
    @ echo "These example applications require $(@F). Please install ncbi-vdb libraries and add their location to LD_LIBRARY_PATH."
	@ false

$(NGS_LIBDIR)/libngs-c++.a:
    @ echo "These example applications require $(@F). Please install ngs libraries and add their location to LD_LIBRARY_PATH."
	@ false

# END OF LIBRARY DETECTION

TARGETS =         \
    FragTest      \
    AlignTest     \
    AlignSliceTest

# This rule triggers detection of the libraries
std: $(NCBI_VDB_LIBDIR)/libncbi-vdb.a $(NGS_LIBDIR)/libngs-c++.a $(TARGETS)

clean:
    rm -f $(TARGETS) *.o

.PHONY: default std $(TARGETS)

TEST_LIBS =              \
    -L$(NCBI_VDB_LIBDIR) \
    -L$(NGS_LIBDIR)      \
    -lncbi-ngs-c++       \
    -lngs-c++            \
    -lncbi-vdb-static    \
    -lpthread            \
    -ldl                 \
    -lm

FRAG_TEST_OBJ = \
    FragTest.o

FragTest: $(FRAG_TEST_OBJ)
    g++ -o $@ $^ $(TEST_LIBS) 


ALIGN_TEST_OBJ = \
    AlignTest.o

AlignTest: $(ALIGN_TEST_OBJ)
    g++ -o $@ $^ $(TEST_LIBS) 


ALIGNSLICE_TEST_OBJ = \
    AlignSliceTest.o

AlignSliceTest: $(ALIGNSLICE_TEST_OBJ)
    g++ -o $@ $^ $(TEST_LIBS) 

# ===========================================================================
#
# example runs

run_frag:  FragTest
    ./$^ ERR225922 1 1

run_align: AlignTest
    ./$^ ERR225922 1 1

run_align_slice: AlignSliceTest
    ./$^ SRR1121656 1 10000 20000 


.PHONY: run_align run_align_slice run_frag
