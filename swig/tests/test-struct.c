#include "__xattr__.h"

typedef int retint __xattr__(sad);
typedef long CFIndex;

long LONG;
CFIndex CFINDEX;
struct unknown;
typedef struct unknown *opaque;

struct foo {
    retint a __xattr__(happy);
    int (*b)(void *);
};
typedef struct {
    const char *yogi;
    CFIndex picnic_baskets;
} boo;
typedef struct {
    int zz;
} *zp;
typedef struct xyz {
    const char *yogi;
    CFIndex picnic_baskets;
} abc;
typedef struct a123 {
    int zz;
} *ap;
typedef struct {
    struct foo f;
    boo b;
} one_level;
typedef struct {
    one_level one;
    zp z;
} two_level;
struct s2 {
    one_level *one;
    zp z;
} as2;

struct foo *blah(boo *in, zp in2, ap in3);
boo *fubar(struct foo *sf, abc *zz);
zp one(one_level *x);
ap two(two_level *x, struct s2 *y);
