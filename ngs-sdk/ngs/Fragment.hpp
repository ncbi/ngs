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

#ifndef _hpp_ngs_fragment_
#define _hpp_ngs_fragment_

#ifndef _hpp_ngs_error_msg_
#include <ngs/ErrorMsg.hpp>
#endif

#ifndef _hpp_ngs_stringref_
#include <ngs/StringRef.hpp>
#endif

#include <stdint.h>

namespace ngs
{

    /*----------------------------------------------------------------------
     * forwards and typedefs
     */
    typedef class FragmentItf * FragmentRef;


    /*======================================================================
     * Fragment
     *  represents an NGS biological fragment
     */
    class  Fragment
    {
    public:

        /* getFragmentId
         *  returns an unique id within the context of the ReadCollection
         *  representing a single biological fragment
         */
        StringRef getFragmentId () const
            throw ( ErrorMsg );


        /*------------------------------------------------------------------
         * fragment details
         */

        /* getFragmentBases
         *  return sequence bases
         *  "offset" is zero-based
         */
        StringRef getFragmentBases () const
            throw ( ErrorMsg );
        StringRef getFragmentBases ( uint64_t offset ) const
            throw ( ErrorMsg );
        StringRef getFragmentBases ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );


        /* getFragmentQualities
         *  return phred quality values
         *  using ASCII offset of 33
         *  "offset" is zero-based
         */
        StringRef getFragmentQualities () const
            throw ( ErrorMsg );
        StringRef getFragmentQualities ( uint64_t offset ) const
            throw ( ErrorMsg );
        StringRef getFragmentQualities ( uint64_t offset, uint64_t length ) const
            throw ( ErrorMsg );

    public:

        // C++ support

        Fragment ( FragmentRef ref )
            throw ();

        Fragment & operator = ( const Fragment & obj )
            throw ( ErrorMsg );
        Fragment ( const Fragment & obj )
            throw ( ErrorMsg );

        ~ Fragment ()
            throw ();

    private:

        Fragment & operator = ( FragmentRef ref )
            throw ();

    protected:

        FragmentRef self;
    };

} // namespace ngs


// inlines
#ifndef _inl_ngs_fragment_
#include <ngs/inl/Fragment.hpp>
#endif

#endif // _hpp_ngs_fragment_
