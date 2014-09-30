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

#import os

from .LibManager import LibManager # TODO probably, LibManager should not be the part of ngs package and be specific to engine

PY_RES_OK    = 0

class NGS:
    lib_manager = LibManager()

    @staticmethod
    def openReadCollection(spec):
        NGS.lib_manager.initialize_ngs_bindings()
    
        from ReadCollection import openReadCollection  # entry point - adding name to ngs package global namespace
        return openReadCollection(spec)


