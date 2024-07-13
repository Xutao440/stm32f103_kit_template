
#include "base_type.h"
#include "base_utils.h"
#include "cs_collection_common.h"
#include "cs_list.h"

//enum CS_STATE cs_list_new(cs_list_t** list) {
//    cs_list_t* l;
//    l = os_malloc(sizeof(cs_list_t));
//    if (vd_null == l) {
//        return CS_ERROR;
//    }

//    l->item = vd_null;
//    l->prev = vd_null;
//    l->next = vd_null;
//    l->size = 0;
//    *list = l;
//    return CS_OK;
//}

//enum CS_STATE cs_list_add(cs_list_t* list, vd_void_t* item) {
//    vd_uint32_t size;
//    vd_int16_t e;

//    if (vd_null == list || vd_null == item) {
//        return CS_ERROR;
//    }
//    e = cs_list_size(list, &size);
//    if (CS_OK != e) {
//        return e;
//    }
//    return cs_list_add_posi(list, size, item);
//}

//enum CS_STATE cs_list_add_posi(cs_list_t* list, vd_uint32_t posi, vd_void_t* item) {
//    cs_list_t* prev_l;
//    cs_list_t* l;
//    cs_list_t* new_l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }
//    if (vd_null == item) {
//        return CS_ERROR;
//    }
//    if (posi > list->size) {
//        return CS_ERROR;
//    }

//    prev_l = vd_null;
//    l = list;
//    for (i = 0; i < posi; i++) {
//        prev_l = l;
//        l = l->next;
//    }

//    if (0 == list->size) {
//        list->item = item;
//        list->size++;
//        return CS_OK;
//    }

//    new_l = os_malloc(sizeof(cs_list_t));
//    if (vd_null == new_l) {
//        return CS_ERROR;
//    }

//    if (0 == posi) {
//        memcpy(new_l, list, sizeof(cs_list_t));
//        list->item = item;
//        list->next = new_l;
//        new_l->next->prev = new_l;
//        new_l->prev = list;
//        list->size++;
//        return CS_OK;
//    }

//    prev_l->next = new_l;
//    new_l->next = l;
//    new_l->item = item;
//    new_l->prev = prev_l;
//    if (vd_null != l) {
//        l->prev = new_l;
//    }
//    if (list->size == posi) {
//        list->end = new_l;
//    }
//    list->size++;
//    return CS_OK;
//}

//enum CS_STATE cs_list_size(cs_list_t* list, vd_uint32_t* size) {
//    if (vd_null == list) {
//        return CS_ERROR;
//    }
//    *size = list->size;
//    return CS_OK;
//}

//enum CS_STATE cs_list_get(cs_list_t* list, vd_uint32_t posi, vd_void_t** item) {
//    cs_list_t* l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }

//    if (0 == list->size || posi >= list->size) {
//        return CS_OUT_QUEUE;
//    }

//    l = list;
//    for (i = 0; i < posi; i++) {
//        l = l->next;
//    }
//    *item = l->item;
//    return CS_OK;
//}

//enum CS_STATE cs_list_remove(cs_list_t* list, vd_uint32_t posi) {
//    cs_list_t* l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }
//    if (posi >= list->size) {
//        return CS_OUT_QUEUE;
//    }

//    if (posi == 0) {
//        if (0 < list->size) {
//            l = list->next;
//            l->size = list->size;
//            vd_memcpy(list, l, sizeof(cs_list_t));
//            list->size--;
//            return CS_OK;
//        } else {
//            list->item = vd_null;
//            list->size--;
//            return CS_OK;
//        }
//    }

//    l = list;
//    for (i = 0; i < posi; i++) {
//        l = l->next;
//    }
//    l->prev->next = l->next;
//    if (vd_null != l->next)
//        l->next->prev = l->prev;
//    os_free(l);
//    list->size--;
//    return CS_OK;
//}

//enum CS_STATE  cs_list_foreach(cs_list_t* list, void (*fn)(vd_uint32_t, void*)) {
//    cs_list_t* l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }
//    if (0 == list->size) {
//        return CS_OK;
//    }

//    l = list;
//    fn (0, l->item);
//    for (i = 1; i < list->size; i++) {
//        l = l->next;
//        fn (i, l->item);
//    }
//    return CS_OK;
//}

//enum CS_STATE  cs_list_foreach_test(cs_list_t* list, void (*fn)(vd_uint32_t, void*), void (*pr)(void*)) {
//    cs_list_t* l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }
//    if (0 == list->size) {
//        return CS_OK;
//    }

//    l = list;
//    fn (0, l->item);
//    for (i = 1; i < list->size; i++) {
//        l = l->next;
//        fn (i, l->item);
//        pr(l->prev->item);
//    }
//    return CS_OK;
//}

//enum CS_STATE cs_list_inner_foreach(cs_list_t* list, void (*fn)(void*)) {
//    cs_list_t* l;
//    vd_uint32_t i;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }

//    l = list;
//    fn(l->item);
//    for (i = 1; i < list->size; i++) {
//        l = l->next;
//        fn (l->item);
//    }
//    return CS_OK;
//}

//enum CS_STATE cs_list_destory(cs_list_t* list) {
//    cs_list_t* bef_l;
//    cs_list_t* l;

//    if (vd_null == list) {
//        return CS_ERROR;
//    }

//    bef_l = list;
//    l = list->next;
//    while (vd_null != l) {
//        bef_l = l;
//        l = l->next;
//        os_free(bef_l);
//    }
//    os_free(list);
//    return CS_OK;
//}

//enum CS_STATE cs_list_destory_cb(cs_list_t* list, void (*cb)(void*)) {
//    vd_int16_t e;

//    e = cs_list_inner_foreach(list, cb);
//    if (CS_OK == e) {
//        return cs_list_destory(list);
//    } else {
//        return e;
//    }
//}