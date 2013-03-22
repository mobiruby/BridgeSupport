/*
 * BrideSupport forces all boolean types to "bool", so that their encoding
 * is 'B'.  _Bool is the magic type used in stdbool.h.
 */
typedef _Bool BOOL;
typedef _Bool Boolean;
