/*
 * Project: linux-kit
 * Moudle: commond
 * File: common_strings.h
 * Created Date: 2021-01-28 14:05:52
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

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

/* ======================================================================================
 * declaration
 */

vd_char_t* common_str_upper(vd_char_t* str);
vd_char_t* common_str_lower(vd_char_t* str);
vd_char_t* common_str_reverse(vd_char_t* str);

vd_bool_t common_str_starts_with(const vd_char_t* str, const vd_char_t* start);
vd_bool_t common_str_ends_with(const vd_char_t* str, const vd_char_t* end);
vd_bool_t common_str_contains(const vd_char_t* str, const vd_char_t* sub);

// strncpy n = sizeof(dest_buf)-1
// common_safe_strncpy n = sizeof(dest_buf)
vd_char_t* common_safe_strncpy(vd_char_t* dest, const vd_char_t* src, size_t n);

// strncat n = sizeof(dest_buf)-1-strlen(dest)
// common_safe_strncpy n = sizeof(dest_buf)
vd_char_t* common_safe_strncat(vd_char_t* dest, const vd_char_t* src, size_t n);

#define strrchr_dot(str) strrchr(str, '.')
vd_char_t* strrchr_dir(const vd_char_t* filepath);

vd_int_t common_str2array(vd_char_t* str, vd_uint8_t* array);

vd_uint32_t common_arrary2str(vd_char_t* str, vd_uint8_t* array, vd_uint32_t array_size);

vd_uint32_t common_str_contain_count(const vd_char_t* str, const vd_char_t* sub);

vd_uint32_t common_str_split(vd_char_t* str, vd_char_t* delim, vd_char_t*** str_array);

vd_void_t common_str_split_free(vd_char_t** str_array, vd_uint32_t array_size);

vd_int32_t common_strcmp(const vd_char_t* str, const vd_char_t* sub);
/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_STRINGS_H
