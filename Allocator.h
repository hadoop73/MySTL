//
// Created by JQK on 2016/5/25.
//

#ifndef MYSTL_ALLOCATOR_H
#define MYSTL_ALLOCATOR_H

#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>
#include <cstddef> // ptrdiff_t

namespace MySTL{
    template <class T,class Alloc>
    class allocator{
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

    public:
        static T* allocate() {
            return static_cast<T *>(Alloc::allocate(sizeof(T)));
        }

        static T *allocate(size_t n) {
            assert(n != 0);
            return static_cast<T *>(Alloc::allocate( sizeof(T)*n ));
        }

        static void deallocate(T *ptr) {
            Alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
        }
        static void deallocate(T *ptr,size_t n) {
            assert(n != 0);
            Alloc::deallocate(static_cast<void *>(ptr), sizeof(T)*n);
        }

        static void construct(T *ptr) {
            new(ptr)T();
        }
        static void construct(T *ptr, const T& value) {
            new(ptr)T(value);
        }
        static void destroy(T *ptr) {
            ptr->~T();
        }
    };
    
}




#endif //MYSTL_ALLOCATOR_H
