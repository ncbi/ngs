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

#include "jni_PileupItf.h"
#include "jni_ErrorMsg.hpp"
#include "jni_String.hpp"

#include <ngs/itf/PileupItf.hpp>
#include <ngs/itf/PileupEventItf.hpp>
#include <ngs/itf/StringItf.hpp>

using namespace ngs;

static
PileupItf * Self ( size_t jself )
{
    if ( jself == 0 )
        throw ErrorMsg ( "NULL self parameter" );

    return ( PileupItf* ) jself;
}

static
jlong Cast ( PileupEventItf * obj )
{
    return ( jlong ) ( size_t ) obj;
}

/*
 * Class:     ngs_itf_PileupItf
 * Method:    GetReferenceSpec
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ngs_itf_PileupItf_GetReferenceSpec
    ( JNIEnv * jenv, jobject jthis, jlong jself )
{
    try
    {
        StringItf * new_ref = Self ( jself ) -> getReferenceSpec ();
        return StringItfConvertToJString ( new_ref, jenv );
    }
    catch ( ErrorMsg & x )
    {
        ErrorMsgThrow ( jenv, xt_error_msg, x . what () );
    }
    catch ( std :: exception & x )
    {
        ErrorMsgThrow ( jenv, xt_runtime, x . what () );
    }
    catch ( ... )
    {
        JNI_INTERNAL_ERROR ( jenv, "%s", __func__ );
    }

    return 0;
}

/*
 * Class:     ngs_itf_PileupItf
 * Method:    GetReferencePosition
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_ngs_itf_PileupItf_GetReferencePosition
    ( JNIEnv * jenv, jobject jthis, jlong jself )
{
    try
    {
        return ( jlong ) Self ( jself ) -> getReferencePosition ();
    }
    catch ( ErrorMsg & x )
    {
        ErrorMsgThrow ( jenv, xt_error_msg, x . what () );
    }
    catch ( std :: exception & x )
    {
        ErrorMsgThrow ( jenv, xt_runtime, x . what () );
    }
    catch ( ... )
    {
        JNI_INTERNAL_ERROR ( jenv, "%s", __func__ );
    }

    return 0;
}

/*
 * Class:     ngs_itf_PileupItf
 * Method:    GetPileupEvents
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_ngs_itf_PileupItf_GetPileupEvents
    ( JNIEnv * jenv, jobject jthis, jlong jself )
{
    try
    {
        PileupEventItf * new_ref = Self ( jself ) -> getPileupEvents ();
        return Cast ( new_ref );
    }
    catch ( ErrorMsg & x )
    {
        ErrorMsgThrow ( jenv, xt_error_msg, x . what () );
    }
    catch ( std :: exception & x )
    {
        ErrorMsgThrow ( jenv, xt_runtime, x . what () );
    }
    catch ( ... )
    {
        JNI_INTERNAL_ERROR ( jenv, "%s", __func__ );
    }

    return 0;
}

/*
 * Class:     ngs_itf_PileupItf
 * Method:    GetPileupDepth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_ngs_itf_PileupItf_GetPileupDepth
    ( JNIEnv * jenv, jobject jthis, jlong jself )
{
    try
    {
        return ( jint ) Self ( jself ) -> getPileupDepth ();
    }
    catch ( ErrorMsg & x )
    {
        ErrorMsgThrow ( jenv, xt_error_msg, x . what () );
    }
    catch ( std :: exception & x )
    {
        ErrorMsgThrow ( jenv, xt_runtime, x . what () );
    }
    catch ( ... )
    {
        JNI_INTERNAL_ERROR ( jenv, "%s", __func__ );
    }

    return 0;
}
