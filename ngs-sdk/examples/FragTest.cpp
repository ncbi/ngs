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

#include <NGS.hpp>
//#include <ngs-bam/ngs-bam.hpp>
#include <ngs/ErrorMsg.hpp>
#include <ngs/ReadCollection.hpp>
#include <ngs/ReadIterator.hpp>
#include <ngs/Read.hpp>


#include <math.h>
#include <iostream>

using namespace ngs;
using namespace std;

class FragTest
{
public:

    static void run_common ( ReadCollection & run, int splitNum, int splitNo )
    {

        String run_name = run.getName ();

        ReadIterator it = run.getReads ( Read::all );

        long i;
        for ( i = 0; it.nextRead (); ++ i )
        {
            cout << it.getReadId();

            //iterate through fragments
            while ( it.nextFragment () )
                cout << '\t' <<  it.getFragmentBases ();

            cout << '\n';
        }

        cerr << "Read " << i << " spots for " << run_name << '\n';
    }

    static void run_csra ( String acc, int splitNum, int splitNo )
    {
        // open requested accession using SRA implementation of the API
        ReadCollection run = ncbi::NGS::openReadCollection ( acc );
        run_common ( run, splitNum, splitNo );
    }

    static void run_bam ( String acc, int splitNum, int splitNo )
    {
        // open requested accession using example BAM implementation of the API
        //ReadCollection run = NGS_BAM::openReadCollection ( acc );
        //run_common ( run, splitNum, splitNo );
    }

    static void run ( String acc, int splitNum, int splitNo )
    {
        size_t dot = acc . find_last_of ( '.' );
        if ( dot != string :: npos )
        {
            String extension = acc . substr ( dot );
            if ( extension == ".bam" || extension == ".BAM" )
            {
                //run_bam ( acc, splitNum, splitNo );
                return;
            }
        }

        run_csra ( acc, splitNum, splitNo );
    }

};

int main (int argc, char const *argv[])
{
    if ( argc != 4 )
    {
        cerr << "Usage: FragTest accession NumChunks ChunkNo\n";
    }
    else try
    {
        ncbi::NGS::setAppVersionString ( "FragTest.1.1.0" );
        FragTest::run ( argv[1], atoi ( argv[2] ), atoi ( argv[3] ) );
        return 0;
    }
    catch ( ErrorMsg & x )
    {
        cerr <<  x.toString () << '\n';
    }
    catch ( exception & x )
    {
        cerr <<  x.what () << '\n';
    }
    catch ( ... )
    {
        cerr <<  "unknown exception\n";
    }

    return 10;
}
