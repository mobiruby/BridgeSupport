#!/usr/bin/ruby

$slb = '/System/Library/BridgeSupport'
_slbr_ = "#{$slb}/ruby-#{RUBY_VERSION.sub(/^(\d+\.\d+)(\..*)?$/, '\1')}"
$:.unshift(_slbr_) unless $:.any? do |p|
    if %r{#{_slbr_}$} =~ p
	$slb = File.dirname(p)
	true
    end
end
require 'bridgesupportparser'
require 'getoptlong'

arch = 'i386'
arch64 = 'x86_64'

ARGV.map! {|x| x.sub(/^-(?:arch|framework|isysroot|64)$/, "-\\&")}
opts = GetoptLong.new(
    ['--64', GetoptLong::NO_ARGUMENT],
    #['--arch', GetoptLong::REQUIRED_ARGUMENT],
    ['-D', GetoptLong::REQUIRED_ARGUMENT],
    ['--framework', GetoptLong::REQUIRED_ARGUMENT],
    ['-I', GetoptLong::REQUIRED_ARGUMENT],
    ['--isysroot', GetoptLong::REQUIRED_ARGUMENT],
    ['-v', '--verbose', GetoptLong::NO_ARGUMENT]
);
$defines = []
$do64 = false
$incdirs = ['.']
$incfws = []
$sysroot = '/'
$VERBOSE = false
opts.each do |opt, arg|
    case opt
    when '--64': $do64 = true
    #when '--arch': arch = arg
    when '-D': $defines << arg
    when '--framework': $incfws << arg
    when '-I': $incdirs << arg
    when '--isysroot': $sysroot = arg
    when '-v': $VERBOSE = $DEBUG = true
    end
end
raise ArgumentError, "No file specified" if ARGV.length == 0

meths = [
    '-(id)action;',
    '-(int)aaa:(int)a bbb:(long)b;',
    '-(int)mmm:(BOOL)m;',
    '-(int)mmn:(bool)m;',
    '-(void)xxx:(void *)x yyy:(void **)y;',
]
types = [
    'int *',
    'long *',
    'double',
    '_Bool',
    'bool',
    'Boolean',
]

$incdirs.unshift("#{$slb}/include")
$incfws.unshift('/System/Library/Frameworks/CoreServices.framework/Frameworks')
p = ARGV.clone
ARGV.unshift('AvailabilityMacros.h')
ARGV.unshift('BSMacTypes.h')

parser = Bridgesupportparser::Parser.new(ARGV, p, meths, types, $defines, $incdirs, $incfws, $sysroot)
parser.parse(arch)
if $do64
    parser64 = Bridgesupportparser::Parser.new(ARGV, p, meths, types, $defines, $incdirs, $incfws, $sysroot)
    parser64.parse(arch64)
    parser.mergeWith64!(parser64)
end
parser.writeXML($stdout, '1.0')
