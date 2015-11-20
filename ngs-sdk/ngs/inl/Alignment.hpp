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

#ifndef _inl_ngs_alignment_
#define _inl_ngs_alignment_

#ifndef _hpp_ngs_alignment_
#include <ngs/Alignment.hpp>
#endif

#ifndef _hpp_ngs_itf_alignmentitf_
#include <ngs/itf/AlignmentItf.hpp>
#endif

namespace ngs
{

    // the "self" member is typed as FragmentRef
    // but is used here as an AlignmentRef
#define self reinterpret_cast < const AlignmentItf * > ( self )

    /*----------------------------------------------------------------------
     * Alignment
     *  inline dispatch
     */

    inline
    StringRef Alignment :: getAlignmentId () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getAlignmentId () ); }

    inline
    String Alignment :: getReferenceSpec () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getReferenceSpec () ) . toString (); }

    inline
    int Alignment :: getMappingQuality () const
        throw ( ErrorMsg )
    { return self -> getMappingQuality (); }

    inline
    StringRef Alignment :: getReferenceBases () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getReferenceBases () ); }

    inline
    String Alignment :: getReadGroup () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getReadGroup () ) . toString (); }

    inline
    StringRef Alignment :: getReadId () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getReadId () ); }

    inline
    StringRef Alignment :: getClippedFragmentBases () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getClippedFragmentBases () ); }

    inline
    StringRef Alignment :: getClippedFragmentQualities () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getClippedFragmentQualities () ); }

    inline
    StringRef Alignment :: getAlignedFragmentBases () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getAlignedFragmentBases () ); }

    inline
    Alignment :: AlignmentCategory Alignment :: getAlignmentCategory () const
        throw ( ErrorMsg )
    { return ( Alignment :: AlignmentCategory ) self -> getAlignmentCategory (); }

    inline
    int64_t Alignment :: getAlignmentPosition () const
        throw ( ErrorMsg )
    { return self -> getAlignmentPosition (); }

    inline
    uint64_t Alignment :: getReferencePositionProjectionRange (int64_t ref_pos) const
        throw ( ErrorMsg )
    { return self -> getReferencePositionProjectionRange (ref_pos); }

    inline
    uint64_t Alignment :: getAlignmentLength () const
        throw ( ErrorMsg )
    { return self -> getAlignmentLength (); }

    inline
    bool Alignment :: getIsReversedOrientation () const
        throw ( ErrorMsg )
    { return self -> getIsReversedOrientation (); }

    inline
    int Alignment :: getSoftClip ( ClipEdge edge ) const
        throw ( ErrorMsg )
    { return self -> getSoftClip ( edge ); }

    inline
    uint64_t Alignment :: getTemplateLength () const
        throw ( ErrorMsg )
    { return self -> getTemplateLength (); }

    inline
    StringRef Alignment :: getShortCigar ( bool clipped ) const
        throw ( ErrorMsg )
    { return StringRef ( self -> getShortCigar ( clipped ) ); }

    inline
    StringRef Alignment :: getLongCigar ( bool clipped ) const
        throw ( ErrorMsg )
    { return StringRef ( self -> getLongCigar ( clipped ) ); }

    inline
    char Alignment :: getRNAOrientation () const
        throw ( ErrorMsg )
    { return self -> getRNAOrientation (); }
    
    inline
    bool Alignment :: hasMate () const
        throw ()
    { return self -> hasMate (); }

    inline
    StringRef Alignment :: getMateAlignmentId () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getMateAlignmentId () ); }

    inline
    Alignment Alignment :: getMateAlignment () const
        throw ( ErrorMsg )
    { return Alignment ( ( AlignmentRef ) self -> getMateAlignment () ); }

    inline
    String Alignment :: getMateReferenceSpec () const
        throw ( ErrorMsg )
    { return StringRef ( self -> getMateReferenceSpec () ) . toString (); }

    inline
    bool Alignment :: getMateIsReversedOrientation () const
        throw ( ErrorMsg )
    { return self -> getMateIsReversedOrientation (); }

#undef self

} // namespace ngs

#endif // _inl_ngs_alignment_
