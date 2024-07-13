#ifndef _VD_LOG_H_
#define _VD_LOG_H_
#include <stdio.h>
#include "platform_config.h"






#if (KIT_LOG == ON)
#define vd_log_init()					PLATFORM_LOG_INIT()
#define vd_logln(format, ...) PLATFORM_LOGLN(format, ##__VA_ARGS__)
#define vd_log(format, ...)   PLATFORM_LOG(format, ##__VA_ARGS__)
#else
#define vd_log_init() 				__NOP
#define vd_logln(format, ...) __NOP
#define vd_log(format, ...) 	__NOP
#endif


#if ABORT_LOG
#define log_abort(format, ...) vd_logln(format, ##__VA_ARGS__)
#else
#define log_abort(format, ...)
#endif

#if ERROR_LOG
#define log_error(format, ...) vd_logln(format, ##__VA_ARGS__)
#else
#define log_error(format, ...)
#endif

#if INFO_LOG
#define log_info(format, ...) vd_logln(format, ##__VA_ARGS__)
#else
#define log_info(format, ...)
#endif

#if DEBUG_LOG
#define log_debug(format, ...) vd_logln(format, ##__VA_ARGS__)
#else
#define log_debug(format, ...)
#endif

#if FATAL_LOG
#define log_fatal(format, ...) vd_logln(format, ##__VA_ARGS__)
#else
#define log_fatal(format, ...)
#endif


#define log_warn log_debug

#define log_array(tip, array, size)      \
    {                                    \
        vd_log("%s[%d]:", tip, size);    \
        for (int i = 0; i < size; i++) { \
            vd_log("%02x ", array[i]);    \
        }                                \
        vd_log("\r\n");                  \
    }

#define log_print(format, ...) vd_log(format, ##__VA_ARGS__)


#endif  //_DT_LOG_H_
