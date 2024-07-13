/*
 * Project: linux-kit
 * Moudle: common
 * File: common_time.h
 * Created Date: 2021-01-28 14:07:41
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_TIME_H
#define COMMON_TIME_H

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
#if (USE_PLATFORM == MCU)
#else
#define SECONDS_PER_HOUR (60 * 60)           // 一小时多少秒
#define SECONDS_PER_DAY  (24 * 60 * 60)      // 一天多少秒
#define SECONDS_PER_WEEK (7 * 24 * 60 * 60)  // 一天多少秒

#define COMMON_TIME_IS_LEAP_YEAR(year) (((year) % 4 == 0 && (year) % 100 != 0) || (year) % 100 == 0)

#define TIME_FMT        "%02d:%02d:%02d"
#define TIME_FMT_BUFLEN 12

#define DATETIME_FMT        "%04d-%02d-%02d %02d:%02d:%02d"
#define DATETIME_FMT_BUFLEN 24

#define GMTIME_FMT        "%.3s, %02d %.3s %04d %02d:%02d:%02d GMT"
#define GMTIME_FMT_BUFLEN 30
#endif

/* ======================================================================================
 * types
 */
#if (USE_PLATFORM == MCU)
#else
typedef struct datetime_s {
    vd_int_t year;
    vd_int_t month;
    vd_int_t day;
    vd_int_t hour;
    vd_int_t min;
    vd_int_t sec;
    vd_int_t ms;
} datetime_t;
#endif

/* ======================================================================================
 * declaration
 */

#if (USE_PLATFORM == MCU)
#else
#ifdef _MSC_VER
/* @see winsock2.h
// Structure used in select() call, taken from the BSD file sys/time.h
struct timeval {
    long    tv_sec;
    long    tv_usec;
};
*/

struct timezone {
    vd_int_t tz_minuteswest; /* of Greenwich */
    vd_int_t tz_dsttime;     /* type of dst correction to apply */
};

#include <sys/timeb.h>
static inline vd_int_t gettimeofday(struct timeval* tv, struct timezone* tz) {
    struct _timeb tb;
    _ftime(&tb);
    if (tv) {
        tv->tv_sec  = (long)tb.time;
        tv->tv_usec = tb.millitm * 1000;
    }
    if (tz) {
        tz->tz_minuteswest = tb.timezone;
        tz->tz_dsttime     = tb.dstflag;
    }
    return 0;
}
#endif

// sleep(s), msleep(ms), usleep(us)
#ifdef OS_WIN
static inline vd_void_t sleep(vd_int_t s) {
    Sleep(s * 1000);
}

static inline vd_void_t msleep(vd_int_t ms) {
    Sleep(ms);
}

static inline vd_void_t usleep(vd_int_t us) {
    Sleep(us / 1000);
}
#else
static inline vd_void_t msleep(vd_int_t ms) {
    usleep(ms * 1000);
}
#endif
#endif

// ms
vd_ulong_t common_time_get_current_time_ms();

vd_void_t common_time_delay(vd_uint32_t count);

#if (USE_PLATFORM == MCU)
#else
// us
vd_ulong_t common_time_get_current_time_us();

/**
 * @brief 获取当前时间的datetime_t
 *
 * @return datetime_t
 */
bool common_time_datetime_now(datetime_t* dt);

datetime_t* common_time_datetime_past(datetime_t* dt, vd_int_t days);
datetime_t* common_time_datetime_future(datetime_t* dt, vd_int_t days);

/**
 * @brief datetime_t 转 vd_time_t
 *
 * @param dt
 * @return vd_time_t
 */
vd_time_t common_time_datetime_mktime(datetime_t* dt);

vd_char_t* common_time_duration_fmt(vd_int_t sec, vd_char_t* buf);

vd_char_t* common_time_datetime_fmt(datetime_t* dt, vd_char_t* buf);

vd_char_t* common_time_gmtime_fmt(vd_time_t time, vd_char_t* buf);

vd_int_t common_time_days_of_month(vd_int_t month, vd_int_t year);

/**
 * @brief 通过‘月份字符串’获取对应的月份时间
 *
 * @param month
 * @return vd_int_t
 */
vd_int_t         common_time_get_month_by_str(const vd_char_t* month);
const vd_char_t* common_time_get_month_str(vd_int_t month);

vd_int_t         common_time_get_weekday_by_str(const vd_char_t* weekday);
const vd_char_t* common_time_get_weekday_str(vd_int_t weekday);

datetime_t common_time_get_compile_datetime();

/*
 * minute   hour    day     week    month       action
 * 0~59     0~23    1~31    0~6     1~12
 *  30      -1      -1      -1      -1          cron.hourly
 *  30      1       -1      -1      -1          cron.daily
 *  30      1       15      -1      -1          cron.monthly
 *  30      1       -1       0      -1          cron.weekly
 *  30      1        1      -1      10          cron.yearly
 */
vd_time_t common_time_cron_next_timeout(vd_int_t minute, vd_int_t hour, vd_int_t day, vd_int_t week, vd_int_t month);
#endif

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_TIME_H