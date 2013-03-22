#include "__xattr__.h"

typedef struct _Foo *CFFooRef;
typedef struct _Boo *CFBooRef __xattr__(gettypeid_func=blah);
typedef struct _Blah *CFBlahRef;

int CFBlahGetTypeID(void);
CFBlahRef CFCreateBlahRef(void);
