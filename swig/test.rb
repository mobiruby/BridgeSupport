#!/usr/bin/ruby

require 'bridgesupportparser'
require 'getoptlong'

arch = 'x86_64'

ARGV.map! {|x| x.sub(/^-(?:arch|framework|isysroot)$/, "-\\&")}
opts = GetoptLong.new(
    ['--arch', GetoptLong::REQUIRED_ARGUMENT],
    ['-D', GetoptLong::REQUIRED_ARGUMENT],
    ['--framework', GetoptLong::REQUIRED_ARGUMENT],
    ['-I', GetoptLong::REQUIRED_ARGUMENT],
    ['--isysroot', GetoptLong::REQUIRED_ARGUMENT],
    ['-v', '--verbose', GetoptLong::NO_ARGUMENT]
);
$defines = []
$incdirs = []
$incfws = []
$sysroot = '/'
$verbose = false
opts.each do |opt, arg|
    case opt
    when '--arch': arch = arg
    when '-D': $defines << arg
    when '--framework': $incfws << arg
    when '-I': $incdirs << arg
    when '--isysroot': $sysroot = arg
    when '-v': $verbose = true
    end
end
raise ArgumentError, "No file specified" if ARGV.length == 0
raise ArgumentError, "Only one file may be specified" if ARGV.length > 1
path = ARGV[0]
triple = "#{arch}-apple-darwin11.0"
puts triple #DEBUG

def recursivefunc(f, indent = 0)
    i = '    ' * indent
    func, rettype, retenc, retattrs, retfunc, fattrs, variadic, inline = f.info
    puts "#{i}retval: \"#{func}\" (#{rettype}) \"#{retenc}\" retattrs=#{retattrs.inspect} retfunc=#{!retfunc.nil?} fattrs=#{fattrs.inspect} variadic=#{variadic} inline=#{inline}"
    recursivefunc(retfunc, indent + 1) if !retfunc.nil?
    n = 0
    f.each_argument do |name, type, enc, attrs, funcptr|
	puts "#{i}arg#{n}: \"#{name}\" (#{type}) \"#{enc}\" attrs=#{attrs.inspect} funcptr=#{!funcptr.nil?}"
	recursivefunc(funcptr, indent + 1) if !funcptr.nil?
	n += 1
    end
end

def method(m)
    mname, rettype, retenc, retattrs, retfunc, mattrs, classmeth, variadic = m.info
    puts "    \"#{mname}\" (#{rettype}) \"#{retenc}\" retattrs=#{retattrs.inspect} retfunc=#{!retfunc.nil?} mattrs=#{mattrs.inspect} classmeth=#{classmeth} variadic=#{variadic}"
    recursivefunc(retfunc, 1) if !retfunc.nil?
    n = 0
    m.each_argument do |name, type, enc, mod, attrs, funcptr|
	puts "      arg#{n}: \"#{name}\" (#{type}) \"#{enc}\" mod=#{mod} attrs=#{attrs.inspect} funcptr=#{!funcptr.nil?}"
	recursivefunc(funcptr, 1) if !funcptr.nil?
    end
end

Bridgesupportparser::BridgeSupportParser.parse(path, triple, $defines, $incdirs, $incfws, $sysroot, $verbose) do |top|
    next if top.path != path
    case top
    when Bridgesupportparser::AnEnum
	enum = top.info
	puts "AnEnum: " + enum
	top.each_value do |name, val|
	    puts "    #{name}: #{val}"
	end
    when Bridgesupportparser::AFunction
	func, rettype, retenc, retattrs, retfunc, fattrs, variadic, inline = top.info
	puts "AFunction: \"#{func}\" (#{rettype}) \"#{retenc}\" retattrs=#{retattrs.inspect} retfunc=#{!retfunc.nil?} fattrs=#{fattrs.inspect} variadic=#{variadic} inline=#{inline}"
	recursivefunc(retfunc) if !retfunc.nil?
	n = 0
	top.each_argument do |name, type, enc, attrs, funcptr|
	    puts "    arg#{n}: \"#{name}\" (#{type}) \"#{enc}\" attrs=#{attrs.inspect} funcptr=#{!funcptr.nil?}"
	    recursivefunc(funcptr, 2) if !funcptr.nil?
	    n += 1
	end
    when Bridgesupportparser::AMacroFunctionAlias
	name, val = top.info
	puts "AMacroFunctionAlias: #{name} #{val}"
    when Bridgesupportparser::AMacroNumber
	name, val = top.info
	puts "AMacroNumber: #{name} #{val}"
    when Bridgesupportparser::AMacroString
	name, val, objcstr = top.info
	puts "AMacroString: #{name} #{val} objcstr=#{objcstr}"
    when Bridgesupportparser::AnObjCCategory
	klass, cname = top.info
	print "AnObjCCategory: #{klass} (#{cname}) protocols:"
	top.each_protocol do |pname|
	    print " #{pname}"
	end
	print "\n"
	top.each_method do |m|
	    method(m)
	end
    when Bridgesupportparser::AnObjCInterface
	iname = top.info
	print "AnObjCInterface: #{iname} protocols:"
	top.each_protocol do |pname|
	    print " #{pname}"
	end
	print "\n"
	top.each_method do |m|
	    method(m)
	end
    when Bridgesupportparser::AnObjCProtocol
	pname = top.info
	print "AnObjCProtocol: #{pname} protocols:"
	top.each_protocol do |pname|
	    print " #{pname}"
	end
	print "\n"
	top.each_method do |m|
	    method(m)
	end
    when Bridgesupportparser::AStruct
	sname, senc = top.info
	puts "AStruct: #{sname} \"#{senc}\""
	top.each_field do |name, type, enc, attr, funcptr|
	    puts "    \"#{name}\" (#{type}) \"#{enc}\" attr=#{attr.inspect} funcptr=#{!funcptr.nil?}"
	    recursivefunc(funcptr, 2) if !funcptr.nil?
	end
    when Bridgesupportparser::ATypedef
	tname, ttype, attrs = top.info
	puts "ATypedef: #{tname} (#{ttype}) attrs=#{attrs.inspect}"
	top.walk_types do |name, type, decl, attrs|
	    puts "    \"#{name}\" #{type} #{decl.inspect()} attrs=#{attrs.inspect}"
	end
    when Bridgesupportparser::AVar
	vname, vtype, venc, attr, funcptr = top.info
	puts "AVar: \"#{vname}\" (#{vtype}) \"#{venc}\" attr=#{attr.inspect} funcptr=#{!funcptr.nil?}"
	recursivefunc(funcptr, 1) if !funcptr.nil?
    end
end
