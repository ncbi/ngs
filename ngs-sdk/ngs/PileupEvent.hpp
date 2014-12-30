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
         *  gives position of event on sequence
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
            // event types representable in reference coordinate space
            match                     = 0,
            mismatch                  = 1,
            deletion                  = 2,

            // an insertion cannot be represented in reference coordinate
            // space ( so no insertion event can be directly represented ),
            // but it can occur before a match or mismatch event.
            // insertion is represented as a bit
            insertion                 = 0x10,

            // insertions into the reference
            insertion_before_match    = insertion | match,
            insertion_before_mismatch = insertion | mismatch,
            
            // additional modifier bits - may be added to any event above
            alignment_start           = 0x80,
            alignment_stop            = 0x40,
            alignment_minus_strand    = 0x20,
        };

        /* getEventType
         *  the type of event being represented
         *
         *  a match event indicates that the aligned sequence base
         *  exactly matches the corresponding base in the reference.
         *
         *  a mismatch event indicates that the sequence and
         *  references bases do not match even though they are
         *  considered aligned. The actual sequence base and its
         *  quality value may be retrieved with
         *    "getAlignmentBase()" and "getAlignmentQuality()"
         *
         *  a deletion event indicates a base that is present in
         *  the reference but missing in the sequence.
         *
         *  an insertion cannot be represented in reference coordinate
         *  space ( so no insertion event can be directly represented ),
         *  but it can occur before a match or mismatch event.
         *  insertion is represented as a modifier bit. If this bit
         *  is set, then the event was preceded by an insertion.
         *  The inserted bases and qualities can be retrieved by
         *    "getInsertionBases()" and "getInsertionQualities()"
         *  
         */
        PileupEventType getEventType () const
            throw ( ErrorMsg );

        /* getAlignmentBase
         *  retrieves base aligned at current Reference position
         *  returns '-' for deletion events
         */
        char getAlignmentBase () const
            throw ( ErrorMsg );

        /* getAlignmentQuality
         *  retrieves quality aligned at current Reference position
         *  returns '!' for deletion events
         *  quality is ascii-encoded phred score
         */
        char getAlignmentQuality () const
            throw ( ErrorMsg );

        /* getInsertionBases
         *  returns bases corresponding to insertion event
         *  returns empty string for all non-insertion events
         */
        StringRef getInsertionBases () const
            throw ( ErrorMsg );

        /* getInsertionQualities
         *  returns qualities corresponding to insertion event
         */
        StringRef getInsertionQualities () const
            throw ( ErrorMsg );

        /* getEventRepeatCount
         *  returns the number of times this event repeats
         *  i.e. the distance to the first reference position
         *  yielding a different event for this alignment.
         */
        uint32_t getEventRepeatCount () const
            throw ( ErrorMsg );

        /* EventIndelType
         */
        enum EventIndelType
        {
            normal_indel              = 0,

            // introns behave like deletions
            // (i.e. can retrieve deletion count),
            // "_plus" and "_minus" signify direction
            // of transcription if known
            intron_plus               = 1,
            intron_minus              = 2,
            intron_unknown            = 3,

            // overlap is reported as an insertion,
            // but is actually an overlap in the read
            // inherent in technology like Complete Genomics
            read_overlap              = 4,

            // gap is reported as a deletion,
            // but is actually a gap in the read
            // inherent in technology like Complete Genomics
            read_gap                  = 5
        };

        /* getEventIndelType
         *  returns detail about the type of indel
         *  when event type is an insertion or deletion
         */
        EventIndelType getEventIndelType () const
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
