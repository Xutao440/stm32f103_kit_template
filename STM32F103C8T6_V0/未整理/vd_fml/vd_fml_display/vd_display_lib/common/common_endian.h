/*
 * Project: linux-kit
 * Moudle: common
 * File: common_endian.h
 * Created Date: 2021-01-28 10:01:19
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_ENDIAN_H
#define COMMON_ENDIAN_H

/* ======================================================================================
 * includes
 */
#include "base.h"

/* ======================================================================================
 * extern
 */
__vd_extern_c_enter__;

/* ======================================================================================
 * macros
 */
#define VD_IS_BIG_END common_endian_is_big_endian()

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/**
 * @brief 是否是大端字节序
 *
 * @return vd_bool_t
 */
vd_bool_t common_endian_is_big_endian(vd_noarg_t);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_ENDIAN_H