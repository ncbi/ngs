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

require 'package.perl';
require 'os-arch.pm';

use Cwd qw (abs_path getcwd);
use File::Basename 'fileparse';
use File::Spec 'catdir';
use FindBin qw($Bin);
use Getopt::Long 'GetOptions';

sub println { print @_; print "\n"; }

my %PKG = PKG();
my $PACKAGE_NAME = PACKAGE_NAME();
my $OUT_MAKEFILE = 'Makefile.config';

my $PACKAGE = PACKAGE();

my $HOME;
if ($ENV{HOME}) {
    $HOME = $ENV{HOME};
} elsif ($ENV{USERPROFILE}) {
    $HOME = $ENV{USERPROFILE};
} else {
    $HOME = getcwd;
}
$HOME = abs_path('.') unless($HOME);

my $OUTDIR = File::Spec->catdir($HOME, $PKG{OUT});

my $package_default_prefix = $PKG{PATH};
my $schema_default_dir = $PKG{SCHEMA_PATH} if ($PKG{SCHEMA_PATH});

my @REQ = REQ();

my @options = ( "arch=s",
                "clean",
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
    foreach ('Makefile.userconfig', 'user.status', glob('Makefile.config*')) {
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

my $AUTORUN = $OPT{'output-makefile'} || $OPT{status};

print "checking system type... " unless ($AUTORUN);
my ($OS, $ARCH, $OSTYPE, $MARCH, @ARCHITECTURES) = OsArch();
println $OSTYPE unless ($AUTORUN);

{
    my $prefix = $OPT{'prefix'};
    $OPT{eprefix} = $prefix unless ($OPT{eprefix} || $OS eq 'win');
    my $eprefix = $OPT{eprefix};
    unless ($OPT{bindir} || $OS eq 'win') {
        $OPT{bindir} = File::Spec->catdir($eprefix, 'bin') ;
    }
    unless ($OPT{libdir} || $OS eq 'win') {
        $OPT{libdir} = File::Spec->catdir($eprefix, 'lib');
    }
    unless ($OPT{includedir} || $OS eq 'win') {
        $OPT{includedir} = File::Spec->catdir($eprefix, 'include');
    }
}

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
my $TARGDIR = File::Spec->catdir($OUTDIR, $PACKAGE);
$TARGDIR = $OPT{'outputdir'} if ($OPT{'outputdir'});
$OUT_MAKEFILE = $OPT{'output-makefile'} if ($OPT{'output-makefile'});

my $BUILD = "rel";

# parse command line
$BUILD = $OPT{'BUILD'} if ($OPT{'BUILD'});
$BUILD = "dbg" if ($OPT{'with-debug'});
$BUILD = "rel" if ($OPT{'without-debug'});

my $BUILD_TYPE = "release";
$BUILD_TYPE = "debug" if ( $BUILD eq "dbg" );

#println unless ($AUTORUN);

if ($OPT{arch}) {
    my $found;
    foreach (@ARCHITECTURES) {
        if ($_ eq $OPT{arch}) {
            ++$found;
            last;
        }
    }
    if ($found) {
        $ARCH = $MARCH = $OPT{arch};
    } else {
        delete $OPT{arch};
    }
}

$OUT_MAKEFILE .= ".$OS.$ARCH";

#my $OSTYPE = `uname -s`; chomp $OSTYPE;

print "checking machine architecture... " unless ($AUTORUN);
#my $MARCH = `uname -m`; chomp $MARCH;
println $MARCH unless ($AUTORUN);

my $TOOLS = "";
$TOOLS = "jdk" if ($PKG{LNG} eq 'JAVA');

print "checking ngs version... " unless ($AUTORUN);
open VFILE, "version" or die "failed to open version file - $!";
my $VERSION = <VFILE>; chomp $VERSION;
println $VERSION unless ($AUTORUN);

# determine architecture

print "checking for supported architecture... " unless ($AUTORUN);

my $BITS;

if ( $MARCH =~ m/x86_64/i )
{
    $BITS = 64;
}
elsif ( $MARCH =~ m/i?86/i )
{
    $BITS = 32;
}
else
{
    die "unrecognized Architecture - " . $ARCH;
}
println "$MARCH ($BITS bits) is supported" unless ($AUTORUN);

# determine OS and related norms
my ($LPFX, $OBJX, $LOBX, $LIBX, $SHLX, $EXEX, $OSINC);

print "checking for supported OS... " unless ($AUTORUN);
if ( $OSTYPE =~ m/linux/i )
{
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
} elsif ($OSTYPE eq 'win') {
    $TOOLS = "vc++";
} else
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
} elsif ($TOOLS eq 'vc++') {
} else
{
    die "unrecognized tool chain - " . $TOOLS;
}
println "$TOOLS tool chain is supported" unless ($AUTORUN);

my $NGS_SDK_PREFIX;
foreach my $href (@REQ) {
    my $found;
    my %a = %$href;
    my ($name, $option) = ( $a{name}, $a{option} );
    my $msg = "checking for $name package... ";
    unless ($AUTORUN) {
        print($msg);
    }
    my $p = $OPT{$option};
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
        print "$a{srcpath} " unless ($AUTORUN);
        if (-e $a{srcpath}) {
            println 'yes' unless ($AUTORUN);
            $p = $a{srcpath};
            $found = 1;
        } else {
            println 'no' unless ($AUTORUN);
        }
    }
    unless ($found) {
        println "configure: error: required $a{name} package not found.";
        exit 1;
    } else {
        $p = abs_path($p);
    }
    if ($name eq 'ngs-sdk') {
        $NGS_SDK_PREFIX = $p;
    }
}

=pod
if ($PKG{NGS_SDK_SRC}) {
    print "checking for ngs-sdk package source files... " unless ($AUTORUN);
    my $p = File::Spec->catdir('..', 'ngs-sdk');
    print "$p " unless ($AUTORUN);
    unless (-d $p) {
        println 'no. skipped' unless ($AUTORUN);
    } else {
        println 'yes' unless ($AUTORUN);
        $OPT{'with-ngs-sdk-src'} = $p;
    }
}
=cut

println "NGS_SDK_PREFIX = $NGS_SDK_PREFIX" if ($DEBUG);

my @config;
my @c_arch;

if ($OS ne 'win') {
    $TARGDIR = File::Spec->catdir($TARGDIR, $OS, $TOOLS, $ARCH, $BUILD);
}

if ($OS ne 'win') {
    # create Makefile.config
    push (@config, "### AUTO-GENERATED FILE ###" );
    push (@c_arch, "### AUTO-GENERATED FILE ###" );
    push (@config,  "" );
    push (@c_arch,  "" );
    push (@config,  'OS_ARCH = $(shell perl $(TOP)/os-arch.perl)' );
    push (@config,  'include $(TOP)/Makefile.userconfig' );
    push (@config,  'include $(TOP)/Makefile.config.$(OS_ARCH)' );
    push (@config,  "" );
    push (@config, "# build type");
    push (@config, "BUILD ?= $BUILD");
    push (@config,  "" );
    push (@c_arch,  "# target OS" );
    push (@c_arch,  "OS = " . $OS );
    push (@c_arch,  "OSINC = " . $OSINC );
    push (@c_arch,  "" );
    push (@c_arch,  "# prefix string for system libraries" );
    push (@c_arch,  "LPFX = " . $LPFX );
    push (@c_arch,  "" );
    push (@c_arch,  "# suffix strings for system libraries" );
    push (@c_arch,  "LIBX = " . $LIBX . "   # ( static library )" );
    push (@c_arch,  "SHLX = " . $SHLX . "  # ( shared library )" );
    push (@c_arch,  "" );
    push (@c_arch,  "# suffix strings for system object files" );
    push (@c_arch,  "OBJX = " . $OBJX . "      # ( fixed )" );
    push (@c_arch,  "LOBX = " . $LOBX . "  # ( position independent )" );
    push (@c_arch,  "# suffix string for system executable" );
    push (@c_arch,  "EXEX = " . $EXEX );
    push (@c_arch,  "" );

    push (@c_arch,  "# system architecture and wordsize" );
    if ( $ARCH eq $MARCH ) {
        push (@c_arch,  "ARCH = " . $ARCH );
    } else {
        push (@c_arch,  "ARCH = " . $ARCH . " # ( " . $MARCH . " )" );
    }
    push (@c_arch,  "BITS = " . $BITS );
    push (@c_arch,  "" );

    push (@config,  "# tools" );
    push (@c_arch,  "# tools" );
    push (@c_arch,  "CC   = " . $CC ) if ($CC);
    push (@c_arch,  "CP   = " . $CP ) if ($CP);
    push (@c_arch,  "AR   = " . $AR ) if ($AR);
    push (@c_arch,  "ARX  = " . $ARX ) if ($ARX);
    push (@c_arch,  "ARLS = " . $ARLS ) if ($ARLS);
    push (@c_arch,  "LD   = " . $LD ) if ($LD);
    push (@c_arch,  "LP   = " . $LP ) if ($LP);
    push (@config,  "JAVAC  = " . $JAVAC ) if ($JAVAC);
    push (@config,  "JAVAH  = " . $JAVAH ) if ($JAVAH);
    push (@config,  "JAR  = " . $JAR ) if ($JAR);
    push (@config,  "" );
    push (@c_arch,  "" );

    push (@config,  "# tool options" );
    push (@c_arch,  "# tool options" );
    if ( $BUILD eq "dbg" ) {
        push (@c_arch,  "DBG     = " . $DBG );
        push (@c_arch,  "OPT     = ");
    } else {
        push (@c_arch,  "DBG     = ");
        push (@c_arch,  "OPT     = " . $OPT ) if ($OPT);
    }
    push (@c_arch,  "PIC     = " . $PIC ) if ($PIC);
    if ($PKG{LNG} eq 'C') {
        push (@c_arch,  "SRCINC  = $INC. $INC\$(SRCDIR)" );
    } elsif ($PKG{LNG} eq 'JAVA') {
        push (@config,  "SRCINC  = -sourcepath \$(INCPATHS)" );
    }
    push (@c_arch,  "INCDIRS = \$(SRCINC) $INC\$(TOP)" ) if ($PIC);
    if ($PKG{LNG} eq 'C') {
        push (@c_arch,  "CFLAGS  = \$(DBG) \$(OPT) \$(INCDIRS) $MD" );
    }
    push (@config,  "CLSPATH = -classpath \$(CLSDIR)" );
    push (@config,  "" );
    push (@c_arch,  "" );

    # version information

    my $MAJMIN;
    my $MAJVERS;

    if ( $VERSION =~ /(\d+)\.(\d+)\.\d+/ )
    {
        $MAJMIN = $1 . "." . $2;
        $MAJVERS = $2;
    }

    push (@config,  "# NGS API and library version" );
    push (@config,  "VERSION = " . $VERSION );
    push (@config,  "MAJMIN = " . $MAJMIN );
    push (@config,  "MAJVERS = " . $MAJVERS );
    push (@config,  "" );

    # determine output path
    if ($PKG{LNG} eq 'C') {
    #    $TARGDIR = $TARGDIR . "/" . $ARCH;
    }
    push (@c_arch,  "# output path" );
    push (@c_arch,  "TARGDIR ?= " . $TARGDIR );
    push (@c_arch,  "" );

    # determine include install path
    # determine library install path

    # other things
    push (@config,  "# derived paths" );
    push (@config,  "MODPATH  ?= \$(subst \$(TOP)/,,\$(CURDIR))" );
    push (@config,  "SRCDIR   ?= \$(TOP)/\$(MODPATH)" );
    push (@config,  "MAKEFILE ?= \$(abspath \$(firstword \$(MAKEFILE_LIST)))" );
    push (@config,  "BINDIR    = \$(TARGDIR)/bin" );
    if ($PKG{LNG} eq 'C') {
        push (@config,  "LIBDIR    = \$(TARGDIR)/lib" );
    } elsif ($PKG{LNG} eq 'JAVA') {
        push (@config,  "LIBDIR    = \$(TARGDIR)/jar" );
    }
    push (@config,  "ILIBDIR   = \$(TARGDIR)/ilib" );
    push (@config,  "OBJDIR    = \$(TARGDIR)/obj/\$(MODPATH)" );
    push (@config,  "CLSDIR    = \$(TARGDIR)/cls" );

    if ($PKG{LNG} eq 'JAVA') {
        push (@config, "INCPATHS = "
            . "\$(SRCDIR):\$(SRCDIR)/itf:\$(TOP)/gov/nih/nlm/ncbi/ngs" );
    }

    push (@config,  "" );

    push (@config,  "# exports" );
    push (@config,  "export TOP" );
    push (@config,  "export MODPATH" );
    push (@config,  "export SRCDIR" );
    push (@config,  "export MAKEFILE" );
    push (@config,  "" );

    push (@config,  "# auto-compilation rules" );
    if ($PKG{LNG} eq 'C') {
        push (@config,  "\$(OBJDIR)/%.\$(OBJX): %.c" );
        push (@config,  "\t\$(CC) -o \$@ \$< \$(CFLAGS)" );
        push (@config,  "\$(OBJDIR)/%.\$(LOBX): %.c" );
        push (@config,  "\t\$(CC) -o \$@ \$< \$(PIC) \$(CFLAGS)" );
    }
    push (@config,  "\$(OBJDIR)/%.\$(OBJX): %.cpp" );
    push (@config,  "\t\$(CP) -o \$@ \$< \$(CFLAGS)" );
    push (@config,  "\$(OBJDIR)/%.\$(LOBX): %.cpp" );
    push (@config,  "\t\$(CP) -o \$@ \$< \$(PIC) \$(CFLAGS)" );
    push (@config,  "" );

    # this is part of Makefile
    push (@config,  "VPATH = \$(SRCDIR)" );
    push (@config,  "" );

    # we know how to find jni headers
    if ($PKG{LNG} eq 'JAVA' and $OPT{'with-ngs-sdk-src'}) {
        push(@config, "JNIPATH = $OPT{'with-ngs-sdk-src'}/language/java");
    }

    push (@config,  "# directory rules" );
    if ($PKG{LNG} eq 'C') {
        push (@config,  "\$(BINDIR) \$(LIBDIR) \$(ILIBDIR) \$(OBJDIR):\n"
                     . "\tmkdir -p \$@" );
    } elsif ($PKG{LNG} eq 'JAVA') {
        # test if we have jni header path
        push (@config,  "\$(LIBDIR) \$(CLSDIR):\n\tmkdir -p \$@" );
    }
    push (@config,  "" );

    push (@config,  "# not real targets" );
    push (@config,  ".PHONY: default clean install all std \$(TARGETS)" );
    push (@config,  "" );

    push (@config,  "# dependencies" );
    if ($PKG{LNG} eq 'C') {
        push (@config,  "include \$(wildcard \$(OBJDIR)/*.d)" );
    } elsif ($PKG{LNG} eq 'JAVA') {
        push (@config,  "include \$(wildcard \$(CLSDIR)/*.d)" );
    }

    push (@config,  "" );
}

if (! $OPT{'status'} ) {
    if ($OS eq 'win') {
        my $OUT = 'Makefile.config.win';
        println "configure: creating '$OUT'";
        open OUT, ">$OUT" or die "cannot open $OUT to write";
        my $name = PACKAGE_NAMW();
        my $outdir = $name . '_OUTDIR';
        my $root = $name . '_ROOT';

        print OUT <<EndText;
<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003">  
  <PropertyGroup Label="Globals">
    <$outdir>$TARGDIR/\</$outdir>
EndText
        if ($NGS_SDK_PREFIX) {
            foreach my $href (@REQ) {
                my %a = %$href;
                if ($a{name} eq 'ngs-sdk') {
                    my $root = "$a{namw}_ROOT";
                    print OUT "    <$root>$NGS_SDK_PREFIX\/</$root>\n";
                    last;
                }
            }
        }
        print OUT <<EndText;
    <$root>$Bin/\</$root>
  </PropertyGroup>
</Project>
EndText
        close OUT;
    } else {
        println "configure: creating 'Makefile.config'" unless ($AUTORUN);
        my $out = 'Makefile.config';
        open OUT, ">$out" or die "cannot open $out to write";
        print OUT "$_\n" foreach (@config);
        close OUT;

        println "configure: creating '$OUT_MAKEFILE'" unless ($AUTORUN);
        open OUT, ">$OUT_MAKEFILE" or die "cannot open $OUT_MAKEFILE to write";
        print OUT "$_\n" foreach (@c_arch);
        close OUT;
    }
}

print "OPT{output-makefile} = $OPT{'output-makefile'}\n" if ($DEBUG);

unless ($AUTORUN) {
    my $OUT = 'user.status';
    println "configure: creating '$OUT'";
    open OUT, ">$OUT" or die "cannot open $OUT to write";
    print OUT "arch=$OPT{arch}\n" if ($OPT{arch});
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
unless ($AUTORUN || $OS eq 'win') {
    my $OUT = 'Makefile.userconfig';
    println "configure: creating '$OUT'";
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

    print "prefix: ";
    print $OPT{'prefix'} if ($OS ne 'win');
    println;

    print "eprefix: ";
    print $OPT{'eprefix'} if ($OPT{'eprefix'});
    println;

    print "includedir: ";
    print $OPT{'includedir'} if ($OPT{'includedir'});
    println;

    print "bindir: ";
    print $OPT{'bindir'} if ($OPT{'bindir'});
    println;

    print "libdir: ";
    print $OPT{'libdir'} if ($OPT{'libdir'});
    println;

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
  --arch=name             specify the name of the target architecture
  --outputdir=DIR         generate build output into DIR directory
                          [$OUTDIR]

Miscellaneous:
  --status                print current configuration information
  --clean                 remove all configuration results

EndText
}
