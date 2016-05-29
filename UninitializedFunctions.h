//
// Created by JQK on 2016/5/27.
//

#ifndef MYSTL_UNINITIALIZEDFUNCTIONS_H
#define MYSTL_UNINITIALIZEDFUNCTIONS_H

#include "TypeTraits.h"
#include "Iterator.h"
#include "Construct.h"

#include <cstring>

namespace MySTL{
    /***************************************************************************************/


    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,_true_type){
        return (ForwardIterator)memmove(result,first,(last - first)* sizeof(*first));  // memmove 可以用于同区间
    }

    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator result,_false_type){
        int i = 0;
        for (;first != last;first++,i++){
            construct((result+i),*first);
        }
        return (result+i);
    }

    template <class InputIterator,class ForwardIterator,class T>
    inline ForwardIterator
    _uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result,T*){
        typedef typename _type_traits<T>::is_POD_type is_POD;
        return _uninitialized_copy_aux(first,last,result,is_POD());
    }

    // 区间拷贝
    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result){
        return _uninitialized_copy(first,last,result,value_type(result));
    }
    /***************************************************************************************/

    template <class ForwardIterator,class T>
    inline void _uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,_true_type){
        return std::fill(first,last,x);
    }

    template <class ForwardIterator,class T>
    inline void _uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& x,_false_type){
        for (;first != last;first++){
            construct(first,x);
        }
    }

    template <class ForwardIterator,class T,class T1>
    inline void _uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x,T1 *){
        typedef typename _type_traits<T1>::is_POD_type is_POD;
        return _uninitialized_fill_aux(first,last,x,is_POD());
    }

    // 区间填充
    template <class ForwardIterator,class T>
    inline void uninitialized_fill(ForwardIterator first,ForwardIterator last, const T& x){
        _uninitialized_fill(first,last,x,value_type(first));

    }
    /***************************************************************************************/

    template <class ForwardIterator,class Size,class T>
    inline ForwardIterator
    _uninitialized_fill_n_aux(ForwardIterator first,Size n, const T& x,_true_type){
        return std::fill_n(first,n,x);
    }

    template <class ForwardIterator,class Size,class T>
    inline ForwardIterator
    _uninitialized_fill_n_aux(ForwardIterator first,Size n, const T& x,_false_type){
        for (int i=0;i<n;i++){
            construct(first+i,x);
        }
        return (first+n);
    }

    template <class ForwardIterator,class Size,class T,class T1>
    inline ForwardIterator
    _uninitialized_file_n(ForwardIterator first,Size n,const T& x,T1 *){
        typedef typename _type_traits<T1>::is_POD_type is_POD;
        return _uninitialized_fill_n_aux(first,n,x,is_POD());
    }

    // 区间 n 填充
    template <class ForwardIterator,class Size,class T>
    inline ForwardIterator
    uninitialized_fill_n(ForwardIterator first,Size n, const T& x){
        _uninitialized_file_n(first,n,x,value_type(first));

    }

    /***************************************************************************************/




}

#endif //MYSTL_UNINITIALIZEDFUNCTIONS_H
