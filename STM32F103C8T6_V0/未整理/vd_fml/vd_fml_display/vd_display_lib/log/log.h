/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "base.h"

#define LOG_VERSION "0.1.0"

#if 1  //(USE_PLATFORM == MCU)
#include "vd_log.h"
#else
#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, DIR_SEPARATOR) ? strrchr(__FILE__, DIR_SEPARATOR) + 1 : __FILE__)
#endif

typedef struct {
    va_list     ap;
    const char* fmt;
    const char* file;
    const char* function;
    struct tm*  time;
    void*       udata;
    int         line;
    int         level;
} log_Event;

typedef void (*log_LogFn)(log_Event* ev);
typedef void (*log_LockFn)(bool lock, void* udata);

enum {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
};

#define log_trace(...) log_log(LOG_LEVEL_TRACE, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_LEVEL_DEBUG, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_LEVEL_INFO, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_LEVEL_WARN, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_error(...) log_log(LOG_LEVEL_ERROR, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_LEVEL_FATAL, __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__)

const char* log_level_string(int level);
void        log_set_lock(log_LockFn fn, void* udata);
void        log_set_level(int level);
void        log_set_quiet(bool enable);
int         log_add_callback(log_LogFn fn, void* udata, int level);
int         log_add_fp(FILE* fp, int level);

void log_log(int level, const char* file, int line, const char* function, const char* fmt, ...);

#endif
#endif
