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

package ngs;


/** 
 *  Represents a single cell of a sparse 2D matrix with Reference coordinates on one axis
 *  and stacked Alignments on the other axis
 */
public interface PileupEvent
{

    /*----------------------------------------------------------------------
     * Reference
     */

    /** 
     * getReferenceSpec
     */
    String getReferenceSpec ()
        throws ErrorMsg;

    /**
     * getReferencePosition
     */
    long getReferencePosition ()
        throws ErrorMsg;

    /** 
     * getMappingQuality
     */
    int getMappingQuality ()
        throws ErrorMsg;


    /*----------------------------------------------------------------------
     * Alignment
     */

    /**
     * getAlignmentId
     * @return unique AlignmentId within ReadCollection
     */
    String getAlignmentId ()
        throws ErrorMsg;

    /**
     * getAlignment
     */
    Alignment getAlignment ()
        throws ErrorMsg;

    /** 
     * getAlignmentPosition
     */
    long getAlignmentPosition ()
        throws ErrorMsg;

    /**
     * getFirstAlignmentPosition
     * @return the position of this Alignment's first event in Reference coordinates
     */
    long getFirstAlignmentPosition ()
        throws ErrorMsg;

    /**
     * getLastAlignmentPosition
     * @return the position of this Alignment's last event in INCLUSIVE Reference coordinates
     */
    long getLastAlignmentPosition ()
        throws ErrorMsg;


    /*----------------------------------------------------------------------
     * event details
     */

    /**
     *  EventType
     */
    static int match     = 0;
    static int mismatch  = 1;
    static int insertion = 2;
    static int deletion  = 3;

    /**
     * getEventType
     * @return the type of event being represented
     */
    int getEventType ()
        throws ErrorMsg;

    /**
     * getAlignmentBase
     * @return retrieves base aligned at current Reference position
     * @throws ErrorMsg if event is an insertion or deletion
     */
    char getAlignmentBase ()
        throws ErrorMsg;

    /** 
     * getAlignmentQuality
     * @return retrieves base aligned at current Reference position
     * @throws ErrorMsg if event is an insertion or deletion
     */
    char getAlignmentQuality ()
        throws ErrorMsg;


    /**
     * getInsertionBases
     * @return bases corresponding to insertion event
     */
    String getInsertionBases ()
        throws ErrorMsg;

    /** 
     * getInsertionQualities
     * @return qualities corresponding to insertion event
     */
    String getInsertionQualities ()
        throws ErrorMsg;

    /**
     * getDeletionCount
     * @return the number of Reference base positions remaining until the next non-deletion event in this alignment.
     */
    int getDeletionCount ()
        throws ErrorMsg;
}
