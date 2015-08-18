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

#ifndef _Included_py_ngs_itf_ErrorMsg
#define _Included_py_ngs_itf_ErrorMsg
#ifdef __cplusplus
extern "C" {
#endif

#include "py_ngs_defs.h"

#ifdef __cplusplus
}
#endif

#include <string.h>

#include <assert.h>
#include <stdint.h>

#include <ngs/itf/ErrorMsg.hpp>

namespace ngs
{
    class StringItf;
    
    class AlignmentItf;
    class FragmentItf;
    class ReadItf;
    class ReadGroupItf;
    class ReferenceItf;
    class PileupItf;
    class PileupEventItf;
    class ReadGroupItf;
    class StatisticsItf;
}

namespace
{
    template < typename T >
    T CheckedCast ( void* pRef )
    {
        if ( !pRef )
            throw ngs::ErrorMsg ( "NULL pRef parameter" );

        return ( T ) pRef;
    }

    template <typename T> struct c_type
    {
        typedef T type_c;
        typedef T type_cpp;
        //static type_cpp& cast_to_cpp (type_c& x) { return (type_cpp) x; }
    };

#define MAP_CPP_TO_C_TYPE( cpp_t, c_t )\
    template <> struct c_type<cpp_t>\
    {\
        typedef c_t   type_c;\
        typedef cpp_t type_cpp;\
    };

    MAP_CPP_TO_C_TYPE ( bool,                 int )
    MAP_CPP_TO_C_TYPE ( ngs::StringItf*,      void* )
    MAP_CPP_TO_C_TYPE ( ngs::AlignmentItf*,   void* )
    MAP_CPP_TO_C_TYPE ( ngs::FragmentItf*,    void* )
    MAP_CPP_TO_C_TYPE ( ngs::PileupItf*,      void* )
    MAP_CPP_TO_C_TYPE ( ngs::PileupEventItf*, void* )
    MAP_CPP_TO_C_TYPE ( ngs::ReadItf*,        void* )
    MAP_CPP_TO_C_TYPE ( ngs::ReadGroupItf*,   void* )
    MAP_CPP_TO_C_TYPE ( ngs::ReferenceItf*,   void* )
    MAP_CPP_TO_C_TYPE ( ngs::StatisticsItf*,  void* )

#undef MAP_CPP_TO_C_TYPE

    template < typename E >
    PY_RES_TYPE ExceptionHandler (E& x, void** ppNGSStrError)
    {
        assert(ppNGSStrError);

        char const* error_descr = x.what();
        size_t len = strlen ( error_descr );
        char* error_copy = new char [ len + 1 ];
        ::memcpy ( error_copy, error_descr, len + 1 );
        *((char**)ppNGSStrError) = error_copy;

        return PY_RES_ERROR;
    }

    PY_RES_TYPE ExceptionHandler ( void** ppNGSStrError )
    {
        char const error_text_constant[] = "INTERNAL ERROR";
        char* error_copy = new char [ sizeof error_text_constant ];
        ::memcpy ( error_copy, error_text_constant, sizeof error_text_constant );
        *((char**)ppNGSStrError) = error_copy;

        return PY_RES_ERROR;
    }

    template < typename TRef, typename TVal_cpp, typename TVal_c >
    PY_RES_TYPE PY_NGS_GetValue(void* pRef, TVal_c* pRet, void** ppNGSStrError, TVal_cpp (TRef::*pfn)() const)
    {
        PY_RES_TYPE ret = PY_RES_ERROR; // TODO: use xt_* codes
        try
        {
            TVal_cpp res = (CheckedCast< TRef* >(pRef) ->* pfn)();
            assert(pRet != NULL);
            *pRet = (TVal_c)res;
            ret = PY_RES_OK;
        }
        catch ( ngs::ErrorMsg & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( std::exception & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( ... )
        {
            ret = ExceptionHandler ( ppNGSStrError );
        }

        return ret;
    }

    template < typename TRef, typename TVal_cpp, typename TVal_c, typename T1 >
    PY_RES_TYPE PY_NGS_GetValueByParams1(void* pRef, T1 p1, TVal_c* pRet, void** ppNGSStrError, TVal_cpp (TRef::*pfn)(T1) const)
    {
        PY_RES_TYPE ret = PY_RES_ERROR; // TODO: use xt_* codes
        try
        {
            TVal_cpp res = (CheckedCast< TRef* >(pRef) ->* pfn)( p1 );
            assert(pRet != NULL);
            *pRet = (TVal_c)res;
            ret = PY_RES_OK;
        }
        catch ( ngs::ErrorMsg & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( std::exception & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( ... )
        {
            ret = ExceptionHandler ( ppNGSStrError );
        }

        return ret;
    }

    template < typename TRef, typename TVal_cpp, typename TVal_c, typename T1, typename T2 >
    PY_RES_TYPE PY_NGS_GetValueByParams2(void* pRef, T1 p1, T2 p2, TVal_c* pRet, void** ppNGSStrError, TVal_cpp (TRef::*pfn)(T1, T2) const)
    {
        PY_RES_TYPE ret = PY_RES_ERROR; // TODO: use xt_* codes
        try
        {
            TVal_cpp res = (CheckedCast< TRef* >(pRef) ->* pfn)( p1, p2 );
            assert(pRet != NULL);
            *pRet = (TVal_c)res;
            ret = PY_RES_OK;
        }
        catch ( ngs::ErrorMsg & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( std::exception & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( ... )
        {
            ret = ExceptionHandler ( ppNGSStrError );
        }

        return ret;
    }

    template < typename TRef, typename TVal_cpp, typename TVal_c, typename T1, typename T2, typename T3 >
    PY_RES_TYPE PY_NGS_GetValueByParams3(void* pRef, T1 p1, T2 p2, T3 p3, TVal_c* pRet, void** ppNGSStrError, TVal_cpp (TRef::*pfn)(T1, T2, T3) const)
    {
        PY_RES_TYPE ret = PY_RES_ERROR; // TODO: use xt_* codes
        try
        {
            TVal_cpp res = (CheckedCast< TRef* >(pRef) ->* pfn)( p1, p2, p3 );
            assert(pRet != NULL);
            *pRet = (TVal_c)res;
            ret = PY_RES_OK;
        }
        catch ( ngs::ErrorMsg & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( std::exception & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( ... )
        {
            ret = ExceptionHandler ( ppNGSStrError );
        }

        return ret;
    }

    template < typename TRef, typename TVal_cpp, typename TVal_c, typename T1, typename T2, typename T3, typename T4, typename T5 >
    PY_RES_TYPE PY_NGS_GetValueByParams5(void* pRef, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, TVal_c* pRet, void** ppNGSStrError, TVal_cpp (TRef::*pfn)(T1, T2, T3, T4, T5) const)
    {
        PY_RES_TYPE ret = PY_RES_ERROR; // TODO: use xt_* codes
        try
        {
            TVal_cpp res = (CheckedCast< TRef* >(pRef) ->* pfn)( p1, p2, p3, p4, p5 );
            assert(pRet != NULL);
            *pRet = (TVal_c)res;
            ret = PY_RES_OK;
        }
        catch ( ngs::ErrorMsg & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( std::exception & x )
        {
            ret = ExceptionHandler ( x, ppNGSStrError );
        }
        catch ( ... )
        {
            ret = ExceptionHandler ( ppNGSStrError );
        }

        return ret;
    }
}

/////////////////// Macros generating getters for any type of values ///////////////////////
#define GEN_PY_FUNC_GET( object_name, func_sub_name, value_type )\
PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValue <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, pRet, ppNGSStrError, & ngs::object_name##Itf::get##func_sub_name );\
}

// TODO: this function is for special cases when Itf method names do not adhere common pattern.
#define GEN_PY_FUNC_GET_EXPLICIT_NAMES( object_name, value_type, generated_function_name, itf_function_name )\
PY_RES_TYPE generated_function_name(void* pRef, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValue <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, pRet, ppNGSStrError, & ngs::object_name##Itf::itf_function_name );\
}

#define GEN_PY_FUNC_GET_BY_PARAMS_1( object_name, func_sub_name, value_type, param_type1, p1 )\
PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<param_type1>::type_c p1, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams1 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), pRet, ppNGSStrError, & ngs::object_name##Itf::get##func_sub_name );\
}

// TODO: this function is for special cases when Itf method names do not adhere common pattern.
#define GEN_PY_FUNC_GET_BY_PARAMS_1_EXPLICIT_NAMES( object_name, value_type, param_type1, p1, generated_function_name, itf_function_name )\
PY_RES_TYPE generated_function_name(void* pRef, c_type<param_type1>::type_c p1, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams1 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), pRet, ppNGSStrError, & ngs::object_name##Itf::itf_function_name );\
}

#define GEN_PY_FUNC_GET_BY_PARAMS_2( object_name, func_sub_name, value_type, param_type1, p1, param_type2, p2 )\
PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<param_type1>::type_c p1, c_type<param_type2>::type_c p2, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams2 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), (c_type<param_type2>::type_cpp)(p2), pRet, ppNGSStrError, & ngs::object_name##Itf::get##func_sub_name );\
}

// TODO: this function is for special cases when Itf method names do not adhere common pattern.
#define GEN_PY_FUNC_GET_BY_PARAMS_2_EXPLICIT_NAMES( object_name, value_type, param_type1, p1, param_type2, p2, generated_function_name, itf_function_name )\
PY_RES_TYPE generated_function_name(void* pRef, c_type<param_type1>::type_c p1, c_type<param_type2>::type_c p2, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams2 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), (c_type<param_type2>::type_cpp)(p2), pRet, ppNGSStrError, & ngs::object_name##Itf::itf_function_name );\
}


#define GEN_PY_FUNC_GET_BY_PARAMS_3( object_name, func_sub_name, value_type, param_type1, p1, param_type2, p2, param_type3, p3 )\
PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<param_type1>::type_c p1, c_type<param_type2>::type_c p2, c_type<param_type3>::type_c p3, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams3 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), (c_type<param_type2>::type_cpp)(p2), (c_type<param_type3>::type_cpp)(p3), pRet, ppNGSStrError, & ngs::object_name##Itf::get##func_sub_name );\
}


#define GEN_PY_FUNC_GET_BY_PARAMS_5( object_name, func_sub_name, value_type, param_type1, p1, param_type2, p2, param_type3, p3, param_type4, p4, param_type5, p5 )\
PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<param_type1>::type_c p1, c_type<param_type2>::type_c p2, c_type<param_type3>::type_c p3, c_type<param_type4>::type_c p4, c_type<param_type5>::type_c p5, c_type<value_type>::type_c* pRet, void** ppNGSStrError)\
{\
    return PY_NGS_GetValueByParams5 <ngs::object_name##Itf, value_type, c_type<value_type>::type_c> ( pRef, (c_type<param_type1>::type_cpp)(p1), (c_type<param_type2>::type_cpp)(p2), (c_type<param_type3>::type_cpp)(p3), (c_type<param_type4>::type_cpp)(p4), (c_type<param_type5>::type_cpp)(p5), pRet, ppNGSStrError, & ngs::object_name##Itf::get##func_sub_name );\
}

/////////////////// Specialized macros to generate getters returning Strings ////////////////

#define GEN_PY_FUNC_GET_STRING( object_name, func_sub_name ) GEN_PY_FUNC_GET( object_name, func_sub_name, ngs::StringItf* )
#define GEN_PY_FUNC_GET_STRING_BY_PARAMS_1( object_name, func_sub_name, param_type1, p1 ) GEN_PY_FUNC_GET_BY_PARAMS_1( object_name, func_sub_name, ngs::StringItf*, param_type1, p1 )
#define GEN_PY_FUNC_GET_STRING_BY_PARAMS_2( object_name, func_sub_name, param_type1, p1, param_type2, p2 ) GEN_PY_FUNC_GET_BY_PARAMS_2( object_name, func_sub_name, ngs::StringItf*, param_type1, p1, param_type2, p2 )

/////////////////// Specialized macros to generate getters returning Strings and reinterpretting pRef as different type ///////////////

//#define GEN_PY_FUNC_GET_STRING_CAST( object_name, func_sub_name, object_reinterpret_name )\
//PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, void** pRet, void** ppNGSStrError)\
//{\
//    return PY_NGS_GetValue <ngs::object_reinterpret_name##Itf, ngs::StringItf*, c_type<ngs::StringItf*>::type_c> ( pRef, pRet, ppNGSStrError, & ngs::object_reinterpret_name##Itf::get##func_sub_name );\
//}
//
//#define GEN_PY_FUNC_GET_STRING_BY_PARAMS_2_CAST( object_name, func_sub_name, object_reinterpret_name, param_type1, p1, param_type2, p2 )\
//PY_RES_TYPE PY_NGS_##object_name##Get##func_sub_name(void* pRef, c_type<param_type1>::type_c p1, c_type<param_type2>::type_c p2, void** pRet, void** ppNGSStrError)\
//{\
//    return PY_NGS_GetValueByParams2 <ngs::object_reinterpret_name##Itf> ( pRef, (c_type<param_type1>::type_cpp)(p1), (c_type<param_type2>::type_cpp)(p2), pRet, ppNGSStrError, & ngs::object_reinterpret_name##Itf::get##func_sub_name );\
//}

///////////////// Separate macro to generate iterator-next functions /////////////////////

#define GEN_PY_ITERATOR_NEXT_IMPL(object_name, generated_function_name)\
PY_RES_TYPE generated_function_name ( void* pRef, int* pRet, void** ppNGSStrError )\
{\
    PY_RES_TYPE ret = PY_RES_ERROR;\
    try\
    {\
        bool res = CheckedCast< ngs::object_name##Itf* >(pRef) -> next##object_name();\
        assert(pRet != NULL);\
        *pRet = (int)res;\
        ret = PY_RES_OK;\
    }\
    catch ( ngs::ErrorMsg & x )\
    {\
        ret = ExceptionHandler ( x, ppNGSStrError );\
    }\
    catch ( std::exception & x )\
    {\
        ret = ExceptionHandler ( x, ppNGSStrError );\
    }\
    catch ( ... )\
    {\
        ret = ExceptionHandler ( ppNGSStrError );\
    }\
    return ret;\
}

#define GEN_PY_ITERATOR_NEXT(object_name) GEN_PY_ITERATOR_NEXT_IMPL(object_name, PY_NGS_##object_name##IteratorNext )
#define GEN_PY_ITERATOR_NEXT_EXPLICIT_NAMES(object_name, generated_function_name) GEN_PY_ITERATOR_NEXT_IMPL(object_name, generated_function_name )

#endif /* _Included_py_ngs_itf_ErrorMsg */
