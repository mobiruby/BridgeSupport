//#define __CONSTANT_CFSTRINGS__
#undef __CONSTANT_CFSTRINGS__
#include <stdio.h>
#include <sys/types.h>
#import <Foundation/Foundation.h>

#include "__xattr__.h"

@protocol Sleeping
-(const char *)snoreLoudness:(int)level;
@end

@protocol Dreaming <Sleeping>
-(long)dreamKind:(const char *)dream;
#ifdef FOO
-(bool)dreaming;
#endif
@end

@interface NSObject (Informal)
-(bool)tieRequired;
@end

@interface Fraction: NSObject <Sleeping> {
    int numerator;
    int denominator;
    double donuts;
    double dollars;
    long lazy;
}

@property double donuts, dollars;
@property (readonly, getter=fubar) long lazy;
-(void) print;
-(void) setNumerator: (int) n;
-(void) setDenominator: (int) d;
-junk: (out const char *)x foo: (double)y bar: (FILE*)z, ...;
-(int) numerator;
-(int) denominator;
-(id) unavailable: (int)x __attribute__((unavailable));
-(void) doArray: (in const char *)array size:(unsigned)s __xattr__(c_array_length_in_arg=2);
-(long) callback: (int (*)(int))cb with: (int)x;
@end

@interface Fraction ()
-(id <Sleeping>) extension;
@end

@interface Fraction (Rational) <Dreaming>
-(double)doubleWithPrecision:(int)precision;
@end

NSString *where = @"there";
extern int bla;
/*extern int bla2 = 4;*/ // invalid

enum {apple, banana, cabbage};
enum mumble {
    aaa = 42,
    bbb = -3,
    ccc = 'a',
    ccc2 = 'ab',
    ccc3 = 'abc',
    ccc4 = 'gurl',
    hex = 0x6775726c,
    ddd = 99
};
enum {myNSNotFound = NSIntegerMax};

#define Z 42
#define A Z
#define Pi 3.1416
#define Sum (2.3+5.9)
#define Hex 0x1234
#define Str "hel\"lo"
#define LStr L"hello"
#define zebra banana
#define junk 'd'
#define KAEINTERNETSUITE 'gurl'
#define Ljunk L'd'
#define apostrophe '\''
#define WHEN @"now"
#define HOW CFSTR("quietly")
#define WHEN CFSTR(Str)
#define WHY CFSTR("brown" "cow")
#define bufsize2 BUFSIZ
#define bufsize bufsize2
#define myqsort qsort
#define E (2.718)
#define F -1
#define G ((long)-2)
#define IF if
#define bra {
#define ket }
#define cbra '{'
#define sket "}"
#define semi ;
#define csemi ';'
#define string "{;}"

int a;
int a = A;
int abc __attribute((unavailable));

int b = 4, c = 5;
const double d = Pi;

NSString *When = WHEN;

typedef unsigned int u32;
typedef int (*funcpointertype)(int, int);
typedef int functype(int, int);
typedef struct _STUFF stuff;
typedef struct {int x; int y;} mypoint;
typedef struct {int w; int h;} *size;
typedef struct _OPAQUE opaque, *opaquep;
typedef void (*FooFuncType)(SEL); // 5610895

int (*funcp)(int, int);

__typeof(funcp) fp2;
funcpointertype fp3;

functype f;
__typeof(f) f2;

struct _STUFF {
    int a;
    const char foo[];
};

inline
int fubar(functype *z)
{
    return z(1, 3);
}

typedef int __xattr__(REALLY LOUD) verbosity_t;

int      asprintf(char ** __attribute__((annotate("other"))), const char * __attribute((annotate("xattr:happy"),annotate("xattr:sad"))), ...) __DARWIN_LDBL_COMPAT(asprintf) __attribute__((format (printf, 2, 3)));
double	 verbose(char const * const message __xattr__(hi;there), verbosity_t verbosity);
void	 qsort(void *base __xattr__((foo=1,2;boo)), size_t nel, size_t width, int (*compar)(const void *, const void *)) __attribute__((nonnull));
void madness(int (*compar)(const void *, int (*)(const void *, int (*)(const void *, const void *)))); 

int main()
{
  int a = 4;

  int b = a;
  __DARWIN_FD_ZERO(&b);
}

/*int test() = main;*/  // illegal

struct s {} t;
struct x {};
struct xyzzy;
typedef struct s ts;

struct _ss {
    int z;
    struct xyzzy *p;
    union {
	int a;
	double d;
    } u;
} ss;
union _uu {
    long l;
    struct {
	int x;
	int y;
    } p;
} uu;
