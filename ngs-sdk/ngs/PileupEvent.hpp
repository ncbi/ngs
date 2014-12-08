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

#ifndef _hpp_ngs_pileup_event_
#define _hpp_ngs_pileup_event_

#ifndef _hpp_ngs_alignment_
#include <ngs/Alignment.hpp>
#endif

#include <stdint.h>

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards and typedefs
     */
    typedef class PileupEventItf * PileupEventRef;
    

    /*======================================================================
     * PileupEvent
     *  represents a single cell of a sparse 2D matrix
     *  with Reference coordinates on one axis
     *  and stacked Alignments on the other axis
     */
    class PileupEvent
    {
    public:

        /*------------------------------------------------------------------
         * Reference
         */

        /* getReferenceSpec
         */
        String getReferenceSpec () const
            throw ( ErrorMsg );

        /* getReferencePosition
         */
        int64_t getReferencePosition () const
            throw ( ErrorMsg );

        /* getMappingQuality
         */
        int getMappingQuality () const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * Alignment
         */

        /* getAlignmentId
         *  unique within ReadCollection
         */
        StringRef getAlignmentId () const
            throw ( ErrorMsg );

        /* getAlignment
         */
        Alignment getAlignment () const
            throw ( ErrorMsg );

        /* getAlignmentPosition
         */
        int64_t getAlignmentPosition () const
            throw ( ErrorMsg );

        /* getFirstAlignmentPosition
         *  returns the position of this Alignment's first event
         *  in Reference coordinates
         */
        int64_t getFirstAlignmentPosition () const
            throw ( ErrorMsg );

        /* getLastAlignmentPosition
         *  returns the position of this Alignment's last event
         *  in INCLUSIVE Reference coordinates
         */
        int64_t getLastAlignmentPosition () const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * event details
         */
         
        /* EventType
         */
        enum PileupEventType
        {
            // no change to coordinate mapping
            match                     = 0,
            mismatch                  = 1,

            // insertions into the reference
            insertion_before_match    = 2,
            insertion_before_mismatch = 3,

            // overlap behaves like insertion
            // (i.e. can retrieve insertion bases),
            // but is actually an overlap in the read
            // inherent in technology like Complete Genomics
            read_overlap              = 4,

            // deletions from the reference
            deletion                  = 5,

            // introns behave like deletions
            // (i.e. can retrieve deletion count),
            // "_plus" and "_minus" signify direction
            // of transcription if known
            intron_plus               = 6,
            intron_minus              = 7,
            intron_unknown            = 8,

            // gap behaves like a deletion
            // (i.e. can retrieve deletion count),
            // but is actuall a gap in the read
            // inherent in technology like Complete Genomics
            read_gap                  = 9,
            
            
            alignment_start           = 0x80,
            alignment_stop            = 0x40,
            alignment_minus_strand    = 0x20,
        };

        /* getEventType
         *  the type of event being represented
         */
        PileupEventType getEventType () const
            throw ( ErrorMsg );

        /* getAlignmentBase
         *  retrieves base aligned at current Reference position
         *  throws exception if event is an insertion or deletion
         */
        char getAlignmentBase () const
            throw ( ErrorMsg );

        /* getAlignmentQuality
         *  retrieves base aligned at current Reference position
         *  throws exception if event is an insertion or deletion
         */
        char getAlignmentQuality () const
            throw ( ErrorMsg );

        /* getInsertionBases
         *  returns bases corresponding to insertion event
         */
        StringRef getInsertionBases () const
            throw ( ErrorMsg );

        /* getInsertionQualities
         *  returns qualities corresponding to insertion event
         */
        StringRef getInsertionQualities () const
            throw ( ErrorMsg );

        /* getDeletionCount
         *  returns the number of bases remaining in deletion event
         *  i.e. the number of Reference base positions remaining
         *  until the next non-deletion event in this alignment.
         */
        uint32_t getDeletionCount () const
            throw ( ErrorMsg );

    public:

        // C++ support

        PileupEvent & operator = ( PileupEventRef ref )
            throw ();
        PileupEvent ( PileupEventRef ref )
            throw ();

        PileupEvent & operator = ( const PileupEvent & obj )
            throw ( ErrorMsg );
        PileupEvent ( const PileupEvent & obj )
            throw ( ErrorMsg );

        ~ PileupEvent ()
            throw ();

    protected:

        PileupEventRef self;
    };

} // namespace ngs


#ifndef _inl_ngs_pileup_event_
#include <ngs/inl/PileupEvent.hpp>
#endif

#endif // _hpp_ngs_pileup_event_
