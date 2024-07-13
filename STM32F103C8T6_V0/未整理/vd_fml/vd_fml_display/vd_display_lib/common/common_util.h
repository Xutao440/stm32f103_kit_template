/*
 * Project: DT1866
 * Module: common util
 * File: common_util.h
 * Created Date: 2021-08-04 11:44:28
 * Author: meizhen
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

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
#define CRC32_INIT 0xffffffffL
/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

vd_void_t common_free(vd_void_t* ptr);

vd_uint8_t common_sum8(vd_uint8_t* dst, vd_uint32_t len);

vd_uint16_t common_sum16(vd_uint8_t* dst, vd_uint32_t len);

vd_uint32_t common_sum32(vd_uint8_t* dst, vd_uint32_t len);

vd_uint8_t common_crc8(vd_uint8_t* data, vd_uint32_t len);

vd_uint16_t common_crc16(vd_uint8_t* data, vd_uint32_t len);

uint16_t common_crc16_gen(uint16_t last, const vd_uint8_t* data, vd_uint32_t len);

vd_uint32_t common_crc32(vd_uint8_t* data, vd_uint32_t len);

vd_uint32_t common_crc32_gen(vd_uint32_t* crc, vd_uint8_t* data, vd_uint32_t len);

vd_uint8_t common_bbc8(vd_uint8_t* pStart, vd_uint32_t uSize);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_UTIL_H