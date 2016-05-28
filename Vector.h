//
// Created by JQK on 2016/5/27.
//

#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"

namespace MySTL{
    template <class T,class Alloc = allocator>
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

        typedef Alloc<T>    dataAllocator;
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
        size_type size() const { return size_type(end() - begin());    }
        size_type capacity() const {
            return size_type(end_of_storage - begin());
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

    template <class T,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator position, const T &x) {
        if (finish != end_of_storage){
            construct(finish,*(finish - 1));
            finish++;
            uninitialized_copy()
        }
    }

}

#endif //MYSTL_VECTOR_H
