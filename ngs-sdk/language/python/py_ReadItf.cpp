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

#include "py_ReadItf.h"
#include "py_ErrorMsg.hpp"

#include <ngs/itf/ReadItf.hpp>
#include <ngs/itf/FragmentItf.hpp>

/*GEN_PY_FUNC_GET_STRING_CAST             ( Read, FragmentId,        Fragment )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2_CAST ( Read, FragmentBases,     Fragment, uint64_t, offset, uint64_t, length )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2_CAST ( Read, FragmentQualities, Fragment, uint64_t, offset, uint64_t, length )
GEN_PY_ITERATOR_NEXT_EXPLICIT_NAMES     ( Fragment, PY_NGS_ReadNextFragment )*/

GEN_PY_FUNC_GET_STRING                      ( Read, ReadId )
GEN_PY_FUNC_GET                             ( Read, NumFragments, uint32_t )
GEN_PY_FUNC_GET_BY_PARAMS_1_EXPLICIT_NAMES  ( Read, bool, uint32_t, fragIdx, PY_NGS_ReadFragmentIsAligned, fragmentIsAligned)
GEN_PY_FUNC_GET                             ( Read, ReadCategory, uint32_t )
GEN_PY_FUNC_GET_STRING                      ( Read, ReadGroup )
GEN_PY_FUNC_GET_STRING                      ( Read, ReadName )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2          ( Read, ReadBases, uint64_t, offset, uint64_t, length )
GEN_PY_FUNC_GET_STRING_BY_PARAMS_2          ( Read, ReadQualities, uint64_t, offset, uint64_t, length )
