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
 * Represents a reference sequence
 */
public interface Reference
{

    /** 
     * getCommonName
     * @return the common name of reference, e.g. "chr1"
     */
    String getCommonName ()
        throws ErrorMsg;

    /** 
     * getCanonicalName
     * @return the accessioned name of reference, e.g. "NC_000001.11"
     */
    String getCanonicalName ()
        throws ErrorMsg;


    /** 
     * getIsCircular
     * @return true if reference is circular
     */
    boolean getIsCircular ()
        throws ErrorMsg;


    /** 
     * getLength
     * @return the length of the reference sequence
     */
    long getLength ()
        throws ErrorMsg;


    /** 
     * getReferenceBases
     * @param offset is zero-based and non-negative
     * @return sub-sequence bases for Reference
     */
    String getReferenceBases ( long offset )
        throws ErrorMsg;
    
    /** 
     * getReferenceBases
     * @param offset is zero-based and non-negative
     * @param length must be >= 0
     * @return sub-sequence bases for Reference
     */
    String getReferenceBases ( long offset, long length )
        throws ErrorMsg;

    /** 
     * getReferenceChunk
     * @param offset is zero-based and non-negative
     * @return largest contiguous chunk available of sub-sequence bases for Reference
     * <p>
     *  NB - actual returned sequence may be shorter
     *  than requested. to obtain all bases available
     *  in chunk, use a negative "size" value
     * </p>
     */
    String getReferenceChunk ( long offset )
        throws ErrorMsg;
    
    /** 
     * getReferenceChunk
     * @param offset is zero-based and non-negative
     * @param length must be >= 0
     * @return largest contiguous chunk available of sub-sequence bases for Reference
     * <p>
     *  NB - actual returned sequence may be shorter
     *  than requested. to obtain all bases available
     *  in chunk, use a negative "size" value
     * </p>
     */
    String getReferenceChunk ( long offset, long length )
        throws ErrorMsg;


    /*----------------------------------------------------------------------
     * ALIGNMENTS
     */

    /** 
     * getAlignment
     * @return an individual Alignment
     * @throws ErrorMsg if Alignment does not exist or is not part of this Reference
     */
    Alignment getAlignment ( String alignmentId )
        throws ErrorMsg;

    /* AlignmentCategory
     * see Alignment for categories
     */

    /** 
     * getAlignments
     * @return an iterator of contained alignments
     */
    AlignmentIterator getAlignments ( int categories )
        throws ErrorMsg;

    /** 
     * getAlignmentSlice
     * @param start is a signed 0-based offset from the start of the Reference
     * @param length is the length of the slice.
     * @return an iterator across a range of Alignments
     */
    AlignmentIterator getAlignmentSlice ( long start, long length )
        throws ErrorMsg;
    
    /** 
     * getAlignmentSlice
     * @param start is a signed 0-based offset from the start of the Reference
     * @param length is the length of the slice.
     * @param categories provides a means of filtering by AlignmentCategory
     * @return an iterator across a range of Alignments
     */
    AlignmentIterator getAlignmentSlice ( long start, long length, int categories )
        throws ErrorMsg;


    /*----------------------------------------------------------------------
     * PILEUP
     */
    
    /** 
     * getPileups
     * @return an iterator of contained Pileups
     */
    PileupIterator getPileups ( int categories )
        throws ErrorMsg;

    /** 
     * getPileupSlice
     * Creates a PileupIterator on a slice (window) of reference
     * @param start is the signed starting position on reference
     * @param length is the unsigned number of bases in the window
     * @return an iterator of contained Pileups
     */
    PileupIterator getPileupSlice ( long start, long length )
        throws ErrorMsg;

    /** 
     * getPileupSlice
     * Creates a PileupIterator on a slice (window) of reference
     * @param start is the signed starting position on reference
     * @param length is the unsigned number of bases in the window
     * @param categories provides a means of filtering by AlignmentCategory
     * @return an iterator of contained Pileups
     */
    PileupIterator getPileupSlice ( long start, long length, int categories )
        throws ErrorMsg;
}
