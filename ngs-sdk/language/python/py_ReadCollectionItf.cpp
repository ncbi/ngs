/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*/

#include "py_ReadCollectionItf.h"
#include "py_ErrorMsg.hpp"

#include <ngs/itf/ReadCollectionItf.hpp>

GEN_PY_FUNC_GET_STRING                  ( ReadCollection, Name )
GEN_PY_FUNC_GET                         ( ReadCollection, ReadGroups,     ngs::ReadGroupItf* )
GEN_PY_FUNC_GET_EXPLICIT_NAMES_1        ( ReadCollection, bool, PY_NGS_ReadCollectionHasReadGroup, hasReadGroup, char const*, spec )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, ReadGroup,      ngs::ReadGroupItf*, char const*, spec )
GEN_PY_FUNC_GET                         ( ReadCollection, References,     ngs::ReferenceItf* )
GEN_PY_FUNC_GET_EXPLICIT_NAMES_1        ( ReadCollection, bool, PY_NGS_ReadCollectionHasReference, hasReference, char const*, spec )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, Reference,      ngs::ReferenceItf*, char const*, spec )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, Alignment,      ngs::AlignmentItf*, char const*, alignmentId )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, Alignments,     ngs::AlignmentItf*, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, AlignmentCount, uint64_t, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_3             ( ReadCollection, AlignmentRange, ngs::AlignmentItf*, uint64_t, first, uint64_t, count, uint32_t, categories  )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, Read,           ngs::ReadItf*, char const*, readId )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, Reads,          ngs::ReadItf*, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( ReadCollection, ReadCount,      uint64_t, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_3             ( ReadCollection, ReadRange,      ngs::ReadItf*, uint64_t, first, uint64_t, count, uint32_t, categories )
