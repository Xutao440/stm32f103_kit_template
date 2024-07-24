/*
 * Project: linux-kit
 * Moudle: BASE
 * File: base_utils.h
 * Created Date: 2021-01-27 10:57:40
 * Author: ShiBin
 * Description: 常用工具宏
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef BASE_UTILS_H
#define BASE_UTILS_H

/* ======================================================================================
 * includes
 */

/* ======================================================================================
 * extern
 */

/* ======================================================================================
 * macros
 */
///bit
#define vd_bit(x)	(1 << (x))
 
/// abs
#define vd_abs(x) ((x) > 0 ? (x) : -(x))

/// max
#define vd_max(x, y) (((x) > (y)) ? (x) : (y))

/// min
#define vd_min(x, y) (((x) < (y)) ? (x) : (y))

/// max3
#define vd_max3(x, y, z) (((x) > (y)) ? (((x) > (z)) ? (x) : (z)) : (((y) > (z)) ? (y) : (z)))

/// min3
#define vd_min3(x, y, z) (((x) < (y)) ? (((x) < (z)) ? (x) : (z)) : (((y) < (z)) ? (y) : (z)))

/// the number of entries in the array
#define vd_array_size(x) ((sizeof((x)) / sizeof((x)[0])))

/// ispow2: 1, 2, 4, 8, 16, 32, ...
#define vd_ispow2(x) (!((x) & ((x)-1)) && (x))

/// align2
#define vd_align2(x) (((x) + 1) >> 1 << 1)

/// align4
#define vd_align4(x) (((x) + 3) >> 2 << 2)

/// align8
#define vd_align8(x) (((x) + 7) >> 3 << 3)

/// align
#define vd_align(x, b) (((vd_size_t)(x) + ((vd_size_t)(b)-1)) & ~((vd_size_t)(b)-1))

/// align u32
#define vd_align_u32(x, b) (((vd_uint32_t)(x) + ((vd_uint32_t)(b)-1)) & ~((vd_uint32_t)(b)-1))

/// align u64
#define vd_align_u64(x, b) (((vd_uint64_t)(x) + ((vd_uint64_t)(b)-1)) & ~((vd_uint64_t)(b)-1))

#if (USE_PLATFORM == MCU)
#else
/*! @def vd_align_cpu
 *
 * align by cpu bytes
 */
#if VD_CPU_BIT64
#define vd_align_cpu(x) vd_align8(x)
#else
#define vd_align_cpu(x) vd_align4(x)
#endif

/// offsetof
#if defined(VD_COMPILER_IS_GCC) && VD_COMPILER_VERSION_BE(4, 1)
#define vd_offsetof(s, m) (vd_size_t) __builtin_offsetof(s, m)
#else
#define vd_offsetof(s, m) (vd_size_t) & (((s const*)0)->m)
#endif

/// container of
#define vd_container_of(s, m, p) ((s*)(((vd_byte_t*)(p)) - vd_offsetof(s, m)))

/// memsizeof
#define vd_memsizeof(s, m)       sizeof(((s const*)0)->m)

/// memtailof
#define vd_memtailof(s, m)       (vd_offsetof(s, m) + vd_memsizeof(s, m))

/// memdiffof: lm - rm
#define vd_memdiffof(s, lm, rm)  (vd_memtailof(s, lm) - vd_memtailof(s, rm))

/// check the offset and size of member for struct or union
#define vd_memberof_eq(ls, lm, rs, rm) \
    ((vd_offsetof(ls, lm) == vd_offsetof(rs, rm)) && (vd_memsizeof(ls, lm) == vd_memsizeof(rs, rm)))

/// pointer to bool
#define vd_p2b(x)   ((vd_bool_t)(vd_size_t)(x))

/// pointer to u8
#define vd_p2u8(x)  ((vd_uint8_t)(vd_size_t)(x))

/// pointer to u16
#define vd_p2u16(x) ((vd_uint16_t)(vd_size_t)(x))

/// pointer to u32
#define vd_p2u32(x) ((vd_uint32_t)(vd_size_t)(x))

/// pointer to u64
#define vd_p2u64(x) ((vd_uint64_t)(vd_size_t)(x))

/// pointer to s8
#define vd_p2i8(x)  ((vd_int8_t)(vd_long_t)(x))

/// pointer to s16
#define vd_p2i16(x) ((vd_int16_t)(vd_long_t)(x))

/// pointer to s32
#define vd_p2i32(x) ((vd_int32_t)(vd_long_t)(x))

/// pointer to s64
#define vd_p2i64(x) ((vd_int64_t)(vd_long_t)(x))

/// bool to pointer
#define vd_b2p(x)   ((vd_void_t*)(vd_size_t)(x))

/// unsigned integer to pointer
#define vd_u2p(x)   ((vd_void_t*)(vd_size_t)(x))

/// integer to pointer
#define vd_i2p(x)   ((vd_void_t*)(vd_long_t)(x))
#endif

/// swap
#define vd_swap(t, l, r) \
    do {                 \
        t __p = (r);     \
        (r)   = (l);     \
        (l)   = __p;     \
    } while (0)

#define vd_safe_free(pointer) \
    if (pointer != vd_null) { \
        os_free(pointer);        \
    }

#define vd_safe_call_2(var, member, param1, param2) \
    if (var != vd_null) {                           \
        if (var##->member != vd_null) {             \
            var##->member(param1, param2);          \
        }                                           \
    }

#define vd_safe_call_1(var, member, param) \
    if (var != vd_null) {                  \
        if (var##->member != vd_null) {    \
            var##->member(param);          \
        }                                  \
    }

#define vd_safe_call(var, member)       \
    if (var != vd_null) {               \
        if (var##->member != vd_null) { \
            var##->member();            \
        }                               \
    }

#define vd_safe_call_2_ret(var, member, param1, param2) \
    if (var != vd_null) {                               \
        if (var##->member != vd_null) {                 \
            return var##->member(param1, param2);       \
        }                                               \
    }                                                   \
    return vd_false;

#define vd_safe_call_1_ret(var, member, param) \
    if (var != vd_null) {                      \
        if (var##->member != vd_null) {        \
            return var##->member(param);       \
        }                                      \
    }                                          \
    return vd_false;

#define vd_safe_call_ret(var, member)   \
    if (var != vd_null) {               \
        if (var##->member != vd_null) { \
            return var##->member();     \
        }                               \
    }                                   \
    return vd_false;

// is string
#define vd_is_string(str) (atoi(str) == 0 && strncmp(str, "0", 1) != 0)
#define vd_assert
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
#endif

	

	









