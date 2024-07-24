#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#include "stm32f10x.h"

#include <stdbool.h>

#ifndef NULL
#define NULL                            ((void *)0)
#endif

#define ON 1
#define OFF 0

#define vd_true    true
#define vd_false   false
#define vd_null    NULL
#define vd_void_t  void
#define vd_bool_t  bool 

typedef signed long    vd_long_t;
typedef unsigned long  vd_ulong_t;
typedef vd_long_t      vd_int64_t;
typedef vd_ulong_t     vd_uint64_t;

typedef signed int     vd_int_t;
typedef unsigned int   vd_uint_t;
typedef vd_int_t       int32_t;
typedef vd_uint_t      uint32_t;
typedef vd_int_t       vd_int32_t;
typedef vd_uint_t      vd_uint32_t;

typedef signed short   vd_short_t;
typedef unsigned short vd_ushort_t;
typedef vd_short_t     vd_int16_t;
typedef vd_ushort_t    vd_uint16_t;
typedef vd_short_t     int16_t;
typedef vd_ushort_t    uint16_t;


//typedef bool           vd_bool_t;

typedef signed char    vd_int8_t;
typedef unsigned char  vd_uint8_t;
typedef unsigned char  uint8_t;
typedef vd_uint8_t     vd_byte_t;
typedef char           vd_char_t;
typedef unsigned char  vd_uchar_t;


#endif  // BASE_TYPE_H
