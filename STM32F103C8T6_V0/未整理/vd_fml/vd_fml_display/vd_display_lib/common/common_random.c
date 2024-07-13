/*
 * Project: linux-kit
 * Moudle: common
 * File: common_random.c
 * Created Date: 2021-01-28 14:05:13
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
#include "common_random.h"

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
vd_int_t common_rand_i() {
    static vd_bool_t flag = vd_false;

    // 种子设置一次
    if (!flag) {
        flag = vd_true;
        srand(time(vd_null));
    }

    return rand();
}

// 产生一个随机数 在区间 [min_val, max_val）,包含最小值，不包含最大值
vd_int_t common_rand_i_range(vd_int_t min_val, vd_int_t max_val) {
    if (min_val == max_val) {
        return min_val;
    }

    if (min_val > max_val) {
        return common_rand_i_range(max_val, min_val);
    }

    return common_rand_i() % (max_val - min_val) + min_val;
}

// 产生一个double 在区间[min_val, max_val）；
vd_double_t common_rand_d_range(vd_double_t min_val, vd_double_t max_val) {
    if (min_val > max_val) {
        return common_rand_d_range(max_val, min_val);
    }
    if (min_val == max_val) {
        return min_val;
    }

    return min_val + common_rand_i() / ((vd_double_t)RAND_MAX / (max_val - min_val));
}