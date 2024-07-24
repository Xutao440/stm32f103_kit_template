/*
 * Project: linux-kit
 * Moudle: BASE
 * File: base_keyword.h
 * Created Date: 2021-01-27 10:13:19
 * Author: ShiBin
 * Description: 一些关键字重定义
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef BASE_KEYWORD_H
#define BASE_KEYWORD_H

/* ======================================================================================
 * includes
 */
 #if (USE_PLATFORM == MCU)
#define __vd_extern_c_enter__
#define __vd_extern_c_leave__
#else
#include "base_compiler.h"
#include "base_cpu.h"

/* ======================================================================================
 * extern
 */

/* ======================================================================================
 * macros
 */

// the register keyword will be deprecated in C++
#ifndef __cplusplus
    #define __vd_register__ register
#else
    #define __vd_register__
#endif
#define __vd_volatile__ volatile

#if defined(VD_COMPILER_IS_MSVC)

    #define __vd_func__         __FUNCTION__
    #define __vd_file__         __FILE__
    #define __vd_line__         __LINE__
    #define __vd_date__         __DATE__
    #define __vd_time__         __TIME__
    #define __vd_asm__          __asm
    #define __vd_inline__       __inline
    #define __vd_inline_force__ __forceinline
    #define __vd_cdecl__        __cdecl
    #define __vd_stdcall__      __stdcall
    #define __vd_fastcall__     __fastcall
    #define __vd_thiscall__     __thiscall
    #define __vd_packed__
    #define __vd_aligned__(a) __declspec(align(a))

#elif defined(VD_COMPILER_IS_GCC)

    #define __vd_func__         __FUNCTION__
    #define __vd_file__         __FILE__
    #define __vd_line__         __LINE__
    #define __vd_date__         __DATE__
    #define __vd_time__         __TIME__
    #define __vd_asm__          __asm__
    #define __vd_inline__       __inline__
    #define __vd_inline_force__ __inline__ __attribute__((always_inline))
    #define __vd_packed__       __attribute__((packed, aligned(1)))
    #define __vd_aligned__(a)   __attribute__((aligned(a)))
    // gcc will generate attribute ignored warning
    #if defined(__x86_64) || defined(__amd64__) || defined(__amd64) || defined(_M_IA64) || defined(_M_X64)
        #define __vd_cdecl__
        #define __vd_stdcall__
        #define __vd_fastcall__
        #define __vd_thiscall__
    #else
        #define __vd_cdecl__    __attribute__((__cdecl__))
        #define __vd_stdcall__  __attribute__((__stdcall__))
        #define __vd_fastcall__ __attribute__((__fastcall__))
        #define __vd_thiscall__ __attribute__((__thiscall__))
    #endif

#elif defined(VD_COMPILER_IS_TINYC)

    #define __vd_func__         __func__
    #define __vd_file__         __FILE__
    #define __vd_line__         __LINE__
    #define __vd_date__         __DATE__
    #define __vd_time__         __TIME__
    #define __vd_asm__          __asm__
    #define __vd_inline__       __inline__
    #define __vd_inline_force__ __inline__ __attribute__((always_inline))
    #define __vd_packed__       __attribute__((packed, aligned(1)))
    #define __vd_aligned__(a)   __attribute__((aligned(a)))
    #define __vd_cdecl__        __attribute__((cdecl))
    #define __vd_stdcall__      __attribute__((stdcall))
    #define __vd_fastcall__     __attribute__((fastcall))
    #define __vd_thiscall__     __attribute__((thiscall))

#else

    #define __vd_asm__
    #define __vd_inline__       inline
    #define __vd_inline_force__ inline
    #define __vd_func__         ""
    #define __vd_file__         ""
    #define __vd_date__         ""
    #define __vd_time__         ""
    #define __vd_line__         (0)

    #define __vd_cdecl__
    #define __vd_stdcall__
    #define __vd_fastcall__
    #define __vd_thiscall__
    #define __vd_packed__
    #define __vd_aligned__(a)

#endif

/*! @def __vd_cpu_aligned__
 *
 * the cpu byte alignment
 */
#if (VD_CPU_BITBYTE == 8)
    #define __vd_cpu_aligned__ __vd_aligned__(8)
#elif (VD_CPU_BITBYTE == 4)
    #define __vd_cpu_aligned__ __vd_aligned__(4)
#elif (VD_CPU_BITBYTE == 2)
    #define __vd_cpu_aligned__ __vd_aligned__(2)
#else
    #error unknown cpu bytes
#endif

// like
#if defined(VD_COMPILER_IS_GCC) && VD_COMPILER_VERSION_BT(2, 0)
    #define __vd_likely__(x)   __builtin_expect((x), 1)
    #define __vd_unlikely__(x) __builtin_expect((x), 0)
#else
    #define __vd_likely__(x)   (x)
    #define __vd_unlikely__(x) (x)
#endif

// // debug
// #ifdef __vd_debug__
// #    define __vd_debug_decl__ , vd_char_t const *func_, vd_size_t line_, vd_char_t const *file_
// #    define __vd_debug_vals__ , __vd_func__, __vd_line__, __vd_file__
// #    define __vd_debug_args__ , func_, line_, file_
// #else
// #    define __vd_debug_decl__
// #    define __vd_debug_vals__
// #    define __vd_debug_args__
// #endif

// // small
// #undef __vd_small__
// #ifdef VD_CONFIG_SMALL
// #    define __vd_small__
// #endif

// newline
#ifdef VD_CONFIG_OS_WINDOWS
    #define __vd_newline__ "\r\n"
#else
    #define __vd_newline__ "\n"
#endif

// // the string only for the large mode
// #ifdef __vd_small__
// #    define __vd_lstring__(x) vd_null
// #else
// #    define __vd_lstring__(x) x
// #endif

// // the string only for the debug mode
// #ifdef __vd_debug__
// #    define __vd_dstring__(x) x
// #else
// #    define __vd_dstring__(x) vd_null
// #endif

// extern c
#ifdef __cplusplus
    #define __vd_extern_c__       extern "C"
    #define __vd_extern_c_enter__ extern "C" {
    #define __vd_extern_c_leave__ }
#else
    #define __vd_extern_c__
    #define __vd_extern_c_enter__
    #define __vd_extern_c_leave__
#endif

// export for the shared library
#if defined(VD_COMPILER_IS_MSVC)
    #define __vd_export__ __declspec(dllexport)
#elif defined(VD_COMPILER_IS_GCC) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
    #define __vd_export__ __attribute__((visibility("default")))
#else
    #define __vd_export__
#endif

#if defined(VD_COMPILER_IS_GCC) && VD_COMPILER_VERSION_BE(3, 0)
    #define __vd_deprecated__ __attribute__((deprecated))
#elif defined(VD_COMPILER_IS_MSVC) && defined(_MSC_VER) && _MSC_VER >= 1300
    #define __vd_deprecated__ __declspec(deprecated)
#else
    #define __vd_deprecated__
#endif

// has feature
#ifdef __has_feature
    #define __vd_has_feature__(x) __has_feature(x)
#else
    #define __vd_has_feature__(x) 0
#endif

// has include
#ifdef __has_include
    #define __vd_has_include__(x)                            __has_include(x)
#else
    #define __vd_has_include__(x) 0
#endif

// has builtin
#ifdef __has_builtin
    #define __vd_has_builtin__(x) __has_builtin(x)
#else
    #define __vd_has_builtin__(x) 0
#endif

// no_sanitize_address
#if __vd_has_feature__(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
    #define __vd_no_sanitize_address__ __attribute__((no_sanitize_address))
#else
    #define __vd_no_sanitize_address__
#endif

// thread local
#if __vd_has_feature__(c_thread_local)
    #define __vd_thread_local__ _Thread_local
#endif

/* c function overloadable
 *
 * @code
    static __vd_inline__ vd_void_t test(vd_int_t a) __vd_overloadable__
    {
        vd_trace_i("test1: %d", a);
    }
    static __vd_inline__ vd_void_t test(vd_int_t a, vd_int_t b) __vd_overloadable__
    {
        vd_trace_i("test2: %d %d", a, b);
    }
 * @endcode
 *
 * If the compiler does not support __vd_overloadable__, we can use the following code to implement function overload.
 *
 * @code
    #define test_n(a, b, ...) test_impl(a, b)
    #define test(a, args ...) test_n(a, ##args, 0, 0, 0)
    static __vd_inline__ vd_void_t test_impl(vd_int_t a, vd_int_t b)
    {
        vd_trace_i("test: %d %d", a, b);
    }

    test(1);
    test(1, 2);
 * @endcode
 */
#if defined(VD_COMPILER_IS_GCC) || defined(VD_COMPILER_IS_CLANG)
    #define __vd_overloadable__ __attribute__((overloadable))
#else
    #define __vd_overloadable__
#endif

/*! the type reference keyword for defining vd_xxxx_ref_t
 *
 * typedef __vd_typeref__(xxxx);
 *
 *
 * suppress gcc 4.9 on c++ codes warning: '__vd_yyyy_t' has a field '__vd_yyyy_t::xxxx' whose type uses the anonymous
 namespace
 *
 * @code
 *
   typedef struct{}*    vd_xxxx_ref_t;

   typedef struct __vd_yyyy_t
   {
       vd_xxxx_ref_t    xxxx;

   }__vd_yyyy_t;

 *
 *
 * @endcode
 *
 */
#define __vd_typeref__(object)       \
    struct __vd_##object##_dummy_t { \
        vd_int_t dummy;              \
    } const* vd_##object##_ref_t

// macros
#define __vd_mstring__(x)    #x
#define __vd_mstring_ex__(x) __vd_mstring__(x)

#define __vd_mconcat__(a, b)    a##b
#define __vd_mconcat_ex__(a, b) __vd_mconcat__(a, b)

#define __vd_mconcat3__(a, b, c)    a##b##c
#define __vd_mconcat3_ex__(a, b, c) __vd_mconcat3__(a, b, c)

#define __vd_mconcat4__(a, b, c, d)    a##b##c##d
#define __vd_mconcat4_ex__(a, b, c, d) __vd_mconcat4__(a, b, c, d)

#define __vd_mconcat5__(a, b, c, d, e)    a##b##c##d##e
#define __vd_mconcat5_ex__(a, b, c, d, e) __vd_mconcat5__(a, b, c, d, e)

#define __vd_mconcat6__(a, b, c, d, e, f)    a##b##c##d##e##f
#define __vd_mconcat6_ex__(a, b, c, d, e, f) __vd_mconcat6__(a, b, c, d, e, f)

#define __vd_mconcat7__(a, b, c, d, e, f, g)    a##b##c##d##e##f##g
#define __vd_mconcat7_ex__(a, b, c, d, e, f, g) __vd_mconcat7__(a, b, c, d, e, f, g)

#define __vd_mconcat8__(a, b, c, d, e, f, g, h)    a##b##c##d##e##f##g##h
#define __vd_mconcat8_ex__(a, b, c, d, e, f, g, h) __vd_mconcat8__(a, b, c, d, e, f, g, h)

#define __vd_mconcat9__(a, b, c, d, e, f, g, h, i)    a##b##c##d##e##f##g##h##i
#define __vd_mconcat9_ex__(a, b, c, d, e, f, g, h, i) __vd_mconcat9__(a, b, c, d, e, f, g, h, i)

#define __vd_mstrcat__(a, b)                       a b
#define __vd_mstrcat3__(a, b, c)                   a b c
#define __vd_mstrcat4__(a, b, c, d)                a b c d
#define __vd_mstrcat5__(a, b, c, d, e)             a b c d e
#define __vd_mstrcat6__(a, b, c, d, e, f)          a b c d e f
#define __vd_mstrcat7__(a, b, c, d, e, f, g)       a b c d e f g
#define __vd_mstrcat8__(a, b, c, d, e, f, g, h)    a b c d e f g h
#define __vd_mstrcat9__(a, b, c, d, e, f, g, h, i) a b c d e f g h i
#endif

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
#endif  // BASE_KEYWORD_H
