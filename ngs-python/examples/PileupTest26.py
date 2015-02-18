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
from ngs.Reference import Reference
from ngs.PileupEvent import PileupEvent


def run(acc, refName, start, stop):
    # open requested accession using SRA implementation of the API
    run = NGS.openReadCollection(acc):
    run_name = run.getName()
    
    # get requested reference
    ref = run.getReference(refName):
    # start iterator on requested range
    it = ref.getPileupSlice(start-1, stop-start+1):
    i = 0
    while it.nextPileup():
        qual = ""
        base = ""
    
        print ("%s\t%d\t%s\t%d" % (
            it.getReferenceSpec(),
            it.getReferencePosition()+1,
            it.getReferenceBase(),
            it.getPileupDepth(),
            ))
        while it.nextPileupEvent():
            e = it.getEventType()
            if (e & PileupEvent.alignment_start ) != 0:
                base = base + '^'
                base = base + chr(it.getMappingQuality() + 33)
    
            if ( e & PileupEvent.insertion ) != 0:
                base = base + '+'
                ibases= it.getInsertionBases()
                c = len(ibases)
                base = base + str(c)
    
                if ( e & PileupEvent.alignment_minus_strand ) != 0:
                    base = base + ibases.lower()
                else:
                    base = base + ibases
    
            evt = e & 7
    
            if ( e & PileupEvent.alignment_minus_strand ) != 0:
                if evt == PileupEvent.deletion:
                    base = base + '<'
                elif evt == PileupEvent.match:
                    base = base + ','
                elif evt == PileupEvent.mismatch:
                    base = base + str(it.getAlignmentBase()).lower()
            else:
                if evt == PileupEvent.deletion:
                    base = base + '>'
                elif evt == PileupEvent.match:
                    base = base + '.'
                elif evt == PileupEvent.mismatch:
                    base = base + str(it.getAlignmentBase()).upper()
            
            if ( e & PileupEvent.alignment_stop ) != 0:
                base = base + '$'
            
            qual = qual + it.getAlignmentQuality()
            
        i += 1
        print ("\t%s\t%s" % (base, qual))
    print ("Read %d pileups for %s" % (i, run_name))

if len(sys.argv) != 5:
    print ("Usage: PileupTest26 accession reference start stop\n")
else:
    try:
        run(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
    except ErrorMsg, x:
        print (x)
        traceback.print_exc()
        # x.printStackTrace - not implemented
    except BaseException, x:
        traceback.print_exc()
