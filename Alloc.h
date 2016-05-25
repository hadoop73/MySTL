//
// Created by JQK on 2016/5/25.
//

#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

#include <cstdlib>

namespace MySTL{
    // �ռ�����
    class alloc{
    private:
        enum EAlign{ ALIGN = 8}; // С��������ϵ��߽�
        enum EMaxBytes{ MAXBYTES = 128};  // С�����������
        enum ENFreeLists{ NFREELISTS = (EMaxBytes::MAXBYTES/EAlign::ALIGN)};  // free-lists �ĸ���

    private:
        // free-lists �ڵ�
        union obj{
            union obj *next;
            char client[1];
        };

        static obj *free_list[ENFreeLists::NFREELISTS];
    private:
        static char *start_free;   // �ڴ����ʼλ��
        static char *end_free;     // �ڴ�ؽ���λ��
        static size_t heap_size;
    private:
        // ��bytes�ϵ���8�ı���
        static size_t ROUND_DP(size_t bytes){
            return (bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1);
        }
        // ���������С�����ص�n��free-list��n��0��ʼ��
        static size_t FREELIST_INDEX(size_t bytes){
            return (bytes+EAlign::ALIGN-1)/(EAlign::ALIGN-1);
        }
        // ����һ����СΪn�Ķ���
        static void *refill(size_t n);
        // ����һ���ռ䣬������nobjs����СΪsize������
        // �������nobjs�������������㣬nobjs���ܻ��С
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
