/*
 * Project: linux-kit
 * Moudle: common
 * File: common_time.c
 * Created Date: 2021-01-28 14:07:00
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
#include "common_time.h"

#include "log.h"
#include "platform_config.h"

#if (KIT_PLATFORM == KIT_RF01)
#elif (KIT_PLATFORM == KIT_VDCHIP5005 || KIT_PLATFORM == KIT_VDCHIP4005)
#include "kit_driver_bsp_config.h"
#endif

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
#if (KIT_PLATFORM == KIT_RF01)
#elif (KIT_PLATFORM == KIT_VDCHIP5005 || KIT_PLATFORM == KIT_VDCHIP4005)
#include "driver_system.h"
#else
static const vd_char_t* s_weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

static const vd_char_t* s_months[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"};

//                                  1       3       5       7   8       10      12
static const vd_uint8_t s_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#endif

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */

vd_ulong_t common_time_get_current_time_ms() {
#if (KIT_PLATFORM == KIT_VDCHIP5005 || KIT_PLATFORM == KIT_VDCHIP4005)	
		return vd_get_sys_curr_time();
#else
    struct timeval tv;
    gettimeofday(&tv, vd_null);
    return tv.tv_sec * (vd_ulong_t)1000 + tv.tv_usec / 1000;
#endif
}

vd_void_t common_time_delay(vd_uint32_t count) {
    vd_uint32_t i;
    for (i = 0; i < count * 1000; i++) __nop();  // for(int j=0;j<255;j++);
}

vd_void_t common_time_sleep(vd_uint32_t count) {
    vd_uint32_t i;
    for (i = 0; i < count * 1000; i++) __nop();  // for(int j=0;j<255;j++);
}

#if (KIT_PLATFORM == KIT_RF01)
#elif (KIT_PLATFORM == KIT_VDCHIP5005 || KIT_PLATFORM == KIT_VDCHIP4005)	
#else
vd_ulong_t common_time_get_current_time_us() {
    struct timeval tv;
    gettimeofday(&tv, vd_null);
    return tv.tv_sec * (vd_ulong_t)1000000 + tv.tv_usec;
}

bool common_time_datetime_now(datetime_t* dt) {
    vd_check_return_val(dt == vd_null, vd_false);
    struct timeval tv;
    struct tm*     tm = vd_null;
    gettimeofday(&tv, vd_null);
    vd_time_t tt = tv.tv_sec;
    tm           = localtime(&tt);

    dt->year  = tm->tm_year + 1900;
    dt->month = tm->tm_mon + 1;
    dt->day   = tm->tm_mday;
    dt->hour  = tm->tm_hour;
    dt->min   = tm->tm_min;
    dt->sec   = tm->tm_sec;
    dt->ms    = tv.tv_usec / 1000;
    return vd_true;
}

vd_int_t common_time_days_of_month(vd_int_t month, vd_int_t year) {
    vd_check_return_val(month < 1 || month > 12, -1);
    vd_int_t days = s_days[month - 1];
    return (month == 2 && COMMON_TIME_IS_LEAP_YEAR(year)) ? ++days : days;
}

datetime_t* common_time_datetime_past(datetime_t* dt, vd_int_t days) {
    vd_check_return_val(dt == vd_null || days < 0, vd_null);
    vd_int_t sub = days;
    while (sub) {
        if (dt->day > sub) {
            dt->day -= sub;
            break;
        }
        sub -= dt->day;
        if (--dt->month == 0) {
            dt->month = 12;
            --dt->year;
        }
        dt->day = common_time_days_of_month(dt->month, dt->year);
    }
    return dt;
}

datetime_t* common_time_datetime_future(datetime_t* dt, vd_int_t days) {
    vd_check_return_val(dt == vd_null || days < 0, vd_null);
    vd_int_t sub = days;
    vd_int_t mdays;
    while (sub) {
        mdays = common_time_days_of_month(dt->month, dt->year);
        if (dt->day + sub <= mdays) {
            dt->day += sub;
            break;
        }
        sub -= (mdays - dt->day + 1);
        if (++dt->month > 12) {
            dt->month = 1;
            ++dt->year;
        }
        dt->day = 1;
    }
    return dt;
}

vd_time_t common_time_datetime_mktime(datetime_t* dt) {
    vd_check_return_val(dt == vd_null, -1);
    struct tm tm;
    vd_time_t ts;
    time(&ts);
    struct tm* ptm = localtime(&ts);
    memcpy(&tm, ptm, sizeof(struct tm));
    tm.tm_yday = dt->year - 1900;
    tm.tm_mon  = dt->month - 1;
    tm.tm_mday = dt->day;
    tm.tm_hour = dt->hour;
    tm.tm_min  = dt->min;
    tm.tm_sec  = dt->sec;
    return mktime(&tm);
}

vd_char_t* common_time_duration_fmt(vd_int_t sec, vd_char_t* buf) {
    vd_check_return_val(buf == vd_null, vd_null);
    vd_int_t hours, minutes, seconds;
    minutes = sec / 60;
    seconds = sec % 60;
    hours   = minutes / 60;
    minutes = minutes % 60;
    sprintf(buf, TIME_FMT, hours, minutes, seconds);
    return buf;
}

vd_char_t* common_time_datetime_fmt(datetime_t* dt, vd_char_t* buf) {
    vd_check_return_val(dt == vd_null || buf == vd_null, vd_null);
    sprintf(buf, DATETIME_FMT, dt->year, dt->month, dt->day, dt->hour, dt->min, dt->sec);
    return buf;
}

vd_char_t* common_time_gmtime_fmt(vd_time_t time, vd_char_t* buf) {
    vd_check_return_val(buf == vd_null, vd_null);
    struct tm* tm = gmtime(&time);
    // strftime(buf, GMTIME_FMT_BUFLEN, "%a, %d %b %Y %H:%M:%S GMT", tm);
    sprintf(
        buf,
        GMTIME_FMT,
        s_weekdays[tm->tm_wday],
        tm->tm_mday,
        s_months[tm->tm_mon],
        tm->tm_year + 1900,
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec);
    return buf;
}

vd_int_t common_time_get_month_by_str(const vd_char_t* month) {
    vd_check_return_val(month == vd_null || strlen(month) < 3, -1);
    for (vd_int_t i = 0; i < 12; ++i) {
        if (strncasecmp(month, s_months[i], strlen(month)) == 0) {
            return i + 1;
        }
    }
    return 0;
}

const vd_char_t* common_time_get_month_str(vd_int_t month) {
    vd_check_return_val(month >= 1 && month <= 12, vd_null);
    return s_months[month - 1];
}

vd_int_t common_time_get_weekday_by_str(const vd_char_t* weekday) {
    vd_check_return_val(weekday == vd_null || strlen(weekday) < 3, -1);
    for (vd_size_t i = 0; i < 7; ++i) {
        if (strncasecmp(weekday, s_weekdays[i], strlen(weekday)) == 0) return i;
    }
    return -1;
}

const vd_char_t* common_time_get_weekday_str(vd_int_t weekday) {
    vd_check_return_val(weekday >= 0 && weekday <= 7, vd_null);
    if (weekday == 7) {
        weekday = 0;
    }
    return s_weekdays[weekday];
}

datetime_t common_time_get_compile_datetime() {
    datetime_t dt;
    vd_char_t  month[32];
    sscanf(__DATE__, "%s %d %d", month, &dt.day, &dt.year);
    sscanf(__TIME__, "%d:%d:%d", &dt.hour, &dt.min, &dt.sec);
    dt.month = common_time_get_month_by_str(month);
    return dt;
}

vd_time_t common_time_cron_next_timeout(vd_int_t minute, vd_int_t hour, vd_int_t day, vd_int_t week, vd_int_t month) {
    enum {
        UNKOWN,
        HOURLY,
        DAILY,
        WEEKLY,
        MONTHLY,
        YEARLY,
    } period_type = UNKOWN;
    struct tm tm;
    vd_time_t tt;
    time(&tt);
    tm                 = *localtime(&tt);
    vd_time_t tt_round = 0;

    tm.tm_sec = 0;
    if (minute >= 0) {
        period_type = HOURLY;
        tm.tm_min   = minute;
    }
    if (hour >= 0) {
        period_type = DAILY;
        tm.tm_hour  = hour;
    }
    if (week >= 0) {
        period_type = WEEKLY;
    } else if (day > 0) {
        period_type = MONTHLY;
        tm.tm_mday  = day;
        if (month > 0) {
            period_type = YEARLY;
            tm.tm_mon   = month - 1;
        }
    }

    if (period_type == UNKOWN) {
        return -1;
    }

    tt_round = mktime(&tm);
    if (week >= 0) {
        tt_round = tt + (week - tm.tm_wday) * SECONDS_PER_DAY;
    }
    if (tt_round > tt) {
        return tt_round;
    }

    switch (period_type) {
        case HOURLY:
            tt_round += SECONDS_PER_HOUR;
            return tt_round;
        case DAILY:
            tt_round += SECONDS_PER_DAY;
            return tt_round;
        case WEEKLY:
            tt_round += SECONDS_PER_WEEK;
            return tt_round;
        case MONTHLY:
            if (++tm.tm_mon == 12) {
                tm.tm_mon = 0;
                ++tm.tm_year;
            }
            break;
        case YEARLY:
            ++tm.tm_year;
            break;
        default:
            return -1;
    }

    return mktime(&tm);
}
#endif