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

#include "py_ReferenceItf.h"
#include "py_ErrorMsg.hpp"

#include <ngs/itf/ReferenceItf.hpp>

GEN_PY_FUNC_GET_STRING             ( Reference, CommonName )
GEN_PY_FUNC_GET_STRING             ( Reference, CanonicalName )
GEN_PY_FUNC_GET                    ( Reference, IsCircular,          bool )
GEN_PY_FUNC_GET                    ( Reference, Length,              uint64_t )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2 ( Reference, ReferenceBases,      uint64_t, offset, uint64_t, length )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2 ( Reference, ReferenceChunk,      uint64_t, offset, uint64_t, length )
GEN_PY_FUNC_GET_BY_PARAMS_1        ( Reference, Alignment,           ngs::AlignmentItf*, char const*, alignmentId )
GEN_PY_FUNC_GET_BY_PARAMS_1        ( Reference, Alignments,          ngs::AlignmentItf*, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_3        ( Reference, AlignmentSlice,      ngs::AlignmentItf*, int64_t, start, uint64_t, length, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_1        ( Reference, Pileups,             ngs::PileupItf*, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_3        ( Reference, FilteredPileups,     ngs::PileupItf*, uint32_t, categories, uint32_t, filters, int32_t, map_qual )
GEN_PY_FUNC_GET_BY_PARAMS_3        ( Reference, PileupSlice,         ngs::PileupItf*, int64_t, start, uint64_t, length, uint32_t, categories )
GEN_PY_FUNC_GET_BY_PARAMS_5        ( Reference, FilteredPileupSlice, ngs::PileupItf*, int64_t, start, uint64_t, length, uint32_t, categories, uint32_t, filters, int32_t, map_qual )
