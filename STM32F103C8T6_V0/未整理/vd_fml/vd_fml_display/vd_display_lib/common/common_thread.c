/*
 * Project: ite
 * Module:
 * File: common_thread.c
 * Created Date: 2021-08-05 10:56:18
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
#include "common_thread.h"

#include <pthread.h>

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
vd_bool_t common_thread_start(start_routine_t routine, vd_void_t* args) {
    pthread_t      thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    return pthread_create(&thread, &attr, routine, args) == 0 ? vd_true : vd_false;
}
