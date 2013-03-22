#ifndef ____XATTR___H__

#  define ____XATTR___H__
#  define __XATTR_PREFIX			"xattr:"
#  define __XATTR_RETURN_VALUE_PREFIX	"return_value:"

#  define __XATTR1__(x)			__attribute__((__annotate__(__XATTR_PREFIX #x)))
#  define __XATTR2__(x,y)			__attribute__((__annotate__(__XATTR_PREFIX x #y)))
#  define __xattr__(x)			__XATTR1__(x)
#  define __xattr_return_value__(x)	__XATTR2__(__XATTR_RETURN_VALUE_PREFIX,x)

#endif /* ____XATTR___H__ */
