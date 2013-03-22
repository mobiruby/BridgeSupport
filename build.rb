require 'gen_bridge_metadata'
require 'pathname'
require 'fileutils'
require 'digest/sha1'
require 'stringio'
require 'thread'

include FileUtils

SLF = '/System/Library/Frameworks'
SLPF = '/System/Library/PrivateFrameworks'

STDOUT.sync = true
WORKNARGS = 5

def measure(something)
  elapsed = Time.now
  yield
  $stderr.puts "    #{something} (#{Time.now - elapsed} seconds)"
end

def work(fname, path, is_private, out_dir, out_file)
  is_private = (is_private == 'true')
  # We have work!
  $stderr.puts "Generating BridgeSupport metadata for: #{fname} ..."
  elapsed = Time.now
  
  # Create a new generator object, configure it accordingly for a first 32-bit pass.
  gen = BridgeSupportGenerator.new
  gen.frameworks << path
  gen.emulate_ppc = false # PPC is not supported anymore since SnowLeopard

  exceptions = "exceptions-#{RUBY_PLATFORM}/#{fname}.xml"
  if File.exist?(exceptions)
    gen.exception_paths << exceptions
  else
    exceptions = "exceptions/#{fname}.xml"
    if File.exist?(exceptions)
      gen.exception_paths << exceptions
    end
  end

  gen.private = true if is_private 
 
  measure('Parse 32 and 64-bit') { gen.parse(true) }
  measure('Write final metadata') do
    mkdir_p(out_dir)
    gen.out_file = out_file
    gen.write
  end

  # Validate.
  measure('Validate XML') do
    unless system("xmllint --dtdvalid ./BridgeSupport.dtd --noout #{out_file}")
      $stderr.puts "Error: `#{out_file}' doesn't validate against BridgeSupport.dtd"
      exit 1
    end
  end

  # Generate inline dynamic library if required.
  if gen.has_inline_functions?
    measure('Generate dylib file') do
      gen.generate_format = BridgeSupportGenerator::FORMAT_DYLIB
      gen.out_file = "#{out_dir}/#{fname}.dylib"
      gen.write
    end
  end
  gen.cleanup

  $stderr.puts "Done (#{Time.now - elapsed} seconds)."
end

if ARGV.length == (WORKNARGS + 1) && ARGV[0] == '-'
  work(*ARGV[1..-1])
  exit
end

puts "Environment is: #{ENV.inspect}"

frameworks = {}
Dir.glob(File.join(SLF, '**', '*.framework')).each do |path|
  name = File.basename(path, '.framework')
  # Ignore frameworks that do not respect basic layout.
  if File.exist?(File.join(path, name)) and File.exist?(File.join(path, 'Headers'))
    frameworks[name] = path
  end
end

# The generator currently fails on these frameworks:
#
# IOKit - architecture-dependent headers (like arm) fail to parse
# MacRuby - requires -fobjc-gc-only and -lauto
# QuickTime - deprecated and 32-bit only
# vecLib - clang-110 abort when objc type encoding of vector types

ignore_list = %w{IOKit MacRuby QuickTime vecLib}
ignore_list.each { |x| frameworks.delete(x) }

also_gen_private_metadata = []

# If '--update-exceptions' is passed to the script, do a pass on all
# exception files and generate new templates.
if ARGV.include?('--update-exceptions')
  $stderr.puts "Updating exceptions..."
  frameworks.sort.each do |fname, path|
    exception = "exceptions/#{fname}.xml"
    next unless File.exist?(exception)
    out_file = exception + '.new'
    next if File.exist?(out_file)
    $stderr.puts "Updating #{exception}"
    gen = BridgeSupportGenerator.new
    gen.frameworks << path
    gen.exception_paths = exception
    gen.generate_format = BridgeSupportGenerator::FORMAT_TEMPLATE
    gen.private = path.include?(SLPF)
    gen.out_file = out_file
    gen.collect
    gen.write
    system("xmllint -format #{out_file} > #{exception}")
    rm out_file
  end
  $stderr.puts "Done."
  exit 0
end

frameworks.delete_if { |fname, path| !ARGV.include?(fname) } unless ARGV.empty?

dstroot = (ENV['DSTROOT'] or '')
$workq = []

frameworks.sort { |ary, ary2|
  # Sort the frameworks by dependency order, using a naive-but-working algorithm.
  deps = [ary, ary2].map { |a| 
    BridgeSupportGenerator.dependencies_of_framework(a[1]).map { |fpath|
      BridgeSupportGenerator.dependencies_of_framework(fpath)
    }.flatten
  }
  len1 = deps[0].reject { |x| deps[1].include?(x) }.length
  len2 = deps[1].reject { |x| deps[0].include?(x) }.length
  len1 <=> len2
}.each do |fname, path|
  dir = Pathname.new("#{path}/Versions/Current/Resources").realpath.to_s + '/BridgeSupport'
  is_private = also_gen_private_metadata.include?(fname)
  file = "#{dir}/#{fname}#{is_private ? 'Private' : ''}.bridgesupport"

  # Check if the bridge support file isn't already in the DSTROOT. 
  out_dir = File.join(dstroot, dir)
  out_file = File.join(dstroot, file)
  next if File.exist?(out_file) and File.size(out_file) > 0
  $workq << [fname, path, is_private, out_dir, out_file]
end

$workmutex = Mutex.new
$stdoutmutex = Mutex.new

def run
  work = ''
  loop do
    $workmutex.synchronize { work = $workq.shift }
    break if work.nil?
    StringIO.open do |sio|
      IO.popen( "ruby #{$0} - '#{work.join("' '")}' 2>&1", 'r' ) do |io|
	begin
	  io.each { |line| sio << line }
	rescue Errno::EIO
	end
      end
      #check exit status
      raise "exit status=#{$?.exitstatus}" unless $?.success?
      $stdoutmutex.synchronize { STDOUT.write(sio.string) }
    end
  end
end

Thread.abort_on_exception = true
ncpu = `sysctl -n hw.ncpu`.chomp.to_i
raise "ncpu = #{ncpu}" unless ncpu > 0
puts "ncpu = #{ncpu}"

threads = []
ncpu.times { |n| threads << Thread.new { run } }
ncpu.times { |n| threads[n].join }
