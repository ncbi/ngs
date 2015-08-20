#===========================================================================
#
#                           PUBLIC DOMAIN NOTICE
#              National Center for Biotechnology Information
#
# This software/database is a "United States Government Work" under the
# terms of the United States Copyright Act.  It was written as part of
# the author's official duties as a United States Government employee and
# thus cannot be copyrighted.  This software/database is freely available
# to the public for use. The National Library of Medicine and the U.S.
# Government have not placed any restriction on its use or reproduction.
#
# Although all reasonable efforts have been taken to ensure the accuracy
# and reliability of the software and data, the NLM and the U.S.
# Government do not and cannot warrant the performance or results that
# may be obtained by using this software or data. The NLM and the U.S.
# Government disclaim all warranties, express or implied, including
# warranties of performance, merchantability or fitness for any particular
# purpose.
#
# Please cite the author in any work or product based on this material.
#
#===========================================================================
#
import sys
import traceback

from ngs import NGS
from ngs.ErrorMsg import ErrorMsg
from ngs.ReadCollection import ReadCollection
from ngs.Alignment import Alignment
from ngs.AlignmentIterator import AlignmentIterator


def run(acc, splitNum, splitNo): # this function doesn't release NGS objects however it might
    # open requested accession using SRA implementation of the API
    run = NGS.openReadCollection(acc)
    run_name = run.getName()
    
    # compute window to iterate through
    MAX_ROW = run.getAlignmentCount()
    chunk = MAX_ROW / splitNum
    first = int(round(chunk * (splitNo-1)))
    next_first = int(round(chunk * (splitNo)))
    if next_first > MAX_ROW:
        next_first = MAX_ROW

    # start iterator on reads
    it = run.getAlignmentRange(first+1, next_first-first, Alignment.primaryAlignment)
    i = 0
    while it.nextAlignment():
        print (
            it.getReadId() + "\t" +
            it.getReferenceSpec() + "\t" +
            str(it.getAlignmentPosition()) + "\t" +
            it.getShortCigar(False) + "\t" +
            it.getFragmentBases() + "\t" +
            ("aligned" if it.isAligned() else "unaligned")
        )
        i += 1
    print ("Read {} alignments for {}".format(i, run_name))


if len(sys.argv) != 4:
    print ("Usage: FragTest accession NumChunks ChunkNo\n")
else:
    try:
        run(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
    except ErrorMsg, x:
        print (x)
        traceback.print_exc()
        # x.printStackTrace - not implemented
    except BaseException, x:
        traceback.print_exc()
