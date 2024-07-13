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
 * @brief 通过path读取文件的n个长度的内容
 *
 * @param path 文件路径
 * @param buf 读到的内容,传出参数
 * @param max_len
 * 一次读取文件内容的最大长度；(默认FILE_NREAD_DEFAULT_MAX_LEN)如果文件内容长度大于max_len，则不读取文件内容
 *
 * Note: free buf memory by your self
 * @return vd_int_t 实际去读内容的长度;大于0表示读到的长度,-1表示文件大小大于max_len
 */
vd_int_t common_file_nread(const vd_char_t* path, vd_char_t** buf, vd_int_t max_len);

/**
 * @brief 创建目录(包含创建子目录)
 *
 * @param path 要创建的路径
 * @param mode 模式
 * @return vd_int_t 0表示成功,-1表示失败
 */
vd_int_t common_mkpath_all(const vd_char_t* path, mode_t mode);

// todo: rm -rf implement

/**
 * @brief 检查目录是否存在
 *
 * @param dir_path 目录路径
 * @return true 表示目录存在
 * @return false 表示目录不存在
 */
vd_bool_t common_dir_exists(const vd_char_t* dir_path);

/**
 * @brief 检查文件是否存在
 *
 * @param path 文件路径
 * @return true 表示文件存在
 * @return false 表示文件不存在
 */
vd_bool_t common_file_exists(const vd_char_t* path);

/**
 * @brief : 删除指定路径的文件
 *
 * @param path ：文件路径
 * @return vd_bool_t
 */
vd_bool_t common_file_delete(const vd_char_t* path);

/**
 * @brief 获取文件的大小
 *
 * @param path 文件路径
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