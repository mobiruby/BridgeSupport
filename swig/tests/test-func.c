#include <AvailabilityMacros.h>
int fubar(const char *baz);

inline int __fubar(const char *) { return 4; }
#define fubar __fubar

inline void aaa(void);
inline void bbb(void*restrict, void const* restrict, const void* restrict);
inline void *zzz(void *) __attribute__((always_inline));
void *yyy(void *) __attribute__((always_inline)); // attribute has no effect
long on_10_3_later(const char *) AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
