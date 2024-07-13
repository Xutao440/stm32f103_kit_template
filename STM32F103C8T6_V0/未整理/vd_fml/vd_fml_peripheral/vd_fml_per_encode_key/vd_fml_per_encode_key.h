/**
 * @file vd_fml_per_code_key.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef VD_FML_PER_ENCODE_KEY_H
#define VD_FML_PER_ENCODE_KEY_H


/* ======================================================================================
 * includes
 */
#include "base.h"
#include "platform_config.h"

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
typedef struct {
    vd_uint8_t code;
    vd_uint8_t key_code;  // 'key_code_e'
    vd_void_t *arg;
} encode_key_define_t;

/* ======================================================================================
 * declaration
 */

/**
 * @brief : 编码按键初始化
 *
 * @param void
 * @return
 */

vd_bool_t vd_fml_per_encodekey_init(vd_void_t);


/**
 * @brief : 编码按键处理
 *
 * @param void
 * @return
 */

vd_void_t vd_fml_per_encodekey_loop(vd_void_t);

/* ======================================================================================
 * extern
 */
__vd_extern_c_leave__

#endif // !VD_FML_PER_CODE_KEY_H
