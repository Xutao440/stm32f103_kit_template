/*
 * Project: linux-kit
 * Moudle: common
 * File: common_file.c
 * Created Date: 2021-01-28 10:34:34
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
#include "common_file.h"

#include "base.h"
#include "common_base.h"
#include "common_strings.h"
#include "log.h"

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
/**
 * @brief ͨ��path��ȡ�ļ���n�����ȵ�����
 *
 * @param path �ļ�·��
 * @param buf ����������,��������
 * @param max_len
 * һ�ζ�ȡ�ļ����ݵ���󳤶ȣ�(Ĭ��FILE_NREAD_DEFAULT_MAX_LEN)����ļ����ݳ��ȴ���max_len���򲻶�ȡ�ļ�����
 * @return vd_int_t ʵ��ȥ�����ݵĳ���;����0��ʾ�����ĳ���,-1��ʾ�ļ���С����max_len
 */
vd_int_t common_file_nread(const vd_char_t* path, vd_char_t** buf, vd_int_t max_len) {
    vd_ulong_t len = 0;
    if (max_len == 0) {
        max_len = COMMON_FILE_NREAD_DEFAULT_MAX_LEN;
    }

    FILE* file;
    if (!common_file_exists(path)) {
        log_error("file not exist, path: %s", path);
        return -1;
    }

    vd_int64_t size = common_file_size(path);

    if (size > max_len) {
        log_error("file size: %d greater than max_len: %d", size, max_len);
        return -1;
    }

    *buf = (vd_char_t*)calloc(1, size + 1);
    if (buf == vd_null) {
        log_error("Failed to malloc, size : %d", size);
        return -1;
    }
    (*buf)[size] = '\0';

    file = fopen(path, "r");
    len  = fread(*buf, 1, size, file);
    if (len == 0) {
        log_error("failed to read file:%s", path);
        free(*buf);
        fclose(file);
        return -1;
    }
    fclose(file);
    return (vd_int_t)len;
}

/**
 * @brief ��������Ŀ¼
 *
 * @param path Ŀ¼
 * @param mode ģʽ
 * @return vd_int_t 0��ʾ�ɹ�,-1��ʾʧ��
 */
static vd_int_t common_do_mkdir(const vd_char_t* path, mode_t mode) {
    struct stat st;
    vd_int_t    status = 0;

    if (stat(path, &st) != 0) {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(path, mode) != 0 && errno != EEXIST) status = -1;
    } else if (!S_ISDIR(st.st_mode)) {
        errno  = ENOTDIR;
        status = -1;
    }

    return (status);
}

/**
 * @brief ����Ŀ¼(����������Ŀ¼)
 *
 * @param path Ҫ������·��
 * @param mode ģʽ
 * @return vd_int_t 0��ʾ�ɹ�,-1��ʾʧ��
 */
vd_int_t common_mkpath_all(const vd_char_t* path, mode_t mode) {
    vd_char_t* pp;
    vd_char_t* sp;
    vd_int_t   status;
    vd_char_t* copypath = strdup(path);

    status = 0;
    pp     = copypath;
    while (status == 0 && (sp = strchr(pp, '/')) != 0) {
        if (sp != pp) {
            /* Neither root nor double slash in path */
            *sp    = '\0';
            status = common_do_mkdir(copypath, mode);
            *sp    = '/';
        }
        pp = sp + 1;
    }
    if (status == 0) status = common_do_mkdir(path, mode);
    free(copypath);
    return (status);
}

/**
 * @brief ���Ŀ¼�Ƿ����
 *
 * @param dir_path Ŀ¼·��
 * @return vd_true ��ʾĿ¼����
 * @return vd_false ��ʾĿ¼������
 */
vd_bool_t common_dir_exists(const vd_char_t* dir_path) {
    return access(dir_path, F_OK) == 0;
}

/**
 * @brief ����ļ��Ƿ����
 *
 * @param path �ļ�·��
 * @return vd_true ��ʾ�ļ�����
 * @return vd_false ��ʾ�ļ�������
 */
vd_bool_t common_file_exists(const vd_char_t* path) {
    return access(path, F_OK) == 0;
}

/**
 * @brief ��ȡ�ļ��Ĵ�С
 *
 * @param path �ļ�·��
 * @return uvd_int64_t
 */
vd_int64_t common_file_size(const vd_char_t* path) {
    struct stat buffer;
    vd_int_t    status;
    status = stat(path, &buffer);
    if (status == 0) {
        // size of file is in member buffer.st_size;
        return buffer.st_size;
    }
    return -1;
}

vd_int_t common_mkdir_p(const vd_char_t* dir) {
    if (access(dir, 0) == 0) {
        return EEXIST;
    }
    vd_char_t tmp[VD_PATH_MAX_LEN];
    common_safe_strncpy(tmp, dir, sizeof(tmp));
    vd_char_t* p     = tmp;
    vd_char_t  delim = '/';
    while (*p) {
#ifdef OS_WIN
        if (*p == '/' || *p == '\\') {
            delim = *p;
#else
        if (*p == '/') {
#endif
            *p = '\0';
            mkdir(tmp, 0770);
            *p = delim;
        }
        ++p;
    }
    if (mkdir(tmp, 0770) != 0) {
        return EPERM;
    }
    return 0;
}

vd_int_t common_rmdir_p(const vd_char_t* dir) {
    if (access(dir, 0) != 0) {
        return ENOENT;
    }
    if (rmdir(dir) != 0) {
        return EPERM;
    }
    vd_char_t tmp[VD_PATH_MAX_LEN];
    common_safe_strncpy(tmp, dir, sizeof(tmp));
    vd_char_t* p = tmp;
    while (*p) ++p;
    while (--p >= tmp) {
#ifdef OS_WIN
        if (*p == '/' || *p == '\\') {
#else
        if (*p == '/') {
#endif
            *p = '\0';
            if (rmdir(tmp) != 0) {
                return 0;
            }
        }
    }
    return 0;
}