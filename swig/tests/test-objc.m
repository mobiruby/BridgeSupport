#include "__xattr__.h"
#include <stdbool.h>
#import <Foundation/NSObject.h>

#define AAA 1

typedef int retint __xattr__(ret);

@interface Foo
-(retint) mumble:(char *) __xattr__(it_works) x __xattr__(method) __xattr_return_value__(ret2=mumble);
-booboo:(in const char *)follow yogi:(const int *) __xattr__(out) food;
-(long) longMethod: (long)x;
-(_Bool) picnicBaskets: (int)n full: (bool)isFull;
@end
@interface NSObject (Informal)
- (int)aaa: (int)a bbb: (int)b __xattr__(_i=2);
@end

retint mumble2(char * __xattr__(it_works) x, void *y __xattr__(out)) __xattr__(function);
