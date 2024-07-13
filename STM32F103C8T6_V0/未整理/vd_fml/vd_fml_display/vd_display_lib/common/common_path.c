/*
 * Project: linux-kit
 * Moudle: module_name
 * File: common_path.c
 * Created Date: 2021-01-28 10:58:41
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "common_path.h"

#include "common_strings.h"
#include "log.h"

/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */
const vd_char_t* common_path_basename(const vd_char_t* filepath) {
    const vd_char_t* pos = strrchr_dir(filepath);
    return pos ? pos + 1 : filepath;
}

const vd_char_t* common_path_suffixname(const vd_char_t* filename) {
    const vd_char_t* pos = strrchr_dot(filename);
    return pos ? pos + 1 : "";
}

const vd_char_t* common_path_get_file_name(vd_char_t* filepath) {
    vd_check_return_val_error(vd_null == filepath, vd_null, "Parameter error");

    vd_char_t* p    = vd_null;
    vd_char_t* pmov = vd_null;

    p = pmov = filepath;
    while ((pmov = strstr(p, "/")) != vd_null) {
        p = pmov;
        p = p + 1;
    }

    return p;
}