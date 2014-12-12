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
 *  Represents an alignment between a Fragment and Reference sub-sequence
 *  provides a path to Read and mate Alignment
 */
public interface Alignment
    extends Fragment
{

    /** 
     * Retrieve an identifying String that can be used for later access.
     * The id will be unique within ReadCollection.
     * @return alignment id
     * @throws ErrorMsg if the property cannot be retrieved
     */
    String getAlignmentId ()
        throws ErrorMsg;


    /*------------------------------------------------------------------
     * Reference
     */

    /** 
     * getReferenceSpec
     */
    String getReferenceSpec ()
        throws ErrorMsg;

    /**
     * getMappingQuality 
     * @return mapping quality 
     */
    int getMappingQuality ()
        throws ErrorMsg;

    /** 
     * getReferenceBases
     * @return reference bases
     */
    String getReferenceBases ()
        throws ErrorMsg;


    /*------------------------------------------------------------------
     * Fragment
     */

    /**
     *  getReadGroup
     */
    String getReadGroup ()
        throws ErrorMsg;

    /** 
     * getReadId
     */
    String getReadId ()
        throws ErrorMsg;

    /** 
     * getClippedFragmentBases
     * @return clipped fragment bases
     */
    String getClippedFragmentBases ()
        throws ErrorMsg;

    /** 
     * getClippedFragmentQualities
     * @return clipped fragment phred quality values using ASCII offset of 33
     */
    String getClippedFragmentQualities ()
        throws ErrorMsg;

    /** 
     * getAlignedFragmentBases
     * @return fragment bases in their aligned orientation
     */
    String getAlignedFragmentBases ()
        throws ErrorMsg;

    /*------------------------------------------------------------------
     * details of this alignment
     */

    /* AlignmentCategory
     */
    static int primaryAlignment   = 1;
    static int secondaryAlignment = 2;
    static int all                = primaryAlignment | secondaryAlignment;

    /** 
     * Alignments are categorized as primary or secondary (alternate).
     * @return either Alignment.primaryAlignment or Alignment.secondaryAlignment
     * @throws ErrorMsg if the property cannot be retrieved
     */
    int getAlignmentCategory ()
        throws ErrorMsg;

    /** 
     * Retrieve the Alignment's starting position on the Reference
     * @return unsigned 0-based offset from start of Reference
     * @throws ErrorMsg if the property cannot be retrieved
     */
    long getAlignmentPosition ()
        throws ErrorMsg;

    /**
     * Retrieve the projected length of an Alignment projected upon Reference.
     * @return unsigned length of projection
     * @throws ErrorMsg if the property cannot be retrieved
     */
    long getAlignmentLength ()
        throws ErrorMsg;

    /**
     * Test if orientation is reversed with respect to the Reference sequence.
     * @return true if reversed
     * @throws ErrorMsg if the property cannot be retrieved
     */
    boolean getIsReversedOrientation ()
        throws ErrorMsg;

    /* ClipEdge
     */
    static int clipLeft  = 0;
    static int clipRight = 1;

    /** 
     * getSoftClip 
     */
    int getSoftClip ( int edge )
        throws ErrorMsg;

    /** 
     * getTemplateLength
     */
    long getTemplateLength ()
        throws ErrorMsg;

    /** 
     * getShortCigar
     * @return a text string describing alignment details
     */
    String getShortCigar ( boolean clipped )
        throws ErrorMsg;

    /** 
     * getLongCigar
     * @return a text string describing alignment details
     */
    String getLongCigar ( boolean clipped )
        throws ErrorMsg;

    /**
     * getRNAOrientation
     * @return '+' if positive strand is transcribed
     * @return '-' if negative strand is transcribed
     * @return '?' if unknown
     */
    char getRNAOrientation ()
        throws ErrorMsg;


    /*------------------------------------------------------------------
     * details of mate alignment
     */

    /** 
     * hasMate
     */
    boolean hasMate ();
        
    /** 
     * getMateAlignmentId
     */
    String getMateAlignmentId ()
        throws ErrorMsg;

    /** 
     * getMateAlignment
     */
    Alignment getMateAlignment ()
        throws ErrorMsg;

    /** 
     * getMateReferenceSpec
     */
    String getMateReferenceSpec ()
        throws ErrorMsg;

    /**
     * getMateIsReversedOrientation
     */
    boolean getMateIsReversedOrientation ()
        throws ErrorMsg;
}
