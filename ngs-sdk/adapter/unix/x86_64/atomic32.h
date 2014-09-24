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

#ifndef _h_ngs_engine_atomic32_
#define _h_ngs_engine_atomic32_

#ifndef _h_ngs_adapter_defs_
#include <ngs/adapter/defs.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* int atomic32_read ( const atomic32_t *v ); */
#define atomic32_read( v ) \
    ( ( v ) -> counter )

/* void atomic32_set ( atomic32_t *v, int i ); */
#define atomic32_set( v, i ) \
    ( ( void ) ( ( ( v ) -> counter ) = ( i ) ) )

/* add to v -> counter and return the prior value */
static __inline__ int atomic32_read_and_add ( atomic32_t *v, int i )
{
    int rtn;
    __asm__ __volatile__
    (
        "mov (%%rdi), %%eax;"
        "mov %%esi, %%ecx;"
        "add %%eax, %%ecx;"
        "lock;"
        "cmpxchg %%ecx, (%%rdi);"
        "jne .-8"
        : "=a" ( rtn )
        : "D" ( v ), "S" ( i )
        : "%ecx"
    );
    return rtn;
}

static __inline__ void atomic32_dec ( atomic32_t *v )
{
    __asm__ __volatile__
    (
        "lock;"
        "decl %0"
        : "=m" ( v -> counter )
        : "m" ( v -> counter )
    );
}

static __inline__
int atomic32_read_and_add_gt ( atomic32_t *v, int i, int t )
{
    int rtn;
    __asm__ __volatile__
    (
        "mov (%%rdi), %%eax;"
        "cmp %%edx, %%eax;"
        "mov %%esi, %%ecx;"
        "jle .+10;"
        "add %%eax, %%ecx;"
        "lock;"
        "cmpxchg %%ecx, (%%rdi);"
        "jne .-12"
        : "=a" ( rtn )
        : "D" ( v ), "S" ( i ), "d" ( t )
        : "%ecx"
    );
    return rtn;
}

#ifdef __cplusplus
}
#endif

#endif /* _h_ngs_engine_atomic32_ */
