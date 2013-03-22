##
# Makefile for BridgeSupport 
##
ifdef RC_ProjectName
Project = $(RC_ProjectName)
else # !RC_ProjectName
Project = BridgeSupport
endif

include $(MAKEFILEPATH)/CoreOS/ReleaseControl/Common.make

# Override defaults
PWD = $(shell pwd)
DEF_DSTROOT = $(PWD)/DSTROOT
DSTROOT = $(DEF_DSTROOT)
OBJROOT = $(PWD)/OBJROOT
SRCROOT = $(PWD)
DEF_SYMROOT = $(PWD)/SYMROOT
SYMROOT = $(DEF_SYMROOT)
DESTDIR = /
RC_ARCHS = x86_64
ORDERED_ARCHS = $(filter %64,$(RC_ARCHS)) $(filter-out %64,$(RC_ARCHS))
RC_CFLAGS = $(foreach arch,$(RC_ARCHS),-arch $(arch)) -pipe

RSYNC = /usr/bin/rsync -rlpt
RUBY = /usr/bin/ruby

# Use files to represent whether a directory exist, avoiding problems with
# the modification date of a directory changing.  To avoid cluttering up
# the DSTROOT and SYMROOT with these files, we make them in the OBJROOT.
MADEFILE = .made
DSTROOT_MADE = $(OBJROOT)/.DSTROOT$(MADEFILE)
OBJROOT_MADE = $(OBJROOT)/$(MADEFILE)
SYMROOT_MADE = $(OBJROOT)/.SYMROOT$(MADEFILE)

USR_BIN = $(DSTROOT)/usr/bin
DTDS = $(DSTROOT)/System/Library/DTDs
ifeq ($(RC_ProjectName),BridgeSupport_ext)
BS_LIBS = $(OBJROOT)/BridgeSupport
else # !BridgeSupport_ext
BS_LIBS = $(DSTROOT)/System/Library/BridgeSupport
endif # !BridgeSupport_ext
BS_INCLUDE = $(BS_LIBS)/include
BS_RUBY := $(BS_LIBS)/ruby-$(shell $(RUBY) -e 'puts RUBY_VERSION.sub(/^(\d+\.\d+)(\..*)?$$/, "\\1")')
RUBYLIB = $(BS_RUBY)

LIBSYSTEM_HEADERS = /usr/include/asl.h /usr/include/notify*.h /usr/include/copyfile.h /usr/include/sandbox.h /usr/include/launch.h /usr/include/CommonCrypto/*.h

# For the Apple build system, we split into two separate projects:
#     BridgeSupport_ext - build extension and save in /usr/local/BridgeSupport
#     BridgeSupport - install extension and use it to build everything else
ifeq ($(RC_XBS),YES)
ifeq ($(RC_ProjectName),BridgeSupport_ext)
build:: start_build build_extension save_extension finish_build
else # !BridgeSupport_ext
build:: start_build install_extension build_files install_additional_files finish_build
endif # !BridgeSupport_ext
else # !RC_XBS
build:: start_build build_extension build_files install_additional_files finish_build
endif # !RC_XBS

.PHONY: start_build build_extension build_files install_additional_files finish_build save_extension install_extension

start_build:
	@/bin/echo -n '*** Started Building $(Project): ' && date

finish_build:
	@/bin/echo -n '*** Finished Building $(Project): ' && date

$(DSTROOT_MADE): $(OBJROOT_MADE)
	$(MKDIR) $(DSTROOT)
	$(TOUCH) $@

$(OBJROOT_MADE):
	$(MKDIR) $(OBJROOT)
	$(TOUCH) $@

$(SYMROOT_MADE): $(OBJROOT_MADE)
	$(MKDIR) $(SYMROOT)
	$(TOUCH) $@

# Subdirectories
CLANG_VERS = clang-111.2
CLANG_TARBALL = $(CLANG_VERS).tar.bz2
CLANG_DIR = $(OBJROOT)/$(CLANG_VERS)
SWIG_DIR = $(OBJROOT)/swig

CLANG_DIR_MADE = $(CLANG_DIR)/$(MADEFILE)
$(CLANG_DIR_MADE): $(OBJROOT_MADE)
	$(TAR) -xjof $(SRCROOT)/$(CLANG_TARBALL) -C $(OBJROOT)
	$(TOUCH) $@

SWIG_DIR_MADE = $(SWIG_DIR)/$(MADEFILE)
$(SWIG_DIR_MADE): $(OBJROOT_MADE)
	$(RSYNC) $(SRCROOT)/swig $(OBJROOT)
	$(TOUCH) $@

ifeq ($(words $(RC_ARCHS)),1)
CLANGROOT = $(CLANG_DIR)/src/darwin-$(RC_ARCHS)/ROOT
else
CLANGROOT = $(OBJROOT)/ROOT
ARCHIVE_DIR = lib
ARCHIVE_LIST = $(OBJROOT)/archive_list
COPY_DIRS = bin include
endif
CLANGROOT_MADE = $(CLANGROOT)/$(MADEFILE)
CLANG_PREFIX = /usr

$(CLANGROOT_MADE): $(CLANG_DIR_MADE)
	@/bin/echo -n '*** Started Building $(CLANG_VERS): ' && date
	@set -x && \
	for arch in $(RC_ARCHS); do \
	    $(RMDIR) $(CLANG_DIR)/src/darwin-$$arch && \
	    $(MKDIR) $(CLANG_DIR)/src/darwin-$$arch && \
	    (cd $(CLANG_DIR)/src/darwin-$$arch && \
	    $(MKDIR) ROOT && \
	    ../configure --prefix=$(CLANG_PREFIX) --enable-debug-runtime --enable-debug-symbols --enable-optimized --disable-timestamps --disable-assertions --with-optimize-option='-Os' --without-llvmgcc --without-llvmgxx --disable-bindings --disable-doxygen --with-extra-options='-DDISABLE_SMART_POINTERS' CC="cc -arch $$arch" CXX="c++ -arch $$arch" && \
	    make -j$(shell sysctl -n hw.ncpu) && \
	    $(MKDIR) $(CLANG_DIR)/src/darwin-$$arch/ROOT && \
	    make install DESTDIR=$(CLANG_DIR)/src/darwin-$$arch/ROOT && \
	    cp -f ../tools/clang/lib/Sema/*.h $(CLANG_DIR)/src/darwin-$$arch/ROOT$(CLANG_PREFIX)/include/clang/Sema) || exit 1; \
	done
ifneq ($(words $(RC_ARCHS)),1)
	$(MKDIR) $(CLANGROOT)$(CLANG_PREFIX)
	cd $(CLANG_DIR)/src/darwin-$(firstword $(ORDERED_ARCHS))/ROOT$(CLANG_PREFIX) && $(RSYNC) $(COPY_DIRS) $(CLANGROOT)$(CLANG_PREFIX)
	cd $(CLANG_DIR)/src/darwin-$(firstword $(ORDERED_ARCHS))/ROOT$(CLANG_PREFIX)/$(ARCHIVE_DIR) && ls *.a > $(ARCHIVE_LIST)
	$(MKDIR) $(CLANGROOT)$(CLANG_PREFIX)/$(ARCHIVE_DIR)
	@set -x && \
	for a in `cat $(ARCHIVE_LIST)`; do \
	    $(LIPO) -create -output $(CLANGROOT)$(CLANG_PREFIX)/$(ARCHIVE_DIR)/$$a $(foreach arch,$(RC_ARCHS),-arch $(arch) $(CLANG_DIR)/src/darwin-$(arch)/ROOT$(CLANG_PREFIX)/$(ARCHIVE_DIR)/$$a) && \
	    $(RANLIB) $(CLANGROOT)$(CLANG_PREFIX)/$(ARCHIVE_DIR)/$$a || exit 1; \
	done
endif
	@/bin/echo -n '*** Finished Building $(CLANG_VERS): ' && date
	$(TOUCH) $@

# Remove the $(BS_RUBY_MADE) file if you want bridgesupportparser.bundle remade
# or if bridgesupport.rb has been modified
BS_RUBY_MADE = $(OBJROOT)/.BS_RUBY$(MADEFILE)
$(BS_RUBY_MADE): $(CLANGROOT_MADE) $(SWIG_DIR_MADE) $(DSTROOT_MADE) $(SYMROOT_MADE)
	@/bin/echo -n '*** Started Building bridgesupport.bundle: ' && date
	@set -x && \
	cd $(SWIG_DIR) && \
	make LLVM-CONFIG=$(CLANGROOT)$(CLANG_PREFIX)/bin/llvm-config RC_CFLAGS='$(RC_CFLAGS)' && \
	$(MKDIR) $(BS_RUBY) && \
	$(RSYNC) bridgesupportparser.bundle* $(SYMROOT) && \
	$(RSYNC) bridgesupportparser.bundle $(BS_RUBY) && \
	$(STRIP) -x $(BS_RUBY)/bridgesupportparser.bundle && \
	$(RSYNC) bridgesupportparser.rb $(BS_RUBY)
	$(TOUCH) $@

BS_INCLUDE_MADE = $(OBJROOT)/.BS_INCLUDE$(MADEFILE)
$(BS_INCLUDE_MADE): $(DSTROOT_MADE)
	$(INSTALL_DIRECTORY) $(BS_INCLUDE)
	$(INSTALL_FILE) $(SRCROOT)/include/_BS_bool.h $(BS_INCLUDE)
	@/bin/echo -n '*** Finished Building bridgesupport.bundle: ' && date
	$(TOUCH) $@

build_extension: $(BS_RUBY_MADE) $(BS_INCLUDE_MADE)

SAVE_DIR = /usr/local/BridgeSupport/extension
save_extension: $(DSTROOT_MADE)
	$(INSTALL_DIRECTORY) $(DSTROOT)$(SAVE_DIR)
	ditto $(BS_LIBS) $(DSTROOT)$(SAVE_DIR)

install_extension: $(DSTROOT_MADE)
	$(INSTALL_DIRECTORY) $(BS_LIBS)
	ditto $(SAVE_DIR) $(BS_LIBS)

SYSTEM_BS = Frameworks/System.framework/Versions/B/Resources/BridgeSupport
SYSTEM_BRIDGESUPPORT = $(DSTROOT)/System/Library/$(SYSTEM_BS)
# Remove the $(LIBSYSTEM_BRIDGESUPPORT) file if you want the .bridgesupport
# files rebuilt.  You will need to remove the corresponding .bridgesupport file
# (or the entire .framework directory) in $(DSTROOT)/System/Library/Frameworks,
# to get build.rb to rebuild it.
LIBSYSTEM_BRIDGESUPPORT = $(BS_LIBS)/libSystem.bridgesupport
LIBSYSTEM_XML = libSystem.xml
LIBSYSTEM_EXCEPTION = $(shell plat=`ruby -e 'puts "exceptions-\#{RUBY_PLATFORM}/$(LIBSYSTEM_XML)"'` && if [ -f "$$plat" ]; then echo "$$plat"; else echo "exceptions/$(LIBSYSTEM_XML)"; fi)
$(LIBSYSTEM_BRIDGESUPPORT):
	@/bin/echo -n '*** Started Building .bridgesupport files: ' && date
	CFLAGS='$(RC_CFLAGS)' DSTROOT='$(DSTROOT)' RUBYLIB='$(RUBYLIB)' $(RUBY) build.rb
	RUBYLIB='$(RUBYLIB)' $(RUBY) gen_bridge_metadata.rb -c '-I/usr/include/CommonCrypto' -e $(LIBSYSTEM_EXCEPTION) -o $@ $(LIBSYSTEM_HEADERS)
	$(INSTALL_DIRECTORY) $(SYSTEM_BRIDGESUPPORT)
	$(LN) -fs `echo $(SYSTEM_BS) | sed 's,[^/]*,..,g'`/BridgeSupport/libSystem.bridgesupport $(SYSTEM_BRIDGESUPPORT)/System.bridgesupport
	@/bin/echo -n '*** Finished Building .bridgesupport files: ' && date

build_files: $(LIBSYSTEM_BRIDGESUPPORT)

BRIDGESUPPORT_5 = $(DSTROOT)$(MANDIR)/man5/BridgeSupport.5
$(BRIDGESUPPORT_5): $(DSTROOT_MADE)
	$(MKDIR) $(USR_BIN)
	$(INSTALL_PROGRAM) gen_bridge_metadata.rb $(USR_BIN)/gen_bridge_metadata
	$(CHMOD) +x $(USR_BIN)/gen_bridge_metadata
	$(MKDIR) $(DTDS)
	$(CP) BridgeSupport.dtd $(DTDS)
	$(MKDIR) $(DSTROOT)$(MANDIR)/man1
	$(INSTALL_FILE) $(SRCROOT)/gen_bridge_metadata.1 $(DSTROOT)$(MANDIR)/man1/gen_bridge_metadata.1
	$(MKDIR) $(DSTROOT)$(MANDIR)/man5
	$(INSTALL_FILE) $(SRCROOT)/BridgeSupport.5 $@

install_additional_files: $(BRIDGESUPPORT_5)

ifeq ($(RC_XBS),YES) # Apple build system

# make binary submission verifier happy
install_source::
	@set -x && \
	cd $(SRCROOT) && \
	$(TAR) -xjof $(CLANG_TARBALL) && \
	find $(CLANG_VERS) -name \*.a -delete -print && \
	$(RM) $(CLANG_TARBALL) && \
	$(TAR) -cjf $(CLANG_TARBALL) $(CLANG_VERS) && \
	$(RMDIR) $(CLANG_VERS)

else # normal make targets

install::
ifneq ($(DESTDIR),/)
	$(MKDIR) $(DESTDIR)
endif
	$(PERL) $(SRCROOT)/tar-chown-root.pl $(DSTROOT) | $(TAR) -xpf - -C $(DESTDIR)

clean::
ifeq ($(DSTROOT),$(DEF_DSTROOT))
	$(_v) $(RMDIR) "$(DSTROOT)" || true
endif
ifeq ($(SYMROOT),$(DEF_SYMROOT))
	$(_v) $(RMDIR) "$(SYMROOT)" || true
endif

endif #

update_exceptions:
	$(RUBY) build.rb --update-exceptions

sort_exceptions:
	for i in `ls exceptions/*.xml`; do $(RUBY) sort.rb $$i | xmllint -format - > $$i.new; mv $$i.new $$i; done
