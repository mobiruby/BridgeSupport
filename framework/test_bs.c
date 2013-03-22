#include <BridgeSupport/BridgeSupport.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

static unsigned element_count;

static void
parse_cb(const char *path, bs_element_type_t type, void *value, void *context)
{
  //printf("%s: element type %d value %p\n", path, type, value);
  bs_element_free(type, value);
  element_count++;
}

static void
load_framework(const char *path)
{
  char *error;  
  uint64_t elapsed, start;
  Nanoseconds elapsedNano;
  
  element_count = 0;
  
  start = mach_absolute_time();
  
  if (!bs_parse(path, 0, parse_cb, NULL, &error)) {
    printf("*** error : %s\n", error);
    free(error);
  }
  
  elapsed = mach_absolute_time() - start;
  elapsedNano = AbsoluteToNanoseconds( *(AbsoluteTime *) &elapsed );

  printf("parsed %d elements, took %lld ns\n",  element_count, *(uint64_t *)&elapsedNano);
}

int 
main(void)
{
  char path[PATH_MAX];
  unsigned i;

  assert(
    bs_find_path("/System/Library/Frameworks/Cocoa.framework", 
                 path, PATH_MAX));

  for (i = 0; i < 10; i++) 
    load_framework(path);
  return 0;
}