/*
 * Project: linux-kit
 * Moudle: BASE
 * File: base_limits.h
 * Created Date: 2021-01-27 10:14:41
 * Author: ShiBin
 * Description: 极限值定义
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef BASE_LIMITS_H
#define BASE_LIMITS_H

/* ======================================================================================
 * includes
 */
#include "base_type.h"
#include "base_keyword.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
#define VD_PATH_MAX_LEN 256

#define VD_MAX8  ((vd_int8_t)(0x7f))
#define VD_MIN8  ((vd_int8_t)(0x81))
#define VD_MAXU8 ((vd_uint8_t)(0xff))
#define VD_MINU8 ((vd_uint8_t)(0))

#define VD_MAX16  ((vd_int16_t)(0x7fff))
#define VD_MIN16  ((vd_int16_t)(0x8001))
#define VD_MAXU16 ((vd_uint16_t)(0xffff))
#define VD_MINU16 ((vd_uint16_t)(0))

#define VD_MAX32  ((vd_int32_t)(0x7fffffff))
#define VD_MIN32  ((vd_int32_t)(0x80000001))
#define VD_MAXU32 ((vd_uint32_t)(0xffffffff))
#define VD_MINU32 ((vd_uint32_t)(0))

#define VD_MAX64  ((vd_int64_t)(0x7fffffffffffffffLL))
#define VD_MIN64  ((vd_int64_t)(0x8000000000000001LL))
#define VD_MAXU64 ((vd_uint64_t)(0xffffffffffffffffULL))
#define VD_MINU64 ((vd_uint64_t)(0))

#define VD_NAN32 (0x80000000)

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // BASE_LIMITS_H