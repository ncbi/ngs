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

#ifndef _hpp_ngs_reference_
#define _hpp_ngs_reference_

#ifndef _hpp_ngs_alignment_iterator_
#include <ngs/AlignmentIterator.hpp>
#endif

#ifndef _hpp_ngs_pileup_iterator_
#include <ngs/PileupIterator.hpp>
#endif

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards and typedefs
     */
    typedef class ReferenceItf * ReferenceRef;


    /*======================================================================
     * Reference
     *  represents a reference sequence
     */
    class Reference
    {
    public:

        /* getCommonName
         *  returns the common name of reference, e.g. "chr1"
         */
        String getCommonName () const
            throw ( ErrorMsg );

        /* getCanonicalName
         *  returns the accessioned name of reference, e.g. "NC_000001.11"
         */
        String getCanonicalName () const
            throw ( ErrorMsg );


        /* getIsCircular
         *  returns true if reference is circular
         */
        bool getIsCircular () const
            throw ( ErrorMsg );


        /* getLength
         *  returns the length of the reference sequence
         */
        uint64_t getLength () const
            throw ( ErrorMsg );


        /* getReferenceBases
         *  return sub-sequence bases for Reference
         *  "offset" is zero-based
         */
        String getReferenceBases ( uint64_t offset ) const
            throw ( ErrorMsg );
        String getReferenceBases ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );

        /* getReferenceChunk
         *  return largest contiguous chunk available of
         *  sub-sequence bases for Reference
         *  "offset" is zero-based
         *
         * NB - actual returned sequence may be shorter
         *  than requested.
         */
        StringRef getReferenceChunk ( uint64_t offset ) const
            throw ( ErrorMsg );
        StringRef getReferenceChunk ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * ALIGNMENTS
         */

        /* getAlignment
         *  returns an individual Alignment
         *  throws ErrorMsg if Alignment does not exist
         *  or is not part of this Reference
         */
        Alignment getAlignment ( const String & alignmentId ) const
            throw ( ErrorMsg );

        /* getAlignments
         *  returns an iterator of contained alignments
         */
        AlignmentIterator getAlignments ( Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );

        /* getAlignmentSlice
         *  returns an iterator across a slice of the Reference
         *
         *  "start" is a 0-based signed offset from the start of the Reference
         *
         *  "length" is the length of the slice.
         *   will be truncated to the right edge of a non-circular Reference
         *   or to the total length of a circular Reference.
         *
         *  "categories" provides a means of filtering by AlignmentCategory
         */
        AlignmentIterator getAlignmentSlice ( int64_t start, uint64_t length ) const
            throw ( ErrorMsg );
        AlignmentIterator getAlignmentSlice ( int64_t start, uint64_t length, Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * PILEUP
         */

        /* getPileups
         *  returns an iterator of contained Pileups
         *  Alignments are filtered by rejecting failures and duplicates
         *  No mapping qualities are taken into account.
         */
        PileupIterator getPileups ( Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );

        /* getFilteredPileups
         *  returns an iterator of contained Pileups
         *  filtered according to criteria in parameters
         *  "filters" is a set of filter bits defined in Alignment
         *  "mappingQuality" is a cutoff to be used according to bits in "filter"
         */
        PileupIterator getFilteredPileups ( Alignment :: AlignmentCategory categories,
                Alignment :: AlignmentFilter filters, int32_t mappingQuality ) const
            throw ( ErrorMsg );

        /* getPileupSlice
         *  creates a PileupIterator on a slice (window) of reference
         *  "start" is the signed starting position on reference
         *  "length" is the number of bases in the window
         *  "categories" provides a means of filtering by AlignmentCategory
         */
        PileupIterator getPileupSlice ( int64_t start, uint64_t length ) const
            throw ( ErrorMsg );
        PileupIterator getPileupSlice ( int64_t start, uint64_t length, Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );

        /* getFilteredPileupSlice
         *  creates a PileupIterator on a slice (window) of reference
         *  filtered according to criteria in parameters
         *  "filters" is a set of filter bits defined in Alignment
         *  "mappingQuality" is a cutoff to be used according to bits in "filter"
         */
        PileupIterator getFilteredPileupSlice ( int64_t start, uint64_t length, Alignment :: AlignmentCategory categories,
                Alignment :: AlignmentFilter filters, int32_t mappingQuality ) const
            throw ( ErrorMsg );

    public:

        // C++ support

        Reference & operator = ( ReferenceRef ref )
            throw ();
        Reference ( ReferenceRef ref )
            throw ();

        Reference & operator = ( const Reference & obj )
            throw ( ErrorMsg );
        Reference ( const Reference & obj )
            throw ( ErrorMsg );

        ~ Reference ()
            throw ();

    protected:

        ReferenceRef self;
    };

} // namespace ngs


#ifndef _inl_ngs_reference_
#include <ngs/inl/Reference.hpp>
#endif

#endif // _hpp_ngs_reference_
