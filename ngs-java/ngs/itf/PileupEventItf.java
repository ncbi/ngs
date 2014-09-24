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
import ngs.PileupEvent;
import ngs.Alignment;


/*==========================================================================
 * PileupEventItf
 *  represents a single cell of a sparse 2D matrix
 *  with Reference coordinates on one axis
 *  and stacked Alignments on the other axis
 */
class PileupEventItf
    extends Refcount
    implements PileupEvent
{

    /*************************
     * PileupEvent Interface *
     *************************/

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

    /* getMappingQuality
     */
    public int getMappingQuality ()
        throws ErrorMsg
    {
        return this . GetMappingQuality ( self );
    }


    /*----------------------------------------------------------------------
     * Alignment
     */

    /* getAlignmentId
     *  unique within ReadCollection
     */
    public String getAlignmentId ()
        throws ErrorMsg
    {
        return this . GetAlignmentId ( self );
    }

    /* getAlignment
     */
    public Alignment getAlignment ()
        throws ErrorMsg
    {
        long ref = this . GetAlignment ( self );
        try
        {
            return new AlignmentItf ( ref );
        }
        catch ( Exception x )
        {
            this . release ( ref );
            throw new ErrorMsg ( x . toString () );
        }
    }

    /* getAlignmentPosition
     */
    public long getAlignmentPosition ()
        throws ErrorMsg
    {
        return this . GetAlignmentPosition ( self );
    }

    /* getFirstAlignmentPosition
     *  returns the position of this Alignment's first event
     *  in Reference coordinates
     */
    public long getFirstAlignmentPosition ()
        throws ErrorMsg
    {
        return this . GetFirstAlignmentPosition ( self );
    }

    /* getLastAlignmentPosition
     *  returns the position of this Alignment's last event
     *  in INCLUSIVE Reference coordinates
     */
    public long getLastAlignmentPosition ()
        throws ErrorMsg
    {
        return this . GetLastAlignmentPosition ( self );
    }


    /*----------------------------------------------------------------------
     * event details
     */

    /* getEventType
     *  the type of event being represented
     */
    public int getEventType ()
        throws ErrorMsg
    {
        return this . GetEventType ( self );
    }

    /* getAlignmentBase
     *  retrieves base aligned at current Reference position
     *  throws exception if event is an insertion or deletion
     */
    public char getAlignmentBase ()
        throws ErrorMsg
    {
        return this . GetAlignmentBase ( self );
    }

    /* getAlignmentQuality
     *  retrieves base aligned at current Reference position
     *  throws exception if event is an insertion or deletion
     */
    public char getAlignmentQuality ()
        throws ErrorMsg
    {
        return this . GetAlignmentQuality ( self );
    }


    /* getInsertionBases
     *  returns bases corresponding to insertion event
     */
    public String getInsertionBases ()
        throws ErrorMsg
    {
        return this . GetInsertionBases ( self );
    }

    /* getInsertionQualities
     *  returns qualities corresponding to insertion event
     */
    public String getInsertionQualities ()
        throws ErrorMsg
    {
        return this . GetInsertionQualities ( self );
    }

    /* getDeletionCount
     *  returns the number of bases remaining in deletion event
     *  i.e. the number of Reference base positions remaining
     *  until the next non-deletion event in this alignment.
     */
    public int getDeletionCount ()
        throws ErrorMsg
    {
        return this . GetDeletionCount ( self );
    }


    /*********************************
     * PileupEventItf Implementation *
     *******************************/


    // constructors
    PileupEventItf ( long ref )
    {
        super ( ref );
    }

    PileupEventItf ( PileupEvent obj )
        throws ErrorMsg
    {
        super ( 0 );
        try
        {
            PileupEventItf ref = ( PileupEventItf ) obj;
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
    private native int GetMappingQuality ( long self )
        throws ErrorMsg;
    private native String GetAlignmentId ( long self )
        throws ErrorMsg;
    private native long GetAlignment ( long self )
        throws ErrorMsg;
    private native long GetAlignmentPosition ( long self )
        throws ErrorMsg;
    private native long GetFirstAlignmentPosition ( long self )
        throws ErrorMsg;
    private native long GetLastAlignmentPosition ( long self )
        throws ErrorMsg;
    private native int GetEventType ( long self )
        throws ErrorMsg;
    private native char GetAlignmentBase ( long self )
        throws ErrorMsg;
    private native char GetAlignmentQuality ( long self )
        throws ErrorMsg;
    private native String GetInsertionBases ( long self )
        throws ErrorMsg;
    private native String GetInsertionQualities ( long self )
        throws ErrorMsg;
    private native int GetDeletionCount ( long self )
        throws ErrorMsg;
}
