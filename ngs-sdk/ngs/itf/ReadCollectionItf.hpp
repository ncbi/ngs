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

#ifndef _hpp_ngs_itf_read_collectionitf_
#define _hpp_ngs_itf_read_collectionitf_

#ifndef _hpp_ngs_itf_refcount_
#include <ngs/itf/Refcount.hpp>
#endif

struct NGS_ReadCollection_v1;

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards
     */
    class ReadItf;
    class StringItf;
    class ReadGroupItf;
    class ReferenceItf;
    class AlignmentItf;

    /*----------------------------------------------------------------------
     * ReadCollectionItf
     */
    class   ReadCollectionItf : public Refcount < ReadCollectionItf, NGS_ReadCollection_v1 >
    {
    public:
        
        StringItf * getName () const
            throw ( ErrorMsg );
        ReadGroupItf * getReadGroups () const
            throw ( ErrorMsg );
        bool hasReadGroup ( const char * spec ) const
            throw ();
        ReadGroupItf * getReadGroup ( const char * spec ) const
            throw ( ErrorMsg );
        ReferenceItf * getReferences () const
            throw ( ErrorMsg );
        bool hasReference ( const char * spec ) const
            throw ();
        ReferenceItf * getReference ( const char * spec ) const
            throw ( ErrorMsg );
        AlignmentItf * getAlignment ( const char * alignmentId ) const
            throw ( ErrorMsg );
        AlignmentItf * getAlignments ( uint32_t categories ) const
            throw ( ErrorMsg );
        uint64_t getAlignmentCount ( uint32_t categories ) const
            throw ( ErrorMsg );
        AlignmentItf * getAlignmentRange ( uint64_t first, uint64_t count, uint32_t categories ) const
            throw ( ErrorMsg );
        ReadItf * getRead ( const char * readId ) const
            throw ( ErrorMsg );
        ReadItf * getReads ( uint32_t categories ) const
            throw ( ErrorMsg );
        uint64_t getReadCount ( uint32_t categories ) const
            throw ( ErrorMsg );
        ReadItf * getReadRange ( uint64_t first, uint64_t count ) const
            throw ( ErrorMsg );
        ReadItf * getReadRange ( uint64_t first, uint64_t count, uint32_t categories ) const
            throw ( ErrorMsg );
    };

} // namespace ngs

#endif // _hpp_ngs_itf_read_collectionitf_
