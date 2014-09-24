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

TOP ?= $(abspath ..)
include $(TOP)/Makefile.config

#VDB_INCDIR = $(abspath $(TOP)/../../interfaces)
#INCDIRS += -I$(VDB_INCDIR)
#VDB_LIBDIR = $(HOME)/lib64

ifndef VDB_INCDIR

std:
	@ echo "These example applications require $(LPFX)ncbi-vdb"

else

TARGETS =         \
	FragTest      \
	AlignTest     \
	AlignSliceTest

all std: $(TARGETS)

endif

clean:
	rm -rf $(addprefix $(BINDIR)/,$(addsuffix *,$(TARGETS))) $(OBJDIR)

.PHONY: default all std $(TARGETS)

TEST_LIBS =           \
	-L$(LIBDIR)       \
	-L$(VDB_LIBDIR)   \
	-lncbi-ngs-c++    \
	-lngs-c++         \
	-lncbi-vdb-static \
	-lpthread         \
	-ldl              \
	-lm

FragTest: $(BINDIR) $(OBJDIR) $(BINDIR)/FragTest$(EXEX)

FRAG_TEST_SRC = \
	FragTest

FRAG_TEST_OBJ = \
	$(addprefix $(OBJDIR)/,$(addsuffix .$(OBJX),$(FRAG_TEST_SRC)))

$(BINDIR)/FragTest$(EXEX): $(FRAG_TEST_OBJ)
	$(LP) $(DBG) $(OPT) -o $@ $^ -L$(LIBDIR) $(TEST_LIBS) 



AlignTest: $(BINDIR) $(OBJDIR) $(BINDIR)/AlignTest$(EXEX)

ALIGN_TEST_SRC = \
	AlignTest

ALIGN_TEST_OBJ = \
	$(addprefix $(OBJDIR)/,$(addsuffix .$(OBJX),$(ALIGN_TEST_SRC)))

$(BINDIR)/AlignTest$(EXEX): $(ALIGN_TEST_OBJ)
	$(LP) $(DBG) $(OPT) -o $@ $^ -L$(LIBDIR) $(TEST_LIBS) 



AlignSliceTest: $(BINDIR) $(OBJDIR) $(BINDIR)/AlignSliceTest$(EXEX)

ALIGNSLICE_TEST_SRC = \
	AlignSliceTest

ALIGNSLICE_TEST_OBJ = \
	$(addprefix $(OBJDIR)/,$(addsuffix .$(OBJX),$(ALIGNSLICE_TEST_SRC)))

$(BINDIR)/AlignSliceTest$(EXEX): $(ALIGNSLICE_TEST_OBJ)
	$(LP) $(DBG) $(OPT) -o $@ $^ -L$(LIBDIR) $(TEST_LIBS) 
