//
// Created by JQK on 2016/5/24.
//

#ifndef MYSTL_CONSTRUCT_H
#define MYSTL_CONSTRUCT_H

#include <new>
#include <d2d1helper.h>

#include "TypeTraits.h"

namespace MySTL{
    // 初始化模板
    // 使用 placement new 方法，分配空间并初始化 value
    template <class T1,class T2>
    inline void construct(T1 *ptr1, const T2& value){
        new(ptr1) T1(value);  // placement new
    }

    // 销毁模板
    template <class T>
    inline void destroy(T *ptr){
        ptr->~T();
    }

    //
    template <class ForwardIterator>
    inline void _destroy(ForwardIterator first,ForwardIterator last,_true_type){}

    // 区间内逐个销毁
    template <class ForwardIterator>
    inline void _destroy(ForwardIterator first,ForwardIterator last,_false_type){
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    // 区间销毁模板
    template <class ForwardIterator>
    inline void destroy(ForwardIterator first,ForwardIterator last){
        typedef typename _type_traits<T>::is_POD_type is_POD_type;
        _destroy(first,last,is_POD_type);
    }

}

#endif //MYSTL_CONSTRUCT_H
