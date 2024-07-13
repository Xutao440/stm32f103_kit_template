/*
 * Project: linux-kit
 * Moudle: common
 * File: common_file.h
 * Created Date: 2021-01-28 10:59:48
 * Author: ShiBin
 * Description: description
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */
#ifndef COMMON_FILE_H
#define COMMON_FILE_H

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

/* ======================================================================================
 * declaration
 */

/**
 * @brief ͨ��path��ȡ�ļ���n�����ȵ�����
 *
 * @param path �ļ�·��
 * @param buf ����������,��������
 * @param max_len
 * һ�ζ�ȡ�ļ����ݵ���󳤶ȣ�(Ĭ��FILE_NREAD_DEFAULT_MAX_LEN)����ļ����ݳ��ȴ���max_len���򲻶�ȡ�ļ�����
 *
 * Note: free buf memory by your self
 * @return vd_int_t ʵ��ȥ�����ݵĳ���;����0��ʾ�����ĳ���,-1��ʾ�ļ���С����max_len
 */
vd_int_t common_file_nread(const vd_char_t* path, vd_char_t** buf, vd_int_t max_len);

/**
 * @brief ����Ŀ¼(����������Ŀ¼)
 *
 * @param path Ҫ������·��
 * @param mode ģʽ
 * @return vd_int_t 0��ʾ�ɹ�,-1��ʾʧ��
 */
vd_int_t common_mkpath_all(const vd_char_t* path, mode_t mode);

// todo: rm -rf implement

/**
 * @brief ���Ŀ¼�Ƿ����
 *
 * @param dir_path Ŀ¼·��
 * @return true ��ʾĿ¼����
 * @return false ��ʾĿ¼������
 */
vd_bool_t common_dir_exists(const vd_char_t* dir_path);

/**
 * @brief ����ļ��Ƿ����
 *
 * @param path �ļ�·��
 * @return true ��ʾ�ļ�����
 * @return false ��ʾ�ļ�������
 */
vd_bool_t common_file_exists(const vd_char_t* path);

/**
 * @brief : ɾ��ָ��·�����ļ�
 *
 * @param path ���ļ�·��
 * @return vd_bool_t
 */
vd_bool_t common_file_delete(const vd_char_t* path);

/**
 * @brief ��ȡ�ļ��Ĵ�С
 *
 * @param path �ļ�·��
 * @return uint64_t
 */
vd_int64_t common_file_size(const vd_char_t* path);

// todo:Note
// mkdir -p
vd_int_t common_mkdir_p(const vd_char_t* dir);

// rmdir -p
vd_int_t common_rmdir_p(const vd_char_t* dir);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__
#endif  // COMMON_FILE_H