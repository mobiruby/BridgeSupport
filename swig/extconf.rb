require 'rbconfig'
require 'mkmf'

CLANGHOME = '/Volumes/XDisk/build/clang-59/clang'
LLVMCONFIG = ['/Developer/usr/local/bin/llvm-config', '/usr/local/bin/llvm-config'].find(lambda{'llvm-config'}) {|f| File.exists?(f)}
MYHOME = '/Volumes/XDisk/tmp/CLANG'
ARCHFLAGS = (ENV['ARCHFLAGS'].nil? || ENV['ARCHFLAGS'].length == 0) ? '-arch i386 -arch ppc -arch x86_64' : ENV['ARCHFLAGS']
DEBUGFLAGS = '-g'
OPTIMIZEFLAGS = ENV['RC_XBS'] == 'YES' ? '-Os' : ''
CXX = 'llvm-g++-4.2'

extension = 'bridgesupportparser'

[
    'clangCodeGen',
    'clangAnalysis',
    'clangRewrite',
    'clangSema',
    'clangFrontend',
    'clangDriver',
    'clangAST',
    'clangParse',
    'clangLex',
    'clangBasic',
    'LLVMCore',
    'LLVMSupport',
    'LLVMSystem',
    'LLVMBitWriter',
    'LLVMBitReader',
    'LLVMCodeGen',
    'LLVMAnalysis',
    'LLVMTarget',
    'LLVMMC',
    'ffi',
].reverse.each {|l| $libs = append_library($libs, l)}

Config::MAKEFILE_CONFIG.each_value {|v| v.sub!(/^(cc|gcc)/, CXX)}
Config::MAKEFILE_CONFIG['CC'] << "\nCXX = $(CC)"
with_cppflags("-I#{CLANGHOME}/include -I#{CLANGHOME}/tools/clang/include -I#{CLANGHOME}/tools/clang/lib -I#{MYHOME}/include #{`#{LLVMCONFIG} --cppflags`}") {true}
with_cflags("#{DEBUGFLAGS} #{OPTIMIZEFLAGS} #{ARCHFLAGS} -DDISABLE_SMART_POINTERS -fno-rtti #{`#{LLVMCONFIG} --cxxflags`.gsub(/ *-[DO][^ ]*/, '')}") {true}
with_ldflags("#{DEBUGFLAGS} #{OPTIMIZEFLAGS} #{ARCHFLAGS} -L#{MYHOME}/lib #{`#{LLVMCONFIG} --ldflags`.gsub(/ *-[DO][^ ]*/, '')}") {true}

$srcs = "#{extension}.cpp #{extension}_wrap.cpp"
$objs = ["#{extension}.o", "#{extension}_wrap.o"]
$cleanfiles << "#{extension}_wrap.cpp"

create_makefile(extension)

open("Makefile", "a") do |mf|
    mf.puts <<EOF

BSP_HEADERS = classes.h #{extension}.h __xattr__.h
$(OBJS): $(BSP_HEADERS)

#{extension}_wrap.cpp: $(BSP_HEADERS)
#{extension}_wrap.cpp: #{extension}.i
	swig -c++ -ruby -o $@ $<

#{extension}_wrap.o: #{extension}_wrap.cpp
	$(CXX) $(INCFLAGS) -DSWIG $(CPPFLAGS) $(CXXFLAGS) -c $<

EOF
end
