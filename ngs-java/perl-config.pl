# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#               National Center for Biotechnology Information
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government have not placed any restriction on its use or reproduction.
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
#  Please cite the author in any work or product based on this material.
#
# ===========================================================================

my $DEBUG;
#++$DEBUG;

use strict;

require "package.perl";

use File::Basename "fileparse";
use Getopt::Long "GetOptions";

sub println { print @_; print "\n"; }

my %PKG = PKG();
my $PACKAGE_NAME = PACKAGE_NAME();
my $OUT_MAKEFILE = 'Makefile.config';

my $PACKAGE = PACKAGE();
my $OUTDIR = "$ENV{HOME}/$PKG{OUT}";
my $package_default_prefix = $PKG{PATH};
my $schema_default_dir = $PKG{SCHEMA_PATH} if ($PKG{SCHEMA_PATH});

my @REQ = REQ();

my @options = ( "clean",
                "help",
                "outputdir=s",
                "output-makefile=s",
                "prefix=s",
                "status",
                "with-debug",
                "without-debug" );
foreach my $href (@REQ) {
    my %a = %$href;
    push @options, "$a{option}=s";
}
push @options, "shemadir" if ($PKG{SCHEMA_PATH});

my %OPT;
die "configure: error" unless (GetOptions(\%OPT, @options));

if ($OPT{'help'}) {
    help();
    exit(0);
} elsif ($OPT{'clean'}) {
    foreach ('Makefile.config', 'Makefile.userconfig', 'user.status') {
        print "removing $_... ";
        if (-e $_) {
            if (unlink $_) {
                println "ok";
            } else {
                println "failed";
            }
        } else {
            println "not found";
        }
    }
    exit(0);
}
my ($filename, $directories, $suffix) = fileparse($0);
die "configure: error: $filename should be run as ./$filename"
    if ($directories ne "./");

$OPT{'prefix'} = $package_default_prefix unless ($OPT{'prefix'});

{
    my $prefix = $OPT{'prefix'};
    $OPT{eprefix} = $prefix unless ($OPT{eprefix});
    my $eprefix = $OPT{eprefix};
    $OPT{bindir} = "$eprefix/bin" unless ($OPT{bindir});
    $OPT{libdir} = "$eprefix/lib" unless ($OPT{libdir});
    $OPT{includedir} = "$eprefix/include" unless ($OPT{includedir});
}

my $AUTORUN = $OPT{'output-makefile'} || $OPT{status};

if ($AUTORUN) {
    while (1) {
        open F, "user.status" or last;
        foreach (<F>) {
            chomp;
            @_ = split /=/;
            if ($#_ == 1) {
                $OPT{$_[0]} = $_[1] unless ($OPT{$_[0]});
            }
        }
        last;
    }
}

# initial values
my $TARGDIR .= "$OUTDIR/$PACKAGE";
$TARGDIR = $OPT{'outputdir'} if ($OPT{'outputdir'});
$OUT_MAKEFILE = $OPT{'output-makefile'} if ($OPT{'output-makefile'});

my $BUILD = "rel";

# parse command line
$BUILD = $OPT{'BUILD'} if ($OPT{'BUILD'});
$BUILD = "dbg" if ($OPT{'with-debug'});
$BUILD = "rel" if ($OPT{'without-debug'});

my $BUILD_TYPE = "release";
$BUILD_TYPE = "debug" if ( $BUILD eq "dbg" );

println unless ($AUTORUN);
print "checking system type... " unless ($AUTORUN);
my $OSTYPE = `uname -s`; chomp $OSTYPE;
println $OSTYPE unless ($AUTORUN);

print "checking machine architecture... " unless ($AUTORUN);
my $MARCH = `uname -m`; chomp $MARCH;
println $MARCH unless ($AUTORUN);

my $TOOLS = "";
$TOOLS = "jdk" if ($PKG{LNG} eq 'JAVA');

print "checking ngs version... " unless ($AUTORUN);
open VFILE, "version" or die "failed to open version file - $!";
my $VERSION = <VFILE>; chomp $VERSION;
println $VERSION unless ($AUTORUN);

# determine architecture

my ($ARCH, $BITS);

print "checking for supported architecture... " unless ($AUTORUN);
if ( $MARCH =~ m/x86_64/i )
{
    $ARCH = $MARCH;
    $BITS = 64;
}
elsif ( $MARCH =~ m/i?86/i )
{
    $ARCH = "i386";
    $BITS = 32;
}
else
{
    die "unrecognized Architecture - " . $ARCH;
}
println "$MARCH ($BITS bits) is supported" unless ($AUTORUN);

# determine OS and related norms
my ($OS, $LPFX, $OBJX, $LOBX, $LIBX, $SHLX, $EXEX, $OSINC);

print "checking for supported OS... " unless ($AUTORUN);
if ( $OSTYPE =~ m/linux/i )
{
    $OS = "linux";
    $LPFX = "lib";
    $OBJX = "o";
    $LOBX = "pic.o";
    $LIBX = "a";
    $SHLX = "so";
    $EXEX = "";
    $OSINC = "unix";
    if ( $TOOLS eq "" )
    {
        $TOOLS = "gcc";
    }
}
elsif ( $OSTYPE =~ m/darwin/i )
{
    $OS = "mac";
    $LPFX = "lib";
    $OBJX = "o";
    $LOBX = "pic.o";
    $LIBX = "a";
    $SHLX = "dylib";
    $EXEX = "";
    $OSINC = "unix";
    if ( $TOOLS eq "" )
    {
        $TOOLS = "clang";
    }
}
else
{
    die "unrecognized OS - " . $OSTYPE;
}
println "$OSTYPE ($OS) is supported" unless ($AUTORUN);

# tool chain
my ($CC, $CP, $AR, $ARX, $ARLS, $LD, $LP);
my ($JAVAC, $JAVAH, $JAR);
my ($DBG, $OPT, $PIC, $INC, $MD);

print "checking for supported tool chain... " unless ($AUTORUN);
if ( $TOOLS =~ m/gcc/i )
{
    $CC = "gcc -c";
    $CP = "g++ -c";
    $AR = "ar rc";
    $ARX = "ar x";
    $ARLS = "ar t";
    $LD = "gcc";
    $LP = "g++";

    $DBG = "-g";
    $OPT = "-O3";
    $PIC = "-fPIC";
    $INC = "-I";
    $MD  = "-MD";
}
elsif ( $TOOLS =~ m/clang/i )
{
    $CC = "clang -c";
    $CP = "clang++ -c";
    $AR = "ar rc";
    $ARX = "ar x";
    $ARLS = "ar t";
    $LD = "clang";
    $LP = "clang++";

    $DBG = "-g";
    $OPT = "-O3";
    $PIC = "-fPIC";
    $INC = "-I";
    $MD  = "-MD";
}
elsif ( $TOOLS =~ m/jdk/i )
{
    $JAVAC = "javac";
    $JAVAH = "javah";
    $JAR   = "jar cf";

    $DBG = "-g";
}
else
{
    die "unrecognized tool chain - " . $TOOLS;
}
println "$TOOLS tool chain is supported" unless ($AUTORUN);

my $NGS_SDK_PREFIX;
foreach my $href (@REQ) {
    my $found;
    my %a = %$href;
    print "checking for $a{name} package... " unless ($AUTORUN);
    my $p = $OPT{$a{option}};
    if ($p) {
        print "\n\t$p " unless ($AUTORUN);
        unless (-d $p) {
            println 'no' unless ($AUTORUN);
            println "configure: error: required $a{name} package not found.";
        } else {
            println 'yes' unless ($AUTORUN);
            $found = 1;
        }
    } else {
        println 'no' unless ($AUTORUN);
        println "configure: error: required $a{name} package not found.";
    }
    unless ($found) {
       exit 1;
    }
    if ($a{name} eq 'ngs-sdk') {
        $NGS_SDK_PREFIX = $p;
    }
}

if ($PKG{NGS_SDK_SRC}) {
    print "checking for ngs-sdk package source files... " unless ($AUTORUN);
    my $p = '../ngs-sdk';
    print "$p " unless ($AUTORUN);
    unless (-d $p) {
        println 'no. skipped' unless ($AUTORUN);
    } else {
        println 'yes' unless ($AUTORUN);
        $OPT{'with-ngs-sdk-src'} = $p;
    }
}

println "NGS_SDK_PREFIX = $NGS_SDK_PREFIX" if ($DEBUG);

my @lines;

# create Makefile.config
push (@lines, "### AUTO-GENERATED FILE ###" );
push (@lines,  "" );
push (@lines,  'include $(TOP)/Makefile.userconfig' );
push (@lines,  "" );
push (@lines, "# build type");
push (@lines, "BUILD ?= $BUILD");
push (@lines,  "" );
push (@lines,  "# target OS" );
push (@lines,  "OS = " . $OS );
push (@lines,  "OSINC = " . $OSINC );
push (@lines,  "" );
push (@lines,  "# prefix string for system libraries" );
push (@lines,  "LPFX = " . $LPFX );
push (@lines,  "" );
push (@lines,  "# suffix strings for system libraries" );
push (@lines,  "LIBX = " . $LIBX . "   # ( static library )" );
push (@lines,  "SHLX = " . $SHLX . "  # ( shared library )" );
push (@lines,  "" );
push (@lines,  "# suffix strings for system object files" );
push (@lines,  "OBJX = " . $OBJX . "      # ( fixed )" );
push (@lines,  "LOBX = " . $LOBX . "  # ( position independent )" );
push (@lines,  "# suffix string for system executable" );
push (@lines,  "EXEX = " . $EXEX );
push (@lines,  "" );

push (@lines,  "# system architecture and wordsize" );
if ( $ARCH eq $MARCH )
{
    push (@lines,  "ARCH = " . $ARCH );
}
else
{
    push (@lines,  "ARCH = " . $ARCH . " # ( " . $MARCH . " )" );
}
push (@lines,  "BITS = " . $BITS );
push (@lines,  "" );

push (@lines,  "# tools" );
push (@lines,  "CC   = " . $CC ) if ($CC);
push (@lines,  "CP   = " . $CP ) if ($CP);
push (@lines,  "AR   = " . $AR ) if ($AR);
push (@lines,  "ARX  = " . $ARX ) if ($ARX);
push (@lines,  "ARLS = " . $ARLS ) if ($ARLS);
push (@lines,  "LD   = " . $LD ) if ($LD);
push (@lines,  "LP   = " . $LP ) if ($LP);
push (@lines,  "JAVAC  = " . $JAVAC ) if ($JAVAC);
push (@lines,  "JAVAH  = " . $JAVAH ) if ($JAVAH);
push (@lines,  "JAR  = " . $JAR ) if ($JAR);
push (@lines,  "" );

push (@lines,  "# tool options" );
if ( $BUILD eq "dbg" ) {
    push (@lines,  "DBG     = " . $DBG );
    push (@lines,  "OPT     = ");
} else {
    push (@lines,  "DBG     = ");
    push (@lines,  "OPT     = " . $OPT ) if ($OPT);
}
push (@lines,  "PIC     = " . $PIC ) if ($PIC);
if ($PKG{LNG} eq 'C') {
    push (@lines,  "SRCINC  = $INC. $INC\$(SRCDIR)" );
} elsif ($PKG{LNG} eq 'JAVA') {
    push (@lines,  "SRCINC  = -sourcepath \$(INCPATHS)" );
}
push (@lines,  "INCDIRS = \$(SRCINC) $INC\$(TOP)" ) if ($PIC);
push (@lines,  "CFLAGS  = \$(DBG) \$(OPT) \$(INCDIRS) $MD" )
    if ($PKG{LNG} eq 'C');
push (@lines,  "CLSPATH = -classpath \$(CLSDIR)" );
push (@lines,  "" );

# version information

my $MAJMIN;
my $MAJVERS;

if ( $VERSION =~ /(\d+)\.(\d+)\.\d+/ )
{
    $MAJMIN = $1 . "." . $2;
    $MAJVERS = $2;
}

push (@lines,  "# NGS API and library version" );
push (@lines,  "VERSION = " . $VERSION );
push (@lines,  "MAJMIN = " . $MAJMIN );
push (@lines,  "MAJVERS = " . $MAJVERS );
push (@lines,  "" );

# determine output path
if ($PKG{LNG} eq 'C') {
    $TARGDIR = $TARGDIR . "/" . $ARCH;
}
push (@lines,  "# output path" );
push (@lines,  "TARGDIR ?= " . $TARGDIR );
push (@lines,  "" );

# determine include install path
# determine library install path

# other things
push (@lines,  "# derived paths" );
push (@lines,  "MODPATH  ?= \$(subst \$(TOP)/,,\$(CURDIR))" );
push (@lines,  "SRCDIR   ?= \$(TOP)/\$(MODPATH)" );
push (@lines,  "MAKEFILE ?= \$(abspath \$(firstword \$(MAKEFILE_LIST)))" );
push (@lines,  "BINDIR    = \$(TARGDIR)/bin" );
if ($PKG{LNG} eq 'C') {
    push (@lines,  "LIBDIR    = \$(TARGDIR)/lib" );
} elsif ($PKG{LNG} eq 'JAVA') {
    push (@lines,  "LIBDIR    = \$(TARGDIR)/jar" );
}
push (@lines,  "ILIBDIR   = \$(TARGDIR)/ilib" );
push (@lines,  "OBJDIR    = \$(TARGDIR)/obj/\$(MODPATH)" );
push (@lines,  "CLSDIR    = \$(TARGDIR)/cls" );

if ($PKG{LNG} eq 'JAVA') {
    push (@lines,
        "INCPATHS = \$(SRCDIR):\$(SRCDIR)/itf:\$(TOP)/gov/nih/nlm/ncbi/ngs" );
}

push (@lines,  "" );

push (@lines,  "# exports" );
push (@lines,  "export TOP" );
push (@lines,  "export MODPATH" );
push (@lines,  "export SRCDIR" );
push (@lines,  "export MAKEFILE" );
push (@lines,  "" );

push (@lines,  "# auto-compilation rules" );
if ($PKG{LNG} eq 'C') {
    push (@lines,  "\$(OBJDIR)/%.\$(OBJX): %.c" );
    push (@lines,  "\t\$(CC) -o \$@ \$< \$(CFLAGS)" );
    push (@lines,  "\$(OBJDIR)/%.\$(LOBX): %.c" );
    push (@lines,  "\t\$(CC) -o \$@ \$< \$(PIC) \$(CFLAGS)" );
}
push (@lines,  "\$(OBJDIR)/%.\$(OBJX): %.cpp" );
push (@lines,  "\t\$(CP) -o \$@ \$< \$(CFLAGS)" );
push (@lines,  "\$(OBJDIR)/%.\$(LOBX): %.cpp" );
push (@lines,  "\t\$(CP) -o \$@ \$< \$(PIC) \$(CFLAGS)" );
push (@lines,  "" );

# this is part of Makefile
push (@lines,  "VPATH = \$(SRCDIR)" );
push (@lines,  "" );

# we know how to find jni headers
if ($PKG{LNG} eq 'JAVA' and $OPT{'with-ngs-sdk-src'}) {
    push(@lines, "JNIPATH = $OPT{'with-ngs-sdk-src'}/language/java");
}

push (@lines,  "# directory rules" );
if ($PKG{LNG} eq 'C') {
    push (@lines,  "\$(BINDIR) \$(LIBDIR) \$(ILIBDIR) \$(OBJDIR):\n"
                 . "\tmkdir -p \$@" );
} elsif ($PKG{LNG} eq 'JAVA') {
    # test if we have jni header path
    push (@lines,  "\$(LIBDIR) \$(CLSDIR):\n\tmkdir -p \$@" );
}
push (@lines,  "" );

push (@lines,  "# not real targets" );
push (@lines,  ".PHONY: default clean install all std \$(TARGETS)" );
push (@lines,  "" );

push (@lines,  "# dependencies" );
if ($PKG{LNG} eq 'C') {
    push (@lines,  "include \$(wildcard \$(OBJDIR)/*.d)" );
} elsif ($PKG{LNG} eq 'JAVA') {
    push (@lines,  "include \$(wildcard \$(CLSDIR)/*.d)" );
}

push (@lines,  "" );

unless ($OPT{'status'}) {
    println "configure: creating $OUT_MAKEFILE" unless ($AUTORUN);
    open OUT, ">$OUT_MAKEFILE" or die "cannot open $OUT_MAKEFILE to write";
    print OUT "$_\n" foreach (@lines);
    close OUT;
}

print "OPT{output-makefile} = $OPT{'output-makefile'}\n" if ($DEBUG);

unless ($AUTORUN) {
    my $OUT = 'user.status';
    println "configure: creating $OUT";
    open OUT, ">$OUT" or die "cannot open $OUT to write";
    print OUT "BUILD=$BUILD\n";
    print OUT "bindir=$OPT{bindir}\n" if ($OPT{bindir});
    print OUT "libdir=$OPT{libdir}\n" if ($OPT{libdir});
    print OUT "includedir=$OPT{includedir}\n" if ($OPT{includedir});
    foreach my $href (@REQ) {
        my %a = %$href;
        print OUT "$a{option}=$OPT{$a{option}}\n" if ($OPT{$a{option}});
    }
    close OUT;
}
unless ($AUTORUN) {
    my $OUT = 'Makefile.userconfig';
    println "configure: creating $OUT";
    open OUT, ">$OUT" or die "cannot open $OUT to write";
    print OUT "### AUTO-GENERATED FILE ###\n\n";

    print OUT "# build type\n"
            . "BUILD = $BUILD\n\n";
    if ($OPT{'outputdir'}) {
        print OUT "# output path\n"
                . "TARGDIR = $TARGDIR/$PACKAGE\n"
                . "\n";
    }
    print OUT "# required packages\n";
    print OUT "NGS_SDK_PREFIX = $NGS_SDK_PREFIX\n" if ($NGS_SDK_PREFIX);
    print OUT "\n";
    print OUT "# install paths\n";
    print OUT "INST_BINDIR = $OPT{bindir}\n" if ($OPT{bindir});
    print OUT "INST_LIBDIR = $OPT{libdir}\n" if ($OPT{libdir});
    print OUT "INST_INCLUDEDIR = $OPT{includedir}\n" if ($OPT{includedir});
    print OUT "INST_SCHEMADIR = $OPT{'shemadir'}" if ($OPT{'shemadir'});
    print OUT "\n";
    close OUT;
}

if (! $AUTORUN || $OPT{'status'}) {
    println "build type: $BUILD_TYPE";
    println "build output path: $OUTDIR";
    println "outputdir: $TARGDIR";
    println "prefix: $OPT{'prefix'}";
    println "eprefix: $OPT{'eprefix'}";
    println "libdir: $OPT{'libdir'}";
    println "includedir: $OPT{'includedir'}";
    println "schemadir: $OPT{'shemadir'}" if ($OPT{'shemadir'});
    println;
}

################################################################################

sub help {
#  --prefix=PREFIX         install architecture-independent files in PREFIX
    print <<EndText;
`configure' configures $PACKAGE_NAME to adapt to many kinds of systems.

Usage: ./configure [OPTION]...

Defaults for the options are specified in brackets.

Configuration:
  -h, --help              display this help and exit

Installation directories:
  --prefix=PREFIX         install all files in PREFIX
                          [$package_default_prefix]

EndText

    if ($PACKAGE eq 'sra-tools') {
        print <<EndText;
  --shemadir=DIR          install schema files in DIR
                          [$schema_default_dir]

EndText
    }

    print <<EndText;
By default, \`make install' will install all the files in
\`$package_default_prefix/bin', \`$package_default_prefix/lib' etc.
You can specify an installation prefix other than \`$package_default_prefix'
using \`--prefix', for instance \`--prefix=\$HOME/$PACKAGE'.

For better control, use the options below.

EndText

    if (@REQ) {
        print "Required Packages:\n";
    }

    foreach my $href (@REQ) {
        my %a = %$href;
        print <<EndText;
  --$a{option}=DIR    search for $a{name} package in DIR

EndText
    }

    print <<EndText;
Build tuning:
  --with-debug
  --without-debug
  --outputdir=DIR         generate build output into DIR directory
                          [$OUTDIR]

Miscellaneous:
  --status                print current configuration information
  --clean                 remove all configuration results

EndText
}
