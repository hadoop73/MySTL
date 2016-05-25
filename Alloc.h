//
// Created by JQK on 2016/5/25.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#include <cstdlib>

namespace MySTL{
    // 空间配置
    class alloc{
    private:
        enum EAlign{ ALIGN = 8}; // 小型区块的上调边界
        enum EMaxBytes{ MAXBYTES = 128};  // 小型区块的上限
        enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES/EAlign::ALIGN)};  // free-lists 的个数

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
            return (bytes+EAlign::ALIGN-1)/(EAlign::ALIGN-1);
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
}


#endif //MYSTL_ALLOC_H
