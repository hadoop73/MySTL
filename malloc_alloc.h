//
// Created by JQK on 2016/5/26.
//

#ifndef MYSTL_MALLOC_ALLOC_H
#define MYSTL_MALLOC_ALLOC_H

#if 0
#   include <new>
#   define THROW_BAD_ALLOC throw bad_alloc
#elif   !defined(THROW_BAD_ALLOC)
#   include<iostream>
#   define THROW_BAD_ALLOC  do{ \
        exit(1);                        \
    }while(0);
#endif

#include <cstdlib>

namespace MySTL{
    template <int ints>
    class _malloc_alloc{
    private:
        // 处理内存不足情况
        static void *oom_malloc(size_t);
        static void *oom_realloc(void *,size_t);
        static void (* malloc_alloc_oom_handler)();

    public:
        static void *allocate(size_t n){
            void *result = malloc(n);
            if (0 == result) result = oom_malloc(n);
            return result;
        }

        static void deallocate(void *p,size_t){
            free(p);
        }

        static void *reallocate(void *p,size_t,size_t new_sz){
            void *result = realloc(p,new_sz);
            if (0 == result) oom_realloc(p,new_sz);
            return result;
        }

        // 仿真C++的set_new_handler()，指定自己的 out-of-memory handler
        static void (* set_malloc_handler(void (*f)()))(){
            void (* old)() = malloc_alloc_oom_handler;
            malloc_alloc_oom_handler = f;
            return (old);
        }
    };

    template <int ints>
    void (* _malloc_alloc<ints>::malloc_alloc_oom_handler)() = 0;

    template <int ints>
    void *_malloc_alloc<ints>::oom_malloc(size_t n) {
        void (*my_malloc_handler)();
        void *result;
        for (;;){
            my_malloc_handler = malloc_alloc_oom_handler;
            if (0 == my_malloc_handler) { THROW_BAD_ALLOC ;}
            (*my_malloc_handler)();
            result = malloc(n);
            if (result) return result;
        }
    }

    template <int ints>
    void *_malloc_alloc<ints>::oom_realloc(void *p, size_t n) {
        void (*my_malloc_handler)();
        void *result;
        for (;;){
            my_malloc_handler = malloc_alloc_oom_handler;
            if (0 == my_malloc_handler) { THROW_BAD_ALLOC ;}
            (*my_malloc_handler)();
            result = realloc(p,n);
            if (result) return result;
        }
    }

    typedef _malloc_alloc<0> malloc_alloc;
}















#endif //MYSTL_MALLOC_ALLOC_H
