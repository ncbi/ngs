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

#ifndef _hpp_ngs_test_engine_pileupitf_
#define _hpp_ngs_test_engine_pileupitf_

#include <ngs/adapter/ErrorMsg.hpp>
#include <ngs/adapter/StringItf.hpp>
#include <ngs/adapter/PileupItf.hpp>

#include "PileupEventItf.hpp"

namespace ngs_test_engine
{

    /*----------------------------------------------------------------------
     * forwards
     */

    /*----------------------------------------------------------------------
     * PileupItf
     */
    class PileupItf : public ngs_adapt::PileupItf
    {
    public:

        virtual ngs_adapt::StringItf * getReferenceSpec () const
        {
            static std::string spec = "pileupRefSpec";
            return new ngs_adapt::StringItf( spec.c_str(), spec.size() ); 
        }

        virtual int64_t getReferencePosition () const
        {
            return 12345;
        }

        virtual ngs_adapt::PileupEventItf * getPileupEvents () const 
        { 
            return new ngs_test_engine::PileupEventItf(7);
        }

        virtual uint32_t getPileupDepth () const 
        { 
            return 21; 
        }

        virtual bool nextPileup ()  
        {
            switch ( iterateFor )
            {
            case -1:    throw ngs_adapt::ErrorMsg ( "invalid iterator access" );
            case 0:     return false;
            default:    --iterateFor; return true;
            }
        }

	public:
		PileupItf ( unsigned int p_iterateFor ) 
        : iterateFor( p_iterateFor )
        { 
            ++instanceCount;
        }
		
        ~PileupItf () 
        { 
            --instanceCount;
        }

        static NGS_EXTERN  unsigned int instanceCount;

        unsigned int iterateFor;
    };

} // namespace ngs_test_engine

#endif // _hpp_ngs_test_engine_pileupitf_
