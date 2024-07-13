/*
 * Project: linux-kit
 * Moudle: path
 * File: common_path.h
 * Created Date: 2021-01-28 10:57:56
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_PATH_H
#define COMMON_PATH_H

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

const vd_char_t* common_path_basename(const vd_char_t* filepath);

const vd_char_t* common_path_suffixname(const vd_char_t* filename);

const vd_char_t* common_path_get_file_name(vd_char_t* filepath);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_PATH_H