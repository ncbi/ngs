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

#ifndef _hpp_ngs_read_
#define _hpp_ngs_read_

#ifndef _hpp_ngs_fragment_iterator_
#include <ngs/FragmentIterator.hpp>
#endif

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards and typedefs
     */
    typedef FragmentRef ReadRef;


    /*======================================================================
     * Read
     *  represents an NGS machine read
     *  having some number of biological Fragments
     */
    class NGS_EXTERN Read : public FragmentIterator
    {
    public:

        /* getReadId
         */
        StringRef getReadId () const
            throw ( ErrorMsg );

        /**
         * getNumFragments
         * the number of biological Fragments contained in the read
         */
        uint32_t getNumFragments () const
            throw ( ErrorMsg );
            
        /*------------------------------------------------------------------
         * read details
         */

        /* ReadCategory
         */
        enum ReadCategory
        {
            fullyAligned     = 1,
            partiallyAligned = 2,
            aligned          = fullyAligned | partiallyAligned,
            unaligned        = 4,
            all              = aligned | unaligned
        };

        /* getReadCategory
         */
        ReadCategory getReadCategory () const
            throw ( ErrorMsg );

        /* getReadGroup
         */
        String getReadGroup () const
            throw ( ErrorMsg );

        /* getReadName
         */
        StringRef getReadName () const
            throw ( ErrorMsg );


        /* getReadBases
         *  return sequence bases
         *  "offset" is zero-based
         */
        StringRef getReadBases () const
            throw ( ErrorMsg );
        StringRef getReadBases ( uint64_t offset ) const
            throw ( ErrorMsg );
        StringRef getReadBases ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );


        /* getReadQualities
         *  return phred quality values
         *  using ASCII offset of 33
         *  "offset" is zero-based
         */
        StringRef getReadQualities () const
            throw ( ErrorMsg );
        StringRef getReadQualities ( uint64_t offset ) const
            throw ( ErrorMsg );
        StringRef getReadQualities ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );

    public:

        // C++ support

        Read ( ReadRef ref )
            throw ();

        Read & operator = ( const Read & obj )
            throw ( ErrorMsg );
        Read ( const Read & obj )
            throw ( ErrorMsg );

        ~ Read ()
            throw ();

    private:

        Read & operator = ( ReadRef ref )
            throw ();
    };

} // namespace ngs


// inlines
#ifndef _inl_ngs_read_
#include <ngs/inl/Read.hpp>
#endif

#endif // _hpp_ngs_read_
