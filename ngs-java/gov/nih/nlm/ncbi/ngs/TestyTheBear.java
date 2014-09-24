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

package gov.nih.nlm.ncbi.ngs;

import ngs.ErrorMsg;
import ngs.ReadCollection;
import ngs.Read;
import ngs.ReadIterator;
import ngs.Fragment;

import java.util.ArrayList;

public class TestyTheBear
{
    // a list of accessions to test
    static String [] ACCESSIONS =
    {
          "SRR000001"  // SRA
        , "SRR600096"  // SRA_DB
        , "SRR1063272" // CSRA
    };

    // loop types:
    //  'a' processes whole reads
    //  'b' processes fragments
    static char LOOP_TYPE = 'a';

    // use a range to both test the facility
    // and to limit the rows that will be processed
    static boolean USE_RANGE = true;
    static long START = 10;
    static long COUNT = 5;

    // a boolean to silence the printing code
    static boolean SILENT = false;

    static void loopa ( ReadCollection run, ReadIterator it )
        throws ErrorMsg, Exception
    {
        String run_name = run . getName ();

        long i;
        for ( i = 0; it . nextRead (); ++ i )
        {
            String read_name = it . getReadName ();
            String bases = it . getReadBases ();
            String qual = it . getReadQualities ();

            if ( SILENT )
                continue;

            System . out . println ( "@"
                                     + run_name
                                     + "."
                                     + it . getReadId ()
                                     + " "
                                     + read_name
                                     + " length="
                                     + bases . length ()
                );

if (TEST_DLL) {                      // This code is used for
 Logger.Level l = Logger.getLevel(); // DLL load test only and is turned on
 Logger.setLevel(Logger.Level.FINE); // by running this app with -q
 LibManager.DetectJVM();
 Logger.setLevel(l);
 if (LibPathIterator.list()) {
  System.err.println("LibPathIterator OK");
  System.exit(0);
 } else {
  System.err.println("LibPathIterator KO");
  System.exit(1);
 }
}

            System . out . println ( bases );

            System . out . println ( "+"
                                     + run_name
                                     + "."
                                     + it . getReadId ()
                                     + " "
                                     + read_name
                                     + " length="
                                     + qual . length ()
                );

            System . out . println ( qual );
        }

        System . err . println ( "Read "
                                + i
                                + " spots for "
                                + run_name
            );

        System . err . println ( "Written "
                                 + i
                                 + " spots for "
                                 + run_name
            );
    }

    static void loopb ( ReadCollection run, ReadIterator it )
        throws ErrorMsg, Exception
    {
        String run_name = run . getName ();

        long i;
        for ( i = 0; it . nextRead (); ++ i )
        {
            String read_name = it . getReadName ();

            int j; // this is wrong - needs to retrieve idx from Read
            for ( j = 1; it . nextFragment (); ++ j )
            {
                String bases = it . getFragmentBases ();
                String qual = it . getFragmentQualities ();

                if ( SILENT )
                    continue;

                System . out . println ( "@"
                                         + run_name
                                         + "."
                                         + it . getReadId ()
                                         + "."
                                         + j
                                         + " "
                                         + read_name
                                         + " length="
                                         + bases . length ()
                    );

                System . out . println ( bases );

                System . out . println ( "+"
                                         + run_name
                                         + "."
                                         + it . getReadId ()
                                         + "."
                                         + j
                                         + " "
                                         + read_name
                                         + " length="
                                         + qual . length ()
                    );

                System . out . println ( qual );
            }
        }

        System . err . println ( "Read "
                                + i
                                + " spots for "
                                + run_name
            );

        System . err . println ( "Written "
                                 + i
                                 + " spots for "
                                 + run_name
            );
    }

    static void run ( String accession )
        throws ErrorMsg, Exception
    {
        // open the named run
        ReadCollection run = NGS . openReadCollection ( accession );

        // get an iterator
        ReadIterator it = USE_RANGE ?
            run . getReadRange ( START, COUNT ):
            run . getReads ( Read . all );

        // run the requested loop
        switch ( LOOP_TYPE )
        {
        case 'a':
            loopa ( run, it );
            break;
        case 'b':
            loopb ( run, it );
            break;
        default:
            throw new Exception ( "WRITE THE CODE!" );
        }
    }

    static boolean parseCmdLine ( String [] args )
        throws Exception
    {
        int i;
        for ( i = 0; i < args . length; ++ i )
        {
            String arg = args [ i ];
            if ( arg . equals ( "-?" ) || arg . equals ( "-h" ) || arg . equals ( "--help" ) )
            {
                System . err . println (
                    "Usage: TestyTheBear [ <options> ] [ <read-collection> [ <read-collection> ... ] ]\n" +
                    "\n" +
                    "  Options:\n" +
                    "    '--loop' ( 'a' | 'b' )  : select loop option where a = reads, b = fragments\n" +
                    "    '--read'                : synonym for '--loop a'\n" +
                    "    '--frag'                : synonym for '--loop b'\n" +
                    "    '--full'                : select entire run ( default is range " + START + ".." + ( START + COUNT - 1 ) + " )\n" +
                    "    '--range' start count   : alter default range to use provided values\n" +
                    "    '--silent'              : do not run printing code ( better for timing )\n" +
                    "    '-q'                    : be quiet: just make sure JNI DLL was loaded\n" +
                    "    '--help|-h|-?'          : print this message and exit\n"
                    );

                try {
                    Logger.fine(LibManager.getEnv());
                } catch (Exception e) {
                    System.err.println(e);
                }

                return false;
            }
        }

        ArrayList < String > accessions = null;

        for ( i = 0; i < args . length; ++ i )
        {
            String arg = args [ i ];

            if ( arg . charAt ( 0 ) != '-' )
            {
                if ( accessions == null )
                    accessions = new ArrayList < String > ();

                accessions . add ( arg );
            }
            else
            {
                if ( arg . equals ( "--loop" ) )
                    LOOP_TYPE = args [ ++ i ] . toLowerCase () . charAt ( 0 );
                else if ( arg . equals ( "--read" ) )
                    LOOP_TYPE = 'a';
                else if ( arg . equals ( "--frag" ) )
                    LOOP_TYPE = 'b';
                else if ( arg . equals ( "--full" ) )
                    USE_RANGE = false;
                else if ( arg . equals ( "--range" ) )
                {
                    START = Long . parseLong ( args [ ++ i ] );
                    COUNT = Long . parseLong ( args [ ++ i ] );
                    if ( START < 1 )
                        throw new Exception ( "bad range start: " + START );
                    if ( COUNT < 0 )
                        throw new Exception ( "bad range count: " + COUNT );

                }
                else if ( arg . equals ( "--silent" ) )
                    SILENT = true;
                else if ( arg . equals ( "-q" ) )
                    TEST_DLL = true;
                else
                {
                    throw new Exception ( "unknown switch: '" + arg + "'" );
                }
            }
        }

        switch ( LOOP_TYPE )
        {
        case 'a':
        case 'b':
            break;
        default:
            System . err . println ( "bad loop type - must be 'a' or 'b'" );
            return false;
        }

        if ( accessions != null )
        {
            ACCESSIONS = accessions . toArray ( new String [ accessions . size () ] );
        }

        System . err . print ( "# Processing " );
        if ( USE_RANGE )
            System . err . println ( "rows " + START + ".." + ( START + COUNT - 1 ) + " of:" );
        else
            System . err . println ( "complete:" );

        System . err . print ( "# Runs " );

        String sep = "{ ";
        int count = ACCESSIONS . length;
        if ( count > 16 )
            count = 16;
        for ( i = 0; i < count; ++ i )
        {
            System . err . print ( sep + ACCESSIONS [ i ] );
            sep = ", ";
        }

        if ( count < ACCESSIONS . length )
            System . err . println ( ", ... }" );
        else
            System . err . println ( " }" );

        System . err . print ( "# Using " );
        System . err . print ( "dual-level ReadIterator, " );
        if ( LOOP_TYPE == 'b' )
            System . err . println ( "and Fragment access." );
        else
            System . err . println ( "and whole Read access." );
        if ( SILENT )
            System . err . println ( "# OUTPUT IS SILENCED." );
        System . err . println ( "# Execute with '--help' to see how to modify these settings.\n" );

        return true;
    }

    public static void main ( String [] args )
    {
        try
        {
            if ( parseCmdLine ( args ) )
            {
                int i, num_errs;
                for ( i = num_errs = 0; i < ACCESSIONS . length; ++ i )
                {
                    try
                    {
                        run ( ACCESSIONS [ i ] );
                    }
                    catch ( ErrorMsg x )
                    {
                        System . err . println ( "ERROR - processing run '" + ACCESSIONS [ i ] + "'" );
                        System . err . println ( x . toString () );
                        x . printStackTrace ();
                        ++ num_errs;
                    }
                    catch ( Exception x )
                    {
                        System . err . println ( "ERROR - processing run '" + ACCESSIONS [ i ] + "'" );
                        System . err . println ( x . toString () );
                        x . printStackTrace ();
                        ++ num_errs;
                    }
                }
            
                System . err . println ( "Processed " + ACCESSIONS . length + " runs with " + num_errs + " errors" );
            }
        }
        catch ( Exception x )
        {
            System . err . println ( x . toString () );
            x . printStackTrace ();
        }
    }

    private static boolean TEST_DLL = false;
}
