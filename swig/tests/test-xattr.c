#include <sys/cdefs.h>
#include "__xattr__.h"

typedef int retval __xattr__(ret1=ignore);

retval foo(int *a __xattr__(arg), int b, const char *fmt, ...) __attribute__((nonnull)) __xattr__(func) __xattr_return_value__(ret2) __printflike(3,4);
int bar(void *x __xattr__(nonnull), void *y __xattr__(null_accepted=true));
