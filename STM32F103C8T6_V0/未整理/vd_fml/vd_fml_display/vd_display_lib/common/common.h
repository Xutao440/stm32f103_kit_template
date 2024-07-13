/*
 * Project: linux-kit
 * Moudle: Common
 * File: common.h
 * Created Date: 2021-01-28 09:47:44
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_H
#define COMMON_H

/* ======================================================================================
 * includes
 */
#include "base.h"
#if (USE_PLATFORM == MCU)
#else
#include "common_endian.h"
#include "common_file.h"
#include "common_path.h"
#include "common_random.h"
#endif
#include "common_strings.h"
#if (USE_PLATFORM == MCU)
#else
#include "common_thread.h"
#endif
#if (USE_PLATFORM == MCU)
#include "common_util.h"
#include "common_time.h"
#else
// #include "common_time.h"
#endif

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

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_H
