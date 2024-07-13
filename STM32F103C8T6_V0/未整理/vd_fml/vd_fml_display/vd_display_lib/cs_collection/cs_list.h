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
 * @brief ��ʼ��
 *
 * @param queue
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_new(cs_list_t** list);

/**
 * @brief ������ĩβ
 *
 * @param list
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_add(cs_list_t* list, vd_void_t* item);

/**
 * @brief ������ָ��λ��
 *
 * @param list
 * @param posi
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_add_posi(cs_list_t* list, vd_uint32_t posi, vd_void_t* item);

/**
 * @brief ��ȡ����
 *
 * @param list
 * @param size
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_size(cs_list_t* list, vd_uint32_t* size);

/**
 * @brief ��ȡ
 *
 * @param list
 * @param posi
 * @param item
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_get(cs_list_t* list, vd_uint32_t posi, vd_void_t** item);

/**
 * @brief �Ƴ�
 *
 * @param list
 * @param posi
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_remove(cs_list_t* list, vd_uint32_t posi);

/**
 * @brief ����
 *
 * @param list
 * @param fn
 * @return enum CS_STATE
 */
enum CS_STATE  cs_list_foreach(cs_list_t* list, void (*fn)(vd_uint32_t, void*));

/**
 * @brief �����ã����
 *
 * @param list
 * @param fn
 * @param pr
 * @return enum CS_STATE
 */
enum CS_STATE  cs_list_foreach_test(cs_list_t* list, void (*fn)(vd_uint32_t, void*), void (*pr)(void*));

/**
 * @brief ����
 *
 * @param list
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_destory(cs_list_t* list);

/**
 * @brief ����(�ص�)
 *
 * @param list
 * @param cb
 * @return enum CS_STATE
 */
enum CS_STATE cs_list_destory_cb(cs_list_t* list, void (*cb)(void*));

#endif // !__CS_LIST_H