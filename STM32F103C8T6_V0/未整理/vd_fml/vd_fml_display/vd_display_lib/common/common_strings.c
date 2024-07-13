/*
 * Project: linux-kit
 * Moudle: common
 * File: common_strings.c
 * Created Date: 2021-01-28 14:06:40
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
#include "common_strings.h"

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

vd_char_t* common_str_upper(vd_char_t* str) {
    vd_char_t* p = str;
    while (*p != '\0') {
        if (*p >= 'a' && *p <= 'z') {
            *p &= ~0x20;
        }
        ++p;
    }
    return str;
}

vd_char_t* common_str_lower(vd_char_t* str) {
    vd_char_t* p = str;
    while (*p != '\0') {
        if (*p >= 'A' && *p <= 'Z') {
            *p |= 0x20;
        }
        ++p;
    }
    return str;
}

vd_char_t* common_str_reverse(vd_char_t* str) {
    if (str == vd_null) return vd_null;
    vd_char_t* b = str;
    vd_char_t* e = str;
    while (*e) {
        ++e;
    }
    --e;
    vd_char_t tmp;
    while (e > b) {
        tmp = *e;
        *e  = *b;
        *b  = tmp;
        --e;
        ++b;
    }
    return str;
}

vd_bool_t common_str_starts_with(const vd_char_t* str, const vd_char_t* start) {
#if (USE_PLATFORM == MCU)
#else
    assert(str != vd_null && start != vd_null);
#endif
    while (*str && *start && *str == *start) {
        ++str;
        ++start;
    }
    return *start == '\0';
}

vd_bool_t common_str_ends_with(const vd_char_t* str, const vd_char_t* end) {
#if (USE_PLATFORM == MCU)
#else
    assert(str != vd_null && end != vd_null);
#endif
    vd_int_t len1 = 0;
    vd_int_t len2 = 0;
    while (*str++) {
        ++len1;
    }
    while (*end++) {
        ++len2;
    }
    if (len1 < len2) return vd_false;
    while (len2-- > 0) {
        --str;
        --end;
        if (*str != *end) {
            return vd_false;
        }
    }
    return vd_true;
}

vd_bool_t common_str_contains(const vd_char_t* str, const vd_char_t* sub) {
#if (USE_PLATFORM == MCU)
#else
    assert(str != vd_null && sub != vd_null);
#endif
    return strstr(str, sub) != vd_null;
}

// n = sizeof(dest_buf)
vd_char_t* common_safe_strncpy(vd_char_t* dest, const vd_char_t* src, size_t n) {
#if (USE_PLATFORM == MCU)
#else
    assert(dest != vd_null && src != vd_null);
#endif
    vd_char_t* ret = dest;
    while (*src != '\0' && --n > 0) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret;
}

// n = sizeof(dest_buf)
vd_char_t* common_safe_strncat(vd_char_t* dest, const vd_char_t* src, size_t n) {
#if (USE_PLATFORM == MCU)
#else
    assert(dest != vd_null && src != vd_null);
#endif
    vd_char_t* ret = dest;
    while (*dest) {
        ++dest;
        --n;
    }
    while (*src != '\0' && --n > 0) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret;
}

vd_char_t* strrchr_dir(const vd_char_t* filepath) {
    vd_char_t* p = (vd_char_t*)filepath;
    while (*p) ++p;
    while (--p >= filepath) {
#ifdef OS_WIN
        if (*p == '/' || *p == '\\')
#else
        if (*p == '/')
#endif
            return p;
    }
    return vd_null;
}

/**
 * @brief : 十六进制字符串转换为整型数组
 *
 * @param str : 十六进制字符串
 * @param array : 整型数组
 * @return vd_int_t : 返回转换后整型数组大小
 */
vd_int_t common_str2array(vd_char_t* str, vd_uint8_t* array) {
    vd_char_t* p    = NULL;
    vd_char_t  high = 0;
    vd_char_t  low  = 0;
    vd_int_t   i    = 0;
    vd_int_t   len  = 0;

    vd_check_return_val(str == vd_null || array == vd_null, 0);

    p   = str;
    len = strlen(p);

    while (i < (len / 2)) {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low  = (*(++p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;

        array[i] = ((high & 0x0f) << 4 | (low & 0x0f));
        p++;
        i++;
    }

    if (0 != len % 2) {
        array[i++] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    }

    return len / 2 + len % 2;
}

/**
 * @brief : 整型数组转换为十六进制字符串
 *
 * @param array
 * @param array_size
 * @return vd_uint32_t
 */
vd_uint32_t common_arrary2str(vd_char_t* str, vd_uint8_t* array, vd_uint32_t array_size) {
    vd_uint32_t i, len = 0;
    vd_check_return_val(str == vd_null || array == vd_null, 0);

    for (i = 0; i < array_size; i++) {
        len += sprintf(&str[len], "%02x", array[i]);
    }
    return len;
}

/**
 * @brief : 计算子串出现的次数
 *
 * @param str
 * @param sub
 * @return vd_uint32_t
 */
vd_uint32_t common_str_contain_count(const vd_char_t* str, const vd_char_t* sub) {
    const vd_char_t* start;
    vd_uint32_t      count = 0;
    vd_check_return_val(str == vd_null || sub == vd_null, 0);

    start = str;
    while ((start = (vd_char_t*)strstr(start, sub)) != vd_null) {
        start += strlen(sub);
        count++;
    }
    return count;
}

#if (USE_PLATFORM == MCU)
#else
/**
 * @brief :字符串分裂函数，返回子串数组，和数组size，注意调用common_str_split_free释放子串内存,和子串数组内存
 *
 * @param str
 * @param delim
 * @param str_array : 指针数组
 * @return vd_uint32_t
 */
vd_uint32_t common_str_split(vd_char_t* str, vd_char_t* delim, vd_char_t*** str_array) {
    vd_uint32_t i = 0;
    vd_char_t *str_new, *token;
    vd_uint32_t count;
    vd_check_return_val(str == vd_null || delim == vd_null, 0);
    vd_check_return_val(strlen(str) == 0, 0);

    count = common_str_contain_count(str, delim);

    *str_array = (vd_char_t**)malloc(sizeof(vd_char_t*) * (count + 1));
    vd_check_return_val(str_array == vd_null, 0);

    str_new = strdup(str);

    for (token = strtok(str_new, delim); token != vd_null; token = strtok(vd_null, delim)) {
        (*str_array)[i] = (vd_char_t*)malloc(strlen(token) + 1);
        vd_check_goto((*str_array)[i] == vd_null, ERROR);
        memset((*str_array)[i], 0, strlen(token) + 1);
        strcpy((*str_array)[i], token);
        i++;
    }

ERROR:
    free(str_new);
    if (i == 0) {
        free(*str_array);
    }

    return i;
}
#endif

/**
 * @brief : 释放common_str_split分配的内存
 *
 * @param str_array
 * @param array_size
 * @return vd_void_t
 */
vd_void_t common_str_split_free(vd_char_t** str_array, vd_uint32_t array_size) {
    vd_uint32_t i;
    for (i = 0; i < array_size; i++) {
        if (str_array[i] != vd_null) {
            free(str_array[i]);
        }
    }
    if (str_array != vd_null) {
        free(str_array);
    }
}

/**
 * @brief : 字符串比较，加入为空判断
 *
 * @param str
 * @param sub
 * @return vd_int32_t
 */
vd_int32_t common_strcmp(const vd_char_t* str, const vd_char_t* sub) {
    vd_check_return_val(str == vd_null, -1);
    vd_check_return_val(sub == vd_null, 1);
    return strcmp(str, sub);
}
