//
// Created by hadoop on 7/7/16.
//

#ifndef MYSTL_PRIORITY_QUEUE_H
#define MYSTL_PRIORITY_QUEUE_H


#include <vector>
#include "Heap.h"


using namespace std;

namespace MySTL{
    template <class T,class Sequence = vector<T>,
            class Compare = less<typename Sequence::value_type>>
    class priority_queue{
    public:
        typedef typename Sequence::value_type           value_type;
        typedef typename Sequence::size_type            size_type;
        typedef typename Sequence::reference            reference;
        typedef typename Sequence::const_reference      const_reference;

    protected:
        Sequence c;         // 底层容器
        Compare  comp;      // 元素大小比较函数

    public:   // constructor
        priority_queue():c(){ }
        explicit priority_queue(const Compare& x):c(),comp(x){}

        template <class InputIterator>
        priority_queue(InputIterator first,InputIterator last,const Compare& x)
                :c(first,last),comp(x){ make_heap(c.begin(),c.end()); }

        template <class InputIterator>
        priority_queue(InputIterator first,InputIterator last)
                :c(first,last){ make_heap(c.begin(),c.end()); }


        bool empty() const{ return c.empty();   }
        size_type size() const{ return c.size();    }
        const_reference top() const{ return c.front(); }

        void push(const value_type& x){
            c.push_back(x);
            push_heap(c.begin(),c.end());
        }

        void pop(){
            pop_heap(c.begin(),c.end());
            c.pop_back();
        }

    };
}


#endif //MYSTL_PRIORITY_QUEUE_H
