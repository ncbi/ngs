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

from ctypes import byref, c_int, c_uint64
from . import NGS
from .Refcount import Refcount
from .String import NGS_RawString, NGS_String, getNGSString, getNGSValue

# Represents a reference sequence standalone object

class ReferenceSequence(Refcount):

    def getCanonicalName(self):
        '''
        :returns: the accessioned name of reference, e.g. "NC_000001.11"
        '''
        return getNGSString(self, NGS.lib_manager.PY_NGS_ReferenceSequenceGetCanonicalName)
        
    def getIsCircular(self):
        return bool(getNGSValue(self, NGS.lib_manager.PY_NGS_ReferenceSequenceGetIsCircular, c_int))
        
    def getLength(self):
        return getNGSValue(self, NGS.lib_manager.PY_NGS_ReferenceSequenceGetLength, c_uint64)
        
    def getReferenceBases(self, offset, length=-1):
        """
        :param: offset is zero-based and non-negative
        :param: length must be >= 0
        :returns: sub-sequence bases for Reference
        """
        ngs_str_err = NGS_RawString()
        try:
            ngs_str_ret = NGS_String()
            try:
                res = NGS.lib_manager.PY_NGS_ReferenceSequenceGetReferenceBases(self.ref, offset, length, byref(ngs_str_ret.ref), byref(ngs_str_err.ref))
                return ngs_str_ret.getPyString()
            finally:
                ngs_str_ret.close()
        finally:
            ngs_str_err.close()

    def getReferenceChunk(self, offset, length=-1):
        """
        :param: offset is zero-based and non-negative
        :param: length must be >= 0
        :returns: largest contiguous chunk available of sub-sequence bases for Reference
        NB - actual returned sequence may be shorter
        than requested. to obtain all bases available
        in chunk, use a negative "size" value
        """
        ngs_str_err = NGS_RawString()
        try:
            ngs_str_ret = NGS_String()
            try:
                res = NGS.lib_manager.PY_NGS_ReferenceSequenceGetReferenceChunk(self.ref, offset, length, byref(ngs_str_ret.ref), byref(ngs_str_err.ref))
                return ngs_str_ret.getPyString()
            finally:
                ngs_str_ret.close()
        finally:
            ngs_str_err.close()
