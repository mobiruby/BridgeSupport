#include <CoreFoundation/CoreFoundation.h>
#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED
#define OK_GREATER_10_5 1
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#define OK_LESS_10_6 1
#endif
#define ALWAYS 1
int foo(int);
int foo_deprecated(int) DEPRECATED_ATTRIBUTE;
double dblfunc(int);

#define NSAppKitVersionNumber10_0 577
#define NSAppKitVersionNumber10_1 620
#define NSAppKitVersionNumber10_2 663
#define NSAppKitVersionNumber10_2_3 663.6
#define NSAppKitVersionNumber10_3 743
#define NSAppKitVersionNumber10_3_2 743.14
#define NSAppKitVersionNumber10_3_3 743.2
#define NSAppKitVersionNumber10_3_5 743.24
#define NSAppKitVersionNumber10_3_7 743.33
#define NSAppKitVersionNumber10_3_9 743.36
#define NSAppKitVersionNumber10_4 824
#define NSAppKitVersionNumber10_4_1 824.1
#define NSAppKitVersionNumber10_4_3 824.23
#define NSAppKitVersionNumber10_4_4 824.33
#define NSAppKitVersionNumber10_4_7 824.41
#define NSAppKitVersionNumber10_5 949
#define NSAppKitVersionNumber10_5_2 949.27
#define NSAppKitVersionNumber10_5_3 949.33

#define CASTDOUBLE	((double)123)
#define NUMWITHCOMMENT	42 /* ultimate answer */
#define PARENDOUBLE (4.2)
#define PARENNEGDOUBLE (-4.2)
#define EXPONENTIAL 1e42
#define NEG -42
#define UNSIGNED 42U
#define LONG 42L
#define LONGDOUBLE 4.2L
#define LONGDOUBLELOWER 4.2l
#define LONGLONG 42LL
#define FLOAT 1.23F
#define SUM (1+1)
#define DOUBLESUM (1.1+2.2)
#define CHAR 'x'
#define LCHAR L'x'
#define STRING "foo"
#define CATSTRING "cat" "foo"
#define OBJCSTRING @"objcfoo"
#define CFSTRING CFSTR("cffoo")
#define CFSTRINGMACRO CFSTR(STRING)
#define PARENCFSTRING (CFSTR("parencffoo"))
#define PARENPARENCFSTRING ((CFSTR("parencffoo")))

#define LRBRACE {}
#define SEMI ;

#define ILLEGAL L3.14

#define funccallint foo(32)
#define funccalldbl dblfunc(42)

#define SERVER @"http://server.com"

typedef float CGDisplayReservationInterval;
#define kCGMaxDisplayReservationInterval ((CGDisplayReservationInterval)(15.0))
