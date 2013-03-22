ifneq ($(SWIG),YES)

SHELL = /bin/sh

#### Start of system configuration section. ####

srcdir = .
topdir = $(shell echo /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/lib/ruby/1.8/universal-darwin*)
hdrdir = $(topdir)
VPATH = $(srcdir):$(topdir):$(hdrdir)
exec_prefix = $(prefix)
prefix = $(DESTDIR)/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr
sharedstatedir = $(prefix)/com
mandir = $(DESTDIR)/usr/share/man
psdir = $(docdir)
oldincludedir = $(DESTDIR)/usr/include
localedir = $(datarootdir)/locale
bindir = $(exec_prefix)/bin
libexecdir = $(exec_prefix)/libexec
sitedir = $(DESTDIR)/Library/Ruby/Site
htmldir = $(docdir)
vendorarchdir = $(vendorlibdir)/$(sitearch)
includedir = $(prefix)/include
infodir = $(DESTDIR)/usr/share/info
vendorlibdir = $(vendordir)/$(ruby_version)
sysconfdir = $(prefix)/etc
libdir = $(exec_prefix)/lib
sbindir = $(exec_prefix)/sbin
rubylibdir = $(libdir)/ruby/$(ruby_version)
docdir = $(datarootdir)/doc/$(PACKAGE)
dvidir = $(docdir)
vendordir = $(libdir)/ruby/vendor_ruby
datarootdir = $(prefix)/share
pdfdir = $(docdir)
archdir = $(rubylibdir)/$(arch)
sitearchdir = $(sitelibdir)/$(sitearch)
datadir = $(datarootdir)
localstatedir = $(prefix)/var
sitelibdir = $(sitedir)/$(ruby_version)

CC = c++
CXX = $(CC)
LIBRUBY = $(LIBRUBY_SO)
LIBRUBY_A = lib$(RUBY_SO_NAME)-static.a
LIBRUBYARG_SHARED = -l$(RUBY_SO_NAME)
LIBRUBYARG_STATIC = -l$(RUBY_SO_NAME)

RUBY_EXTCONF_H = 
LLVM-CONFIG = llvm-config
CFLAGS   =  $(RC_CFLAGS) $(shell $(LLVM-CONFIG) --cxxflags | sed -e 's/ *-arch [^ ]*//g' -e 's/ *-D[^ ]*//g')
 
INCFLAGS = -I. -I$(topdir) -I$(hdrdir) -I$(srcdir)
DEFS     = 
CPPFLAGS =  $(shell $(LLVM-CONFIG) --cppflags | sed 's/ *-arch [^ ]*//g')

CXXFLAGS = $(CFLAGS) 
ldflags  = $(RC_CFLAGS) $(shell $(LLVM-CONFIG) --ldflags | sed 's/ *-arch [^ ]*//g')

dldflags = 
archflag = 
DLDFLAGS = $(ldflags) $(dldflags) $(archflag)
LDSHARED = $(CC) $(RC_CFLAGS) -pipe -bundle -undefined dynamic_lookup
AR = ar
EXEEXT = 

RUBY_INSTALL_NAME = ruby
RUBY_SO_NAME = ruby
arch = $(shell $(RUBY) -e 'puts RUBY_PLATFORM')
sitearch = $(arch)
ruby_version = 1.8
ruby = /System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/bin/ruby
RUBY = $(ruby)
RM = rm -f
MAKEDIRS = mkdir -p
INSTALL = /usr/bin/install -c
INSTALL_PROG = $(INSTALL) -m 0755
INSTALL_DATA = $(INSTALL) -m 644
COPY = cp

#### End of system configuration section. ####

preload = 

libpath = . $(libdir)
LIBPATH =  -L. -L$(libdir)
DEFFILE = 

CLEANFILES = mkmf.log bridgesupportparser_wrap.cpp
DISTCLEANFILES = 

extout = 
extout_prefix = 
target_prefix = 
LOCAL_LIBS = 
LIBS = $(LIBRUBYARG_SHARED) -lclangCodeGen -lclangAnalysis -lclangRewrite -lclangSema -lclangFrontend -lclangDriver -lclangAST -lclangParse -lclangLex -lclangBasic -lLLVMCore -lLLVMSupport -lLLVMSystem -lLLVMBitWriter -lLLVMBitReader -lLLVMCodeGen -lLLVMAnalysis -lLLVMTarget -lLLVMMC
SRCS = bridgesupportparser.cpp bridgesupportparser_wrap.cpp
OBJS = bridgesupportparser.o bridgesupportparser_wrap.o
TARGET = bridgesupportparser
DLLIB = $(TARGET).bundle
EXTSTATIC = 
STATIC_LIB = 

BINDIR        = $(bindir)
RUBYCOMMONDIR = $(sitedir)$(target_prefix)
RUBYLIBDIR    = $(sitelibdir)$(target_prefix)
RUBYARCHDIR   = $(sitearchdir)$(target_prefix)

TARGET_SO     = $(DLLIB)
CLEANLIBS     = $(TARGET).bundle $(TARGET).il? $(TARGET).tds $(TARGET).map
CLEANOBJS     = *.o *.a *.s[ol] *.pdb *.exp *.bak

all:		$(DLLIB)
static:		$(STATIC_LIB)

clean:
		@-$(RM) $(CLEANLIBS) $(CLEANOBJS) $(CLEANFILES)

distclean:	clean
		@-$(RM) Makefile $(RUBY_EXTCONF_H) conftest.* mkmf.log
		@-$(RM) core ruby$(EXEEXT) *~ $(DISTCLEANFILES)

realclean:	distclean
install: install-so install-rb

install-so: $(RUBYARCHDIR)
install-so: $(RUBYARCHDIR)/$(DLLIB)
$(RUBYARCHDIR)/$(DLLIB): $(DLLIB)
	$(INSTALL_PROG) $(DLLIB) $(RUBYARCHDIR)
install-rb: pre-install-rb install-rb-default
install-rb-default: pre-install-rb-default
pre-install-rb: Makefile
pre-install-rb-default: Makefile
$(RUBYARCHDIR):
	$(MAKEDIRS) $@

site-install: site-install-so site-install-rb
site-install-so: install-so
site-install-rb: install-rb

.SUFFIXES: .c .m .cc .cxx .cpp .C .o

.cc.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.cxx.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.cpp.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.C.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.c.o:
	$(CC) $(INCFLAGS) $(CPPFLAGS) $(CFLAGS) -c $<

$(DLLIB): $(OBJS) Makefile
	@-$(RM) $@
	$(LDSHARED) -o $@ $(OBJS) $(LIBPATH) $(DLDFLAGS) $(LOCAL_LIBS) $(LIBS)
ifneq ($(findstring -g,$(CXXFLAGS)),)
	dsymutil $@
endif


$(OBJS): ruby.h defines.h

BSP_HEADERS = classes.h bridgesupportparser.h __xattr__.h
$(OBJS): $(BSP_HEADERS)

bridgesupportparser_wrap.o: bridgesupportparser_wrap.cpp
	$(CXX) $(INCFLAGS) -DSWIG $(CPPFLAGS) $(CXXFLAGS) -c $<

else # SWIG = YES

# Because of different versions of swig on different OS releases, it isn't
# safe to create bridgesupportparser_wrap.cpp by running swig at build time
# (bridgesupportparser.i is dependent on certain swig internals that
# may change with different versions of swig).  So we use a frozen version
# of bridgesupportparser_wrap.cpp, which can be regenerated by running
# "make SWIG=YES".  Anytime classes.h, bridgesupportparser.h, __xattr__.h
# or bridgesupportparser.i change, bridgesupportparser_wrap.cpp will need
# to be regenerated manually.

bridgesupportparser_wrap.cpp: $(BSP_HEADERS)
bridgesupportparser_wrap.cpp: bridgesupportparser.i
	swig -c++ -ruby -o $@ $<

endif # SWIG = YES
