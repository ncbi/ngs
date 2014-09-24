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
* ==============================================================================
*
*/

package gov.nih.nlm.ncbi.ngs;

/** The Logger is used to print log messages to stderr.
 An associated "Level" reflects a minimum Level that this logger cares about. */
class Logger {

    enum Level {
        WARNING(0), // WARNING is a message level indicating a potential problem
        FINE(1),    // FINE is a message level providing tracing information
        FINER(2),   // FINER indicates a fairly detailed tracing message
        FINEST(3);  // FINEST indicates a highly detailed tracing message
        private Level(int id) { this.id = id; }
        private int id() { return id; }
        private final int id;
    }

    /** Default logging Level is WARNING.
        It could be changed by setting "vdb.log" java system property */
    private Logger() {
        String s = System.getProperty("vdb.log");
        if (s == null) {
            level = Level.WARNING;
        } else if (s.equals("FINER")) {
            level = Level.FINER;
        } else if (s.equals("FINEST")) {
            level = Level.FINEST;
        } else {
            level = Level.FINE;
        }
    }

    static void fine  (String msg) { logger.log(msg, Level.FINE  ); }
    static void finer (String msg) { logger.log(msg, Level.FINER ); }
    static void finest(String msg) { logger.log(msg, Level.FINEST); }

    static Level getLevel() { return logger.level; }
    static void setLevel(Level newLevel) { logger.level = newLevel; }

    private void log(String msg, Level level) {
        if (this.level.id() < level.id()) {
            return;
        }
        System.err.println(msg);
    }

    private Level level;
    private static Logger logger = new Logger();
}
