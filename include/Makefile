##
# Makefile for custom header files that force boolean types to "bool", so
# their encoding is 'B'
##

INCLUDEOBJC = $(BS_INCLUDE)/objc

include:
	mkdir -p $(INCLUDEOBJC)
	cp /System/Library/Frameworks/CoreServices.framework/Frameworks/CarbonCore.framework/Headers/MacTypes.h $(BS_INCLUDE)/BSMacTypes.h
	ed - $(BS_INCLUDE)/BSMacTypes.h < BSMacTypes.h.ed
	cp /usr/include/objc/objc.h $(INCLUDEOBJC)
	ed - $(INCLUDEOBJC)/objc.h < objc.h.ed
