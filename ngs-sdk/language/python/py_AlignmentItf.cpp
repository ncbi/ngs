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

#include "py_AlignmentItf.h"
#include "py_ErrorMsg.hpp"

#include <ngs/itf/AlignmentItf.hpp>
#include <ngs/itf/FragmentItf.hpp>

/*
GEN_PY_FUNC_GET_STRING_CAST             ( Alignment, FragmentId,        Fragment )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2_CAST ( Alignment, FragmentBases,     Fragment, uint64_t, offset, uint64_t, length )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2_CAST ( Alignment, FragmentQualities, Fragment, uint64_t, offset, uint64_t, length )
*/

GEN_PY_FUNC_GET_STRING                  ( Alignment, AlignmentId )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ReferenceSpec )
GEN_PY_FUNC_GET                         ( Alignment, MappingQuality, int32_t )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ReferenceBases )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ReadGroup )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ReadId )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ClippedFragmentBases )
GEN_PY_FUNC_GET_STRING                  ( Alignment, ClippedFragmentQualities )
GEN_PY_FUNC_GET_STRING                  ( Alignment, AlignedFragmentBases )
GEN_PY_FUNC_GET                         ( Alignment, AlignmentCategory, uint32_t )
GEN_PY_FUNC_GET                         ( Alignment, AlignmentPosition, int64_t )
GEN_PY_FUNC_GET                         ( Alignment, AlignmentLength, uint64_t )
GEN_PY_FUNC_GET                         ( Alignment, IsReversedOrientation, bool )
GEN_PY_FUNC_GET_BY_PARAMS_1             ( Alignment, SoftClip, int32_t, uint32_t, edge )
GEN_PY_FUNC_GET                         ( Alignment, TemplateLength, uint64_t )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_1      ( Alignment, ShortCigar, bool, clipped )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_1      ( Alignment, LongCigar, bool, clipped )
GEN_PY_FUNC_GET                         ( Alignment, RNAOrientation, char )
//GEN_PY_FUNC_GET                         ( Alignment, HasMate, bool ) // TODO: decide what to do with non-standard names
GEN_PY_FUNC_GET_EXPLICIT_NAMES          ( Alignment, bool, PY_NGS_AlignmentHasMate, hasMate )
GEN_PY_FUNC_GET_STRING                  ( Alignment, MateAlignmentId )
GEN_PY_FUNC_GET                         ( Alignment, MateAlignment, ngs::AlignmentItf* )
GEN_PY_FUNC_GET_STRING                  ( Alignment, MateReferenceSpec )
GEN_PY_FUNC_GET                         ( Alignment, MateIsReversedOrientation, bool )


