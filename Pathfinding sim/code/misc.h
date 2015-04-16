#ifndef MISC
#define MISC

#include <stdint.h>

typedef unsigned int uint;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint8 bool8;

typedef float float32;
typedef double float64;

#define ever (;;)

#define len(array) sizeof(array)/sizeof((array)[0])

#ifdef DEBUG
#define assert(this_is_true) if(!(this_is_true)) *((char*)0)=0
#endif

#define megabyte 1024*1024
#define gigabyte 1024*1024*1024

#endif
