/*
 * Project: linux-kit
 * Moudle: BASE
 * File: base_check.h
 * Created Date: 2021-01-27 09:59:48
 * Author: ShiBin
 * Description: 常用的检查宏
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef BASE_BASE_CHECK_H
#define BASE_BASE_CHECK_H

#include "log.h"
/* ======================================================================================
 * includes
 */

/* ======================================================================================
 * extern
 */

/* ======================================================================================
 * macros
 */
/**
 * @brief 检查之后结束，return不带返回值的
 *
 */
#define vd_check_return(expression) \
    if (expression) {               \
        return;                     \
    }

#define vd_check_return_debug(expression, ...)   \
    if (expression) {                            \
        log_debug(#expression "; " __VA_ARGS__); \
        return;                                  \
    }

#define vd_check_return_warn(expression, ...)   \
    if (expression) {                           \
        log_warn(#expression "; " __VA_ARGS__); \
        return;                                 \
    }

#define vd_check_return_error(expression, ...)   \
    if (expression) {                            \
        log_error(#expression "; " __VA_ARGS__); \
        return;                                  \
    }

/**
 * @brief 检查之后结束，return带返回值的
 *
 */
#define vd_check_return_val(expression, val) \
    if (expression) {                        \
        return val;                          \
    }

#define vd_check_return_val_debug(expression, val, ...) \
    if (expression) {                                   \
        log_debug(#expression "; " __VA_ARGS__);        \
        return val;                                     \
    }

#define vd_check_return_val_warn(expression, val, ...) \
    if (expression) {                                  \
        log_warn(#expression "; " __VA_ARGS__);        \
        return val;                                    \
    }

#define vd_check_return_val_error(expression, val, ...) \
    if (expression) {                                   \
        log_error(#expression "; " __VA_ARGS__);        \
        return val;                                     \
    }

/**
 * @brief 检查之后goto转跳
 *
 */
#define vd_check_goto(expression, label) \
    if (expression) goto label;

#define vd_check_goto_debug(expression, label, ...) \
    if (expression) {                               \
        log_debug(#expression "; " __VA_ARGS__);    \
        goto label;                                 \
    }

#define vd_check_goto_warn(expression, label, ...) \
    if (expression) {                              \
        log_warn(#expression "; " __VA_ARGS__);    \
        goto label;                                \
    }

#define vd_check_goto_error(expression, label, ...) \
    if (expression) {                               \
        log_error(#expression "; " __VA_ARGS__);    \
        goto label;                                 \
    }

/**
 * @brief 检查之后break
 *
 */
#define vd_check_break(expression) \
    if (expression) break;

#define vd_check_break_debug(expression, ...)    \
    if (expression) {                            \
        log_debug(#expression "; " __VA_ARGS__); \
        break;                                   \
    }
#define vd_check_break_warn(expression, ...)    \
    if (expression) {                           \
        log_warn(#expression "; " __VA_ARGS__); \
        break;                                  \
    }
#define vd_check_break_error(expression, ...)    \
    if (expression) {                            \
        log_error(#expression "; " __VA_ARGS__); \
        break;                                   \
    }

#define vd_check_continue(expression) \
    if (expression) continue;

#define vd_check_continue_debug(expression, ...) \
    if (expression) {                            \
        log_debug(#expression "; " __VA_ARGS__); \
        continue;                                \
    }

#define vd_check_continue_warn(expression, ...) \
    if (expression) {                           \
        log_warn(#expression "; " __VA_ARGS__); \
        continue;                               \
    }

#define vd_check_continue_error(expression, ...) \
    if (expression) {                            \
        log_error(#expression "; " __VA_ARGS__); \
        continue;                                \
    }

#define vd_check_break_state(expression, state, val) \
    if (expression) {                                \
        (state) = (val);                             \
        break;                                       \
    }

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * extern
 */
#endif  // BASE_BASE_CHECK_H
