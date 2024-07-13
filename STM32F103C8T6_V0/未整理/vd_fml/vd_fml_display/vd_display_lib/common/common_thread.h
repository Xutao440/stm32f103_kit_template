/*
 * Project: ite
 * Module:
 * File: common_thread.h
 * Created Date: 2021-08-05 10:55:59
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

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
typedef vd_void_t* (*start_routine_t)(vd_void_t* args);

/* ======================================================================================
 * declaration
 */
vd_bool_t common_thread_start(start_routine_t routine, vd_void_t* args);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_THREAD_H