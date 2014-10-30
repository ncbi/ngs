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

#ifndef _hpp_ngs_itf_alignment_
#define _hpp_ngs_itf_alignment_

#ifndef _hpp_ngs_itf_refcount
#include <ngs/itf/Refcount.hpp>
#endif

struct NGS_Alignment_v1;

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards
     */
    class StringItf;

    /*----------------------------------------------------------------------
     * AlignmentItf
     */
    class   AlignmentItf : public Refcount < AlignmentItf, NGS_Alignment_v1 >
    {
    public:

        StringItf * getAlignmentId () const
            throw ( ErrorMsg );
        StringItf * getReferenceSpec () const
            throw ( ErrorMsg );
        int32_t getMappingQuality () const
            throw ( ErrorMsg );
        StringItf * getReferenceBases () const
            throw ( ErrorMsg );
        StringItf * getReadGroup () const
            throw ( ErrorMsg );
        StringItf * getReadId () const
            throw ( ErrorMsg );
        StringItf * getClippedFragmentBases () const
            throw ( ErrorMsg );
        StringItf * getClippedFragmentQualities () const
            throw ( ErrorMsg );
        StringItf * getAlignedFragmentBases () const
            throw ( ErrorMsg );
        uint32_t getAlignmentCategory () const
            throw ( ErrorMsg );
        int64_t getAlignmentPosition () const
            throw ( ErrorMsg );
        uint64_t getAlignmentLength () const
            throw ( ErrorMsg );
        bool getIsReversedOrientation () const
            throw ( ErrorMsg );
        int32_t getSoftClip ( uint32_t edge ) const
            throw ( ErrorMsg );
        uint64_t getTemplateLength () const
            throw ( ErrorMsg );
        StringItf * getShortCigar ( bool clipped ) const
            throw ( ErrorMsg );
        StringItf * getLongCigar ( bool clipped ) const
            throw ( ErrorMsg );
        bool hasMate () const
            throw ();
        StringItf * getMateAlignmentId () const
            throw ( ErrorMsg );
        AlignmentItf * getMateAlignment () const
            throw ( ErrorMsg );
        StringItf * getMateReferenceSpec () const
            throw ( ErrorMsg );
        bool getMateIsReversedOrientation () const
            throw ( ErrorMsg );
        bool nextAlignment ()
            throw ( ErrorMsg );
    };

} // namespace ngs

#endif // _hpp_ngs_itf_alignment_
