#include <sys/cdefs.h>
#include "__xattr__.h"
#include <Foundation/Foundation.h>

@interface A: NSObject

+alloc;
-init;
-release;
//-(int)printfFormat:(NSString *)fmt, ... __printflike(1,2) __attribute__((__nonnull__(1)));
-(int)printfFormat:(NSString *) __xattr__(printf_format) fmt, ... __attribute__((__nonnull__(1)));
@end

int
main()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    A *a = [[A alloc] init];
    [a printfFormat:@"%d\n", 42];
    [a release];
    return 0;
}
