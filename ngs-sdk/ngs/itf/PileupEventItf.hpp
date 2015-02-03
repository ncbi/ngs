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

#ifndef _hpp_ngs_itf_pileup_eventitf_
#define _hpp_ngs_itf_pileup_eventitf_

#ifndef _hpp_ngs_itf_refcount_
#include <ngs/itf/Refcount.hpp>
#endif

struct NGS_PileupEvent_v1;

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards
     */
    class StringItf;
    class AlignmentItf;

    /*----------------------------------------------------------------------
     * PileupEventItf
     */
    class   PileupEventItf : public Refcount < PileupEventItf, NGS_PileupEvent_v1 >
    {
    public:

        int32_t getMappingQuality () const
            throw ( ErrorMsg );
        StringItf * getAlignmentId () const
            throw ( ErrorMsg );
        AlignmentItf * getAlignment () const
            throw ( ErrorMsg );
        int64_t getAlignmentPosition () const
            throw ( ErrorMsg );
        int64_t getFirstAlignmentPosition () const
            throw ( ErrorMsg );
        int64_t getLastAlignmentPosition () const
            throw ( ErrorMsg );
        uint32_t getEventType () const
            throw ( ErrorMsg );
        char getAlignmentBase () const
            throw ( ErrorMsg );
        char getAlignmentQuality () const
            throw ( ErrorMsg );
        StringItf * getInsertionBases () const
            throw ( ErrorMsg );
        StringItf * getInsertionQualities () const
            throw ( ErrorMsg );
        uint32_t getEventRepeatCount () const
            throw ( ErrorMsg );
        uint32_t getEventIndelType () const
            throw ( ErrorMsg );
        bool nextPileupEvent ()
            throw ( ErrorMsg );
        void resetPileupEvent ()
            throw ( ErrorMsg );

    };

} // namespace ngs

#endif // _hpp_ngs_itf_pileup_eventitf_
