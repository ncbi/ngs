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

TARGETS =             \
    NGS-JavaTest.jar

std: $(TARGETS)

clean:
	rm -f $(TARGETS) examples/*.class

.PHONY: default std $(TARGETS)

TEST_SRC =                      \
	examples/FragTest.java      \
	examples/AlignTest.java     \
	examples/AlignSliceTest.java

NGS-JavaTest.jar: $(TEST_SRC)
	javac $^ -d . -classpath $(CLASSPATH) && touch $@

# ===========================================================================
#
# example runs

run_frag: NGS-JavaTest.jar
	java -classpath $^:$(CLASSPATH) examples.FragTest ERR225922 1 1

run_align: NGS-JavaTest.jar
	java -classpath $^:$(CLASSPATH) examples.AlignTest ERR225922 1 1

run_align_slice: NGS-JavaTest.jar
	java -classpath $^:$(CLASSPATH) examples.AlignSliceTest SRR1121656 1 10000 20000


.PHONY: run_align run_align_slice run_frag
