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

#include <ngs/itf/ReferenceItf.hpp>
#include <ngs/itf/PileupItf.hpp>
#include <ngs/itf/AlignmentItf.hpp>
#include <ngs/itf/StringItf.hpp>
#include <ngs/itf/ErrBlock.hpp>
#include <ngs/itf/VTable.hpp>

#include <ngs/itf/ReferenceItf.h>

#include <ngs/Alignment.hpp>

namespace ngs
{
    /*----------------------------------------------------------------------
     * metadata
     */
    extern ItfTok NGS_Refcount_v1_tok;
    ItfTok NGS_Reference_v1_tok ( "NGS_Reference_v1", NGS_Refcount_v1_tok );


    /*----------------------------------------------------------------------
     * access vtable
     */
    static inline
    const NGS_Reference_v1_vt * Access ( const NGS_VTable * vt )
    {
        const NGS_Reference_v1_vt * out = static_cast < const NGS_Reference_v1_vt* >
            ( Cast ( vt, NGS_Reference_v1_tok ) );
        if ( out == 0 )
            throw ErrorMsg ( "object is not of type NGS_Reference_v1" );
        return out;
    }


    /*----------------------------------------------------------------------
     * ReferenceItf
     */

    StringItf * ReferenceItf :: getCommonName () const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_cmn_name != 0 );
        NGS_String_v1 * ret  = ( * vt -> get_cmn_name ) ( self, & err );

        // check for errors
        err . Check ();

        return StringItf :: Cast ( ret );
    }

    StringItf * ReferenceItf :: getCanonicalName () const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_canon_name != 0 );
        NGS_String_v1 * ret  = ( * vt -> get_canon_name ) ( self, & err );

        // check for errors
        err . Check ();

        return StringItf :: Cast ( ret );
    }

    bool ReferenceItf :: getIsCircular () const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> is_circular != 0 );
        bool ret  = ( * vt -> is_circular ) ( self, & err );

        // check for errors
        err . Check ();

        return ret;
    }

    uint64_t ReferenceItf :: getLength () const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_length != 0 );
        uint64_t ret  = ( * vt -> get_length ) ( self, & err );

        // check for errors
        err . Check ();

        return ret;
    }

    StringItf * ReferenceItf :: getReferenceBases ( uint64_t offset ) const
        throw ( ErrorMsg )
    {
        return this -> getReferenceBases ( offset, -1 );
    }

    StringItf * ReferenceItf :: getReferenceBases ( uint64_t offset, uint64_t length ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_ref_bases != 0 );
        NGS_String_v1 * ret  = ( * vt -> get_ref_bases ) ( self, & err, offset, length );

        // check for errors
        err . Check ();

        return StringItf :: Cast ( ret );
    }

    StringItf * ReferenceItf :: getReferenceChunk ( uint64_t offset ) const
        throw ( ErrorMsg )
    {
        return this -> getReferenceChunk ( offset, -1 );
    }

    StringItf * ReferenceItf :: getReferenceChunk ( uint64_t offset, uint64_t length ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_ref_chunk != 0 );
        NGS_String_v1 * ret  = ( * vt -> get_ref_chunk ) ( self, & err, offset, length );

        // check for errors
        err . Check ();

        return StringItf :: Cast ( ret );
    }

    AlignmentItf * ReferenceItf :: getAlignment ( const char * alignmentId ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_alignment != 0 );
        NGS_Alignment_v1 * ret  = ( * vt -> get_alignment ) ( self, & err, alignmentId );

        // check for errors
        err . Check ();

        return AlignmentItf :: Cast ( ret );
    }

    AlignmentItf * ReferenceItf :: getAlignments ( uint32_t categories ) const
    throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_alignments != 0 );
        bool wants_primary      = ( categories & Alignment :: primaryAlignment ) != 0;
        bool wants_secondary    = ( categories & Alignment :: secondaryAlignment ) != 0;
        NGS_Alignment_v1 * ret  = ( * vt -> get_alignments ) ( self, & err, wants_primary, wants_secondary );

        // check for errors
        err . Check ();

        return AlignmentItf :: Cast ( ret );
    }

    AlignmentItf * ReferenceItf :: getAlignmentSlice ( int64_t start, uint64_t length ) const
        throw ( ErrorMsg )
    {
        return this -> getAlignmentSlice ( start, length, Alignment :: all );
    }

    AlignmentItf * ReferenceItf :: getAlignmentSlice ( int64_t start, uint64_t length, uint32_t categories ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_align_slice != 0 );
        bool wants_primary      = ( categories & Alignment :: primaryAlignment ) != 0;
        bool wants_secondary    = ( categories & Alignment :: secondaryAlignment ) != 0;
        NGS_Alignment_v1 * ret  = ( * vt -> get_align_slice ) ( self, & err, start, length, wants_primary, wants_secondary );

        // check for errors
        err . Check ();

        return AlignmentItf :: Cast ( ret );
    }

    PileupItf * ReferenceItf :: getPileups ( uint32_t categories ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_pileups != 0 );
        bool wants_primary      = ( categories & Alignment :: primaryAlignment ) != 0;
        bool wants_secondary    = ( categories & Alignment :: secondaryAlignment ) != 0;
        NGS_Pileup_v1 * ret  = ( * vt -> get_pileups ) ( self, & err, wants_primary, wants_secondary );

        // check for errors
        err . Check ();

        return PileupItf :: Cast ( ret );
    }

    PileupItf * ReferenceItf :: getPileupSlice ( int64_t start, uint64_t length ) const
        throw ( ErrorMsg )
    {
        return this -> getPileupSlice ( start, length, Alignment :: all );
    }

    PileupItf * ReferenceItf :: getPileupSlice ( int64_t start, uint64_t length, uint32_t categories ) const
        throw ( ErrorMsg )
    {
        // the object is really from C
        const NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> get_pileup_slice != 0 );
        bool wants_primary      = ( categories & Alignment :: primaryAlignment ) != 0;
        bool wants_secondary    = ( categories & Alignment :: secondaryAlignment ) != 0;
        NGS_Pileup_v1 * ret  = ( * vt -> get_pileup_slice ) ( self, & err, start, length, wants_primary, wants_secondary );

        // check for errors
        err . Check ();

        return PileupItf :: Cast ( ret );
    }
    
    bool ReferenceItf :: nextReference ()
        throw ( ErrorMsg )
    {
        // the object is really from C
        NGS_Reference_v1 * self = Test ();

        // cast vtable to our level
        const NGS_Reference_v1_vt * vt = Access ( self -> vt );

        // call through C vtable
        ErrBlock err;
        assert ( vt -> next != 0 );
        bool ret  = ( * vt -> next ) ( self, & err );

        // check for errors
        err . Check ();

        return ret;
    }
}

