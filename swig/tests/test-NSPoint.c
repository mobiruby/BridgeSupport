#import <ApplicationServices/../Frameworks/CoreGraphics.framework/Headers/CGBase.h>
#import <ApplicationServices/../Frameworks/CoreGraphics.framework/Headers/CGGeometry.h>

#ifdef __LP64__
typedef CGPoint NSPoint;
#else
typedef struct _NSPoint {
    CGFloat x;
    CGFloat y;
} NSPoint;
#endif
