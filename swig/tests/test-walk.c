#include "__xattr__.h"
typedef int foo __xattr__(yes=no);
typedef foo *baz __xattr__(__nonnull__);
typedef baz boo __xattr__(a;b);
