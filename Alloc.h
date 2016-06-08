//
// Created by JQK on 2016/5/25.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#include <cstdlib>
#include "malloc_alloc.h"

namespace MySTL{
    // ???????
    class alloc{
    private:
        enum EAlign{ ALIGN = 8}; // §³?????????????
        enum EMaxBytes{ MAXBYTES = 128};  // §³???????????
        enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES/EAlign::ALIGN)};  // free-lists ?????
        enum ENObjs{ NOBJS = 20}; // ??¦Ç?????????

    private:
        // free-lists ???
        union obj{
            union obj *next;
            char client[1];
        };

        static obj *free_list[ENFreeLists::NFREELISTS];
    private:
        static char *start_free;   // ???????¦Ë??
        static char *end_free;     // ???????¦Ë??
        static size_t heap_size;
    private:
        // ??bytes?????8?????
        static size_t ROUND_DP(size_t bytes){
            return (bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1);
        }
        // ?????????§³???????n??free-list??n??0?????
        static size_t FREELIST_INDEX(size_t bytes){
            return (bytes+EAlign::ALIGN-1)/EAlign::ALIGN-1;
        }
        // ?????????§³?n?????
        static void *refill(size_t n);
        // ????????????????nobjs????§³?size??????
        // ???????nobjs??????????????nobjs??????§³
        static char *chunk_alloc(size_t size,size_t &nobjs);

    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr,size_t bytes);
        static void *reallocate(void *ptr,size_t old_sz,size_t new_sz);
    };

    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;
    alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    void alloc::deallocate(void *ptr, size_t bytes) {
        if (bytes > EMaxBytes::MAXBYTES){
            free(ptr);
            return;
        }
        size_t index = FREELIST_INDEX(bytes);
        obj *node = (obj *)(ptr);
        node->next = free_list[index];
        free_list[index] = node;
    }

    void *alloc::reallocate(void *ptr,size_t old_sz,size_t new_sz){
        deallocate(ptr,old_sz);
        ptr = allocate(new_sz);
        return ptr;
    }

    void *alloc::allocate(size_t bytes) {
        if (bytes <= 0){
            return 0;
        }
        if (bytes>EMaxBytes::MAXBYTES){ // ????128byte????????????
            return malloc_alloc::allocate(bytes);
        }
        // ???????????????????¦Ä?????
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if (list){  // free_list ?§Ý????????
            free_list[index] = list->next;
            return list;
        } else{ // free_list ?????????????????¡¤?????
            return refill((ROUND_DP(bytes)));
        }
    }

    void *alloc::refill(size_t bytes) {
        size_t nobjs = ENObjs::NOBJS;  // ??????¦Ç??? 20??
        char *chunk = chunk_alloc(bytes,nobjs);
        obj **my_free_list = 0;
        obj *result = 0;
        obj *current_obj = 0,*next_obj = 0;

        if (nobjs == 1){  // ??????????????
            return chunk;
        } else{
            my_free_list = free_list + FREELIST_INDEX(bytes);
            result = (obj *)(chunk);
            *my_free_list = next_obj = (obj *)(chunk + bytes);
            for (int i = 1; ; ++i) {
                current_obj = next_obj;
                next_obj = (obj *)((char *)next_obj + bytes);
                if (i == nobjs-1){
                    current_obj->next = 0;
                    break;
                } else{
                    current_obj->next = next_obj;
                }
            }
            return result;
        }
    }

    char *alloc::chunk_alloc(size_t size,size_t &nobjs){
        char *result = 0;
        size_t total_size = size*nobjs;
        size_t bytes_left = end_free-start_free;
        if (bytes_left >= total_size){
            result = start_free;
            start_free += total_size;
            return result;
        } else if (bytes_left >= size){
            nobjs = bytes_left/size;
            total_size = size*nobjs;
            result = start_free;
            start_free += total_size;
            return result;
        } else{ // ?????????úY??????????????????
            if (bytes_left > 0){
                obj ** my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj *)start_free)->next = *my_free_list;
                *my_free_list = (obj *)start_free;
            }
            size_t bytes_to_get = 2 * total_size + ROUND_DP(heap_size>>4);
            start_free = (char *)malloc(bytes_to_get);
            if (!start_free){
                obj ** my_free_list = 0;
                for (int i = size; i < EMaxBytes::MAXBYTES ; i += EAlign::ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    if (*my_free_list){
                        start_free = (char *)(*my_free_list);
                        end_free = start_free + i;
                        *my_free_list = (*my_free_list)->next;
                        return chunk_alloc(size,nobjs);
                    }
                }
            }
            end_free = start_free + bytes_to_get;
            heap_size += bytes_to_get;
            return chunk_alloc(size,nobjs);
        }
    }
}


#endif //MYSTL_ALLOC_H
