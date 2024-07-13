/*
 * Project: linux-kit
 * Moudle: common
 * File: common_random.h
 * Created Date: 2021-01-28 14:04:21
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_RANDOM_H
#define COMMON_RANDOM_H

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

// �������int����������[0, RAND_MAX]
vd_int_t common_rand_i();

/* �������һ��int����������[min_val, max_val��
max_val = max(min_val, max_val)  min_val = min(min_val, max_val)*/
vd_int_t common_rand_i_range(vd_int_t min_val, vd_int_t max_val);

/* �������һ��double���� ������(min_val, max_val)
max_val = max(min_val, max_val)  min_val = min(min_val, max_val) */
vd_double_t common_rand_d_range(vd_double_t min_val, vd_double_t max_val);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_RANDOM_H