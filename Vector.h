//
// Created by JQK on 2016/5/27.
//

#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"

namespace MySTL{
    template <class T,class Alloc = allocator<T>>
    class vector{
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef T*              iterator;
        typedef T&              reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       diference_type;

    private:
        iterator    start;           // 目前空间的头
        iterator    finish;          // 目前空间的尾
        iterator    end_of_storage;  // 可用空间的尾

        typedef Alloc    dataAllocator;
        void insert_aux(iterator position,const T& x);

    protected:
        iterator allocate_and_fille(size_type n, const T& value){
            iterator result = dataAllocator::allocate(n);
            uninitialized_fill_n(result,n,value);
            return result;
        }
        void fill_initialize(size_type n,const T& value){
            start = allocate_and_fille(n,value);
            finish = start + n;
            end_of_storage = finish;
        }

        void deallocate(){
            if (start){
                dataAllocator::deallocate(start,end_of_storage - start);
            }
        }

    public:
        iterator begin()    { return start;     }
        iterator end()      { return finish;    }
        size_type size() const { return size_type(finish - start);    }
        size_type capacity() const {
            return size_type(end_of_storage - start);
        }
        bool empty() const { return begin() == end(); }
        reference operator[](size_type n) { return *(begin() + n); }

        vector() :  start(0),finish(0),end_of_storage(0) {}
        vector(size_type n, const T& value) { fill_initialize(n,value); }
        vector(int n, const T& value) { fill_initialize(n,value); }
        vector(long n, const T& value) { fill_initialize(n,value); }
        explicit vector(size_type n) { fill_initialize(n,T()); }

        ~vector(){
            destroy(start,finish);
            deallocate();
        }

        reference front() { return *begin(); }
        reference back()  { return *(end() - 1); }
        // 将元素插入尾端
        void push_back(const T& x){
            if (finish != end_of_storage){
                construct(finish,x);
                finish++;
            } else{
                insert_aux(end(),x);
            }
        }
        void pop_back(){
            finish--;
            destroy(finish);
        }
        iterator erase(iterator position){  // 清楚某位置上的元素
            if (position + 1 != end()){
                memcpy(position,position+1,end()-position);
            }
            finish--;
            destroy(finish);
            return position;
        }
        void resize(size_type new_size, const T& x){
            if (new_size < size()){
                erase(begin() + new_size);
            }
        }

    };

    template <class Iterator>
    Iterator copy_backward(Iterator first,Iterator last,Iterator result){
        while (last != first) *(--result) = *(--last);
        return result;
    }

    template <class T,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator position, const T &x) {
        if (finish != end_of_storage){
            copy_backward(position,finish,finish+1);
            construct(position,x);
            finish++;
        } else{  // 无备用空间的分配
            const size_type len = size();
            const size_type new_size = len>0?2*len:1;
            iterator new_start;
            try {
                new_start = dataAllocator::allocate(new_size);
                uninitialized_copy(start,position,new_start);
                auto temp = position - start;
                construct(new_start + temp,x);
                uninitialized_copy(position,finish,new_start + temp + 1);
            }catch(...) {
                destroy(new_start,new_start + new_size);
                dataAllocator::deallocate(new_start,new_size) ;
                throw ;
            }

            destroy(begin(),end());
            deallocate();

            start = new_start;
            end_of_storage = start + new_size;
            finish = start + len + 1;

        }
    }

}

#endif //MYSTL_VECTOR_H
