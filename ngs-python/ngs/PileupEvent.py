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
# 
# 
from ctypes import c_char, c_int32, c_uint32, c_int64, c_void_p
from . import NGS

from .String import getNGSString, getNGSValue
from .Refcount import Refcount

from .Alignment import Alignment


# Represents a single cell of a sparse 2D matrix with Reference coordinates on one axis
# and stacked Alignments on the other axis

class PileupEvent(Refcount):

    # ----------------------------------------------------------------------
    # Reference
    
    def getReferenceSpec(self):
        return getNGSString(self, NGS.lib_manager.PY_NGS_PileupEventGetReferenceSpec)

    def getReferencePosition(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetReferencePosition, c_int64)

    def getMappingQuality(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetMappingQuality, c_int32)

    # ----------------------------------------------------------------------
    # Alignment        
        
    def getAlignmentId(self):
        return getNGSString(self, NGS.lib_manager.PY_NGS_PileupEventGetAlignmentId)

    def getAlignment(self):
        ret = Alignment()
        ret.ref = getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetAlignment, c_void_p)
        return ret

    def getAlignmentPosition(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetAlignmentPosition, c_int64)

    def getFirstAlignmentPosition(self):
        """
        :returns: the position of this Alignment's first event in Reference coordinates
        """
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetFirstAlignmentPosition, c_int64)

    def getLastAlignmentPosition(self):
        """
        :returns: the position of this Alignment's last event in INCLUSIVE Reference coordinates
        """
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetLastAlignmentPosition, c_int64)

    # ----------------------------------------------------------------------
    # event details        

    match                     = 0
    mismatch                  = 1
    deletion                  = 2
    insertion                 = 0x10
    insertion_before_match    = insertion | match
    insertion_before_mismatch = insertion | mismatch
    alignment_start           = 0x80
    alignment_stop            = 0x40
    alignment_minus_strand    = 0x20

    def getEventType(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetEventType, c_uint32)

    def getAlignmentBase(self):
        """
        :returns: retrieves base aligned at current Reference position
        :throws: ErrorMsg if event is an insertion or deletion
        """
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetAlignmentBase, c_char)

    def getAlignmentQuality(self):
        """
        :returns: retrieves base aligned at current Reference position
        :throws: ErrorMsg if event is an insertion or deletion
        """
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetAlignmentQuality, c_char)

    def getInsertionBases(self):
        """
        :returns: bases corresponding to insertion event
        """
        return getNGSString(self, NGS.lib_manager.PY_NGS_PileupEventGetInsertionBases)

    def getInsertionQualities(self):
        """
        :returns: qualities corresponding to insertion event
        """
        return getNGSString(self, NGS.lib_manager.PY_NGS_PileupEventGetInsertionQualities)

    def getEventRepeatCount(self):
        """
        :returns: the number of times this event repeats, i.e. the distance to the first reference position yielding a different event type for this alignment
        """
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetEventRepeatCount, c_uint32)

    normal_indel              = 0
    intron_plus               = 1
    intron_minus              = 2
    intron_unknown            = 3
    read_overlap              = 4
    read_gap                  = 5

    def getEvenIndeltType(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_PileupEventGetEventIndelType, c_uint32)

