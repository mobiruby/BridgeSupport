#define SIZE sizeof(long)
long longfunc(long *x);

#ifdef __LP64__
#define XXX 64
#define ONLY64 "64"
long aLong;
#else /* !__LP64__ */
#define XXX 32
#define ONLY32 "32"
long long aLong;
#endif /* __LP64__ */
