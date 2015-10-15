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

#ifndef _hpp_ngs_read_collection_
#define _hpp_ngs_read_collection_

#ifndef _hpp_ngs_read_group_iterator_
#include <ngs/ReadGroupIterator.hpp>
#endif

#ifndef _hpp_ngs_reference_iterator_
#include <ngs/ReferenceIterator.hpp>
#endif

#ifndef _hpp_ngs_alignment_iterator_
#include <ngs/AlignmentIterator.hpp>
#endif

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards and typedefs
     */
    typedef class ReadCollectionItf * ReadCollectionRef;


    /*======================================================================
     * ReadCollection
     *  represents an NGS-capable object with a collection of Reads
     */
    class ReadCollection
    {
    public:
        
        /* getName
         *  returns the simple name of the read collection
         *  this name is generally extracted from the "spec"
         *  used to create the object, but may also be mapped
         *  to a canonical name if one may be determined and
         *  differs from that given in the spec.
         *
         *  if the name is extracted from "spec" and contains
         *  well-known file extensions that do not form part of
         *  a canonical name (e.g. ".sra"), they will be removed.
         */
        String getName () const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * READ GROUPS
         */

        /* getReadGroups
         *  returns an iterator of all ReadGroups used
         */
        ReadGroupIterator getReadGroups () const
            throw ( ErrorMsg );

        /* hasReadGroup
         *  returns true if a call to "getReadGroup()" should succeed
         */
        bool hasReadGroup ( const String & spec ) const
            throw ();

        /* getReadGroup
         */
        ReadGroup getReadGroup ( const String & spec ) const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * REFERENCES
         */

        /* getReferences
         *  returns an iterator of all References used
         *  iterator will be empty if no Reads are aligned
         */
        ReferenceIterator getReferences () const
            throw ( ErrorMsg );

        /* hasReference
         *  returns true if a call to "getReference()" should succeed
         */
        bool hasReference ( const String & spec ) const
            throw ();

        /* getReference
         */
        Reference getReference ( const String & spec ) const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * ALIGNMENTS
         */

        /* getAlignment
         *  returns an individual Alignment
         *  throws ErrorMsg if Alignment does not exist
         */
        Alignment getAlignment ( const String & alignmentId ) const
            throw ( ErrorMsg );

        /* getAlignments
         *  returns an iterator of all Alignments from specified categories
         */
        AlignmentIterator getAlignments ( Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );

        /* getAlignmentCount
         *  returns count of all alignments
         *  "categories" provides a means of filtering by AlignmentCategory
         */
        uint64_t getAlignmentCount () const
            throw ( ErrorMsg );
        uint64_t getAlignmentCount ( Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );

        /* getAlignmentRange
         *  returns an iterator across a range of Alignments
         *  "first" is an ordinal into set
         *  "categories" provides a means of filtering by AlignmentCategory
         */
        AlignmentIterator getAlignmentRange ( uint64_t first, uint64_t count ) const
            throw ( ErrorMsg );
        AlignmentIterator getAlignmentRange ( uint64_t first, uint64_t count, Alignment :: AlignmentCategory categories ) const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * READS
         */

        /* getRead
         *  returns an individual Read
         *  throws ErrorMsg if Read does not exist
         */
        Read getRead ( const String & readId ) const
            throw ( ErrorMsg );

        /* getReads
         *  returns an iterator of all contained machine Reads
         *  "categories" provides a means of filtering by ReadCategory
         */
        ReadIterator getReads ( Read :: ReadCategory categories ) const
            throw ( ErrorMsg );

        /* getReadCount
         *  returns the number of reads in the collection
         *  "categories" provides an optional means of filtering by ReadCategory
         */
        uint64_t getReadCount () const
            throw ( ErrorMsg );
        uint64_t getReadCount ( Read :: ReadCategory categories ) const
            throw ( ErrorMsg );

        /* getReadRange
         *  returns an iterator across a range of Reads
         *  "first" is an ordinal into set
         *  "categories" provides an optional means of filtering by ReadCategory
         */
        ReadIterator getReadRange ( uint64_t first, uint64_t count ) const
            throw ( ErrorMsg );
        ReadIterator getReadRange ( uint64_t first, uint64_t count, Read :: ReadCategory categories ) const
            throw ( ErrorMsg );

    public:

        // C++ support

        ReadCollection & operator = ( ReadCollectionRef ref )
            throw ();
        ReadCollection ( ReadCollectionRef ref )
            throw ();

        ReadCollection & operator = ( const ReadCollection & obj )
            throw ();
        ReadCollection ( const ReadCollection & obj )
            throw ();

        ~ ReadCollection ()
            throw ();

    protected:

        ReadCollectionRef self;
    };

} // namespace ngs


#ifndef _inl_ngs_read_collection_
#include <ngs/inl/ReadCollection.hpp>
#endif

#endif // _hpp_ngs_read_collection_
