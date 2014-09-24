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

package ngs.itf;

import ngs.ErrorMsg;
import ngs.Pileup;
import ngs.PileupEventIterator;


/*==========================================================================
 * PileupItf
 *  represents a slice through a stack of Alignments
 *  at a given position on the Reference
 */
class PileupItf
    extends Refcount
    implements Pileup
{

    /********************
     * Pileup Interface *
     ********************/

    /*----------------------------------------------------------------------
     * Reference
     */

    /* getReferenceSpec
     */
    public String getReferenceSpec ()
        throws ErrorMsg
    {
        return this . GetReferenceSpec ( self );
    }

    /* getReferencePosition
     */
    public long getReferencePosition ()
        throws ErrorMsg
    {
        return this . GetReferencePosition ( self );
    }


    /*----------------------------------------------------------------------
     * PileupEvent
     */

    /* getPileupEvents
     */
    public PileupEventIterator getPileupEvents ()
        throws ErrorMsg
    {
        long ref = this . GetPileupEvents ( self );
        try
        {
            return new PileupEventIteratorItf ( ref );
        }
        catch ( Exception x )
        {
            this . release ( ref );
            throw new ErrorMsg ( x . toString () );
        }
    }


    /*----------------------------------------------------------------------
     * details of this pileup row
     */

    /* getPileupDepth
     *  returns the coverage depth
     *  at the current reference position
     */
    public int getPileupDepth ()
        throws ErrorMsg
    {
        return this . GetPileupDepth ( self );
    }


    /****************************
     * PileupItf Implementation *
     **************************/


    // constructors
    PileupItf ( long ref )
    {
        super ( ref );
    }

    PileupItf ( Pileup obj )
        throws ErrorMsg
    {
        super ( 0 );
        try
        {
            PileupItf ref = ( PileupItf ) obj;
            this . self = ref . duplicate ();
        }
        catch ( Exception x )
        {
            throw new ErrorMsg ( x . toString () );
        }
    }

    // native interface
    private native String GetReferenceSpec ( long self )
        throws ErrorMsg;
    private native long GetReferencePosition ( long self )
        throws ErrorMsg;
    private native long GetPileupEvents ( long self )
        throws ErrorMsg;
    private native int GetPileupDepth ( long self )
        throws ErrorMsg;
}
