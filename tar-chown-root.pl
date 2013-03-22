#!/usr/bin/perl

# This perl script tars up the contents of the directory given as the first
# argument, and modifies user/group information as if the files were created
# by root/wheel.  For directories that already exist in root, the
# mode/owner/group information is duplicated.  The tarball is writen to the
# standard output.

use strict;
use Archive::Tar;
use Archive::Tar::File;
use File::Basename ();
use File::Find ();
use File::Spec;
use File::stat ();
use IO::Handle;

my $MyName = File::Basename::basename($0);
die "Usage: $MyName directory\n" unless scalar(@ARGV) == 1 && -d $ARGV[0];

chdir $ARGV[0] or die "$MyName: Can't chdir $ARGV[0]: $!\n";

my @files;
File::Find::find(sub { push @files, $File::Find::name }, <*>);

my $tar = Archive::Tar->new();
$tar->add_files(@files);

my $st;
for($tar->get_files()) {
    if($_->is_dir && defined($st = File::stat::stat(File::Spec->join('/', $_->full_path)))) {
	$_->mode($st->mode);
	$_->uname(scalar(getpwuid($st->uid)));
	$_->uid($st->uid);
	$_->gname(scalar(getgrgid($st->gid)));
	$_->gid($st->gid);
    } else {
	$_->uname('root');
	$_->uid(0); # root
	$_->gname('wheel');
	$_->gid(0); # wheel
    }
    #printf "%s: 0%o %s %d %s %d\n", $_->full_path, $_->mode, $_->uname, $_->uid, $_->gname, $_->gid;
}

my $stdout = IO::Handle->new();
$stdout->fdopen(fileno(STDOUT), "w") or die "$MyName: Can't create stdout filehandle\n";
$tar->write($stdout);
