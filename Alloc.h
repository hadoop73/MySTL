//
// Created by JQK on 2016/5/25.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#include <cstdlib>
#include "malloc_alloc.h"

namespace MySTL{
    // 空间配置
    class alloc{
    private:
        enum EAlign{ ALIGN = 8}; // 小型区块的上调边界
        enum EMaxBytes{ MAXBYTES = 128};  // 小型区块的上限
        enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES/EAlign::ALIGN)};  // free-lists 的个数
        enum ENObjs{ NOBJS = 20}; // 每次分配的空间个数

    private:
        // free-lists 节点
        union obj{
            union obj *next;
            char client[1];
        };

        static obj *free_list[ENFreeLists::NFREELISTS];
    private:
        static char *start_free;   // 内存池起始位置
        static char *end_free;     // 内存池结束位置
        static size_t heap_size;
    private:
        // 将bytes上调至8的倍数
        static size_t ROUND_DP(size_t bytes){
            return (bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1);
        }
        // 根据区块大小，返回第n号free-list，n从0开始算
        static size_t FREELIST_INDEX(size_t bytes){
            return (bytes+EAlign::ALIGN-1)/EAlign::ALIGN-1;
        }
        // 返回一个大小为n的对象
        static void *refill(size_t n);
        // 配置一大块空间，可容纳nobjs个大小为size的区块
        // 如果配置nobjs个区块有所不便，nobjs可能会减小
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
        obj *node = static_cast<obj *>(ptr);
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
        if (bytes>EMaxBytes::MAXBYTES){ // 大于128byte，一次性分配空间
            return malloc_alloc::allocate(bytes);
        }
        // 在二级配置空间的链表中取未分配块
        size_t index = FREELIST_INDEX(bytes);
        obj *list = free_list[index];
        if (list){  // free_list 中还用空间分配
            free_list[index] = list->next;
            return list;
        } else{ // free_list 空间不足时，从内存池重新分配空间
            return refill((ROUND_DP(bytes)));
        }
    }

    void *alloc::refill(size_t bytes) {
        size_t nobjs = ENObjs::NOBJS;  // 尝试一次分配 20个
        char *chunk = chunk_alloc(bytes,nobjs);
        obj **my_free_list = 0;
        obj *result = 0;
        obj *current_obj = 0,*next_obj = 0;

        if (nobjs == 1){  // 最后只能分配一个块
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
        } else{ // 最后连一个块都无法申请，先处理零碎的块
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
