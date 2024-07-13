#ifndef __CS_LIST_H
#define __CS_LIST_H
#include "cs_collection_common.h"

typedef struct cs_list_s{
    vd_void_t* item;
    struct cs_list_s* prev;
    struct cs_list_s* next;
    struct cs_list_s* end;
    vd_uint32_t size;
} cs_list_t;

/**
 * @brief 初始化
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_new(cs_list_t** list);

/**
 * @brief 增加至末尾
 *
 * @param list
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_add(cs_list_t* list, vd_void_t* item);

/**
 * @brief 增加至指定位置
 *
 * @param list
 * @param posi
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_add_posi(cs_list_t* list, vd_uint32_t posi, vd_void_t* item);

/**
 * @brief 获取长度
 *
 * @param list
 * @param size
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_size(cs_list_t* list, vd_uint32_t* size);

/**
 * @brief 读取
 *
 * @param list
 * @param posi
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_get(cs_list_t* list, vd_uint32_t posi, vd_void_t** item);

/**
 * @brief 移除
 *
 * @param list
 * @param posi
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_remove(cs_list_t* list, vd_uint32_t posi);

/**
 * @brief 遍历
 *
 * @param list
 * @param fn
 * @return enum CS_STATE
 */
enum CS_STATE  cs_list_foreach(cs_list_t* list, void (*fn)(vd_uint32_t, void*));

/**
 * @brief 测试用，别管
 *
 * @param list
 * @param fn
 * @param pr
 * @return enum CS_STATE
 */
enum CS_STATE  cs_list_foreach_test(cs_list_t* list, void (*fn)(vd_uint32_t, void*), void (*pr)(void*));

/**
 * @brief 销毁
 *
 * @param list
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_destory(cs_list_t* list);

/**
 * @brief 销毁(回调)
 *
 * @param list
 * @param cb
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_destory_cb(cs_list_t* list, void (*cb)(void*));

#endif // !__CS_LIST_H