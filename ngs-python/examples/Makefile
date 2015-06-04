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

std: 

clean:

.PHONY: default std $(TARGETS)

PYTHON_VERS = $(shell python -V 2>&1)
$(warning PYTHON_VERS=$(PYTHON_VERS))

# there is a separate set of sources for 2.5 and 2.6
ifeq (Python 2.6,$(shell expr match "$(PYTHON_VERS)" "\(Python 2.6\)"))
PYTHON_VERS=26
elif (Python 2.5,$(shell expr match "$(PYTHON_VERS)" "\(Python 2.5\)"))
PYTHON_VERS=26
else
PYTHON_VERS=
endif

NGS_EXAMPLES = \
	FragTest \
	AlignTest \
	AlignSliceTest \
	PileupTest \
	RefTest \

# ===========================================================================
#
# example runs

run_frag: 
	python FragTest$(PYTHON_VERS).py ERR225922 10000 2 

run_align: 
	python AlignTest$(PYTHON_VERS).py ERR225922 10000 2 

run_align_slice: 
	python AlignSliceTest$(PYTHON_VERS).py SRR1121656 1 1 9999

run_pileup:
	python PileupTest$(PYTHON_VERS).py SRR1121656 1 9999 10003

run_ref:
	python RefTest$(PYTHON_VERS).py SRR1121656
    
ALL_TESTS = run_frag run_align run_align_slice run_pileup run_ref
    
run_all: $(ALL_TESTS)
    
.PHONY: run_align run_align_slice run_frag
