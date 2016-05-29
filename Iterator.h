//
// Created by JQK on 2016/5/27.
//

#ifndef MYSTL_ITERATOR_H
#define MYSTL_ITERATOR_H

#include <crtdefs.h>

namespace MySTL{
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag:public input_iterator_tag{};
    struct bidirectional_iterator_tag:public forward_iterator_tag{};
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};

    template <class Category,class T,class Distance = ptrdiff_t ,
            class Pointer = T*,class Reference = T&>
    struct iterator{
        typedef Category        iterator_category;
        typedef T               value_type;
        typedef Distance        difference_type;
        typedef Pointer         pointer;
        typedef Reference       reference;
    };

    // ��ȡ traits
    template <class Iterator>
    struct iterator_traits{
        typedef typename Iterator::iterator_category        iterator_category;
        typedef typename Iterator::value_type               value_type;
        typedef typename Iterator::difference_type          difference_type;
        typedef typename Iterator::pointer                  pointer;
        typedef typename Iterator::reference                reference;
    };

    // ԭ��ָ�� iterator_traits ƫ�ػ�
    template <class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
    };

    // point-const-ָ�� iterator_traits ƫ�ػ�
    template <class T>
    struct iterator_traits<const T*>{
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
    };

    // ��ȡ����������
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    };

    // ��ȡ��������������
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
    };

    // ��ȡ������ value_type ����
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator){
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }

    /****************************************************************************/

    // ���� distance ����
    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first,InputIterator last,input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last){
            first++;  n++;
        }
        return n;
    }

    template <class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    _distance(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag){
        return last - first;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first,InputIterator last){
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return _distance(first,last,category());
    }
    /****************************************************************************/
    // ���� advance ����
    template <class InputIterator,class Distance>
    inline void _advance(InputIterator& i,Distance n,input_iterator_tag){
        while (n--)  ++i;
    }

    template <class BidirectionalIterator,class Distance>
    inline void _advance(BidirectionalIterator& i,Distance n,bidirectional_iterator_tag){
        if (n>0){
            while (n--) ++i;
        } else{
            while (n++) --i;
        }
    }

    template <class RandomAccessIterator,class Distance>
    inline void _advance(RandomAccessIterator& i,Distance n,random_access_iterator_tag){
        i += n;
    }

    template <class InputIterator,class Distance>
    inline void advance(InputIterator& i,Distance n){
        _advance(i,n,iterator_category(i));
    }
    /****************************************************************************/
}


#endif //MYSTL_ITERATOR_H
