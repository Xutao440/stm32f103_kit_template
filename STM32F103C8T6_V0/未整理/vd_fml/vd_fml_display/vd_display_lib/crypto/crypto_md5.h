/*
 * Project: linux-kit
 * Moudle: crypto
 * File: crypto_md5.h
 * Created Date: 2021-01-28 19:30:00
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef CRYPTO_MD5_H
#define CRYPTO_MD5_H

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

/* ======================================================================================
 * types
 */

/* MD5 context. */
typedef struct {
    vd_uint32_t state[4];   /* state (ABCD) */
    vd_uint32_t count[2];   /* number of bits, modulo 2^64 (lsb first) */
    vd_uchar_t  buffer[64]; /* input buffer */
} md5_ctx_t;

/* ======================================================================================
 * declaration
 */
vd_void_t crypto_md5_init(md5_ctx_t* context);
vd_void_t crypto_md5_update(md5_ctx_t* context, vd_uchar_t* input, vd_uint_t inputLen);
vd_void_t crypto_md5_final(vd_uchar_t digest[16], md5_ctx_t* context);
vd_int_t  crypto_md5_file(vd_uchar_t* file_name, vd_uchar_t* md5);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // CRYPTO_MD5_H