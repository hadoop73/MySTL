//
// Created by hadoop on 7/4/16.
//

#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include "Alloc.h"
#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"

namespace MySTL{


    inline size_t _deque_buffer_size(size_t n,size_t sz){
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template <class T,class Ref,class Ptr,size_t BufSize>
    struct _deque_iterator{
        typedef _deque_iterator<T,T&,T*,BufSize>     iterator;
        typedef _deque_iterator<T,const T&,const T*,BufSize> const_iterator;
        static size_t buffer_size(){ return _deque_buffer_size(BufSize,sizeof(T));   }


        typedef random_access_iterator_tag  iterator_category;
        typedef T               value_type;
        typedef Ptr             pointer;
        typedef Ref             reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        typedef T**             map_pointer;
        typedef _deque_iterator self;

        T* cur;     // 当前元素指针
        T* first;  // 缓冲区的头部
        T* last;   // 指向缓冲区的最后，实际上不能用来存储元素，而是拿来作为边界
        map_pointer node;

        // constructor
        _deque_iterator(T* x,map_pointer y)
            :cur(x),first(y),last(*y+buffer_size()),node(y){}
        _deque_iterator():cur(0),first(0),last(0),node(0){}
        _deque_iterator(const iterator& x):cur(x.cur),first(x.first),last(x.last),node(x.node){}


        difference_type operator-(const self& x) const{
            return difference_type(buffer_size()*(node-x.node-1))+
                    (cur-first) + (x.last-x.cur);
        }

        void set_node(map_pointer new_node){
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size()) ;
        }

        reference operator*() const { return *cur;  }
        pointer operator->()  const { return &(operator*()); }


        self& operator++() {
            ++cur;
            if (cur == last){
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }

        self operator++(int){ // 后置式，标准写法
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--(){
            if (cur == first){
                set_node(node-1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int){
            self tmp = *this;
            --*this;
            return tmp;
        }

        self& operator+=(difference_type n){
            difference_type offset = n + (cur - first);
            if(offset >= 0 && offset < difference_type(buffer_size())){
                cur += n;  // 目标位置在同一个缓冲区内
            } else{
                // 目标位置不在同一个缓冲区，offset 可以为正或负数
                difference_type node_offset =
                        offset > 0 ? offset/difference_type(buffer_size()):
                        -difference_type((-offset - 1)/buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset*difference_type(buffer_size()));
            }
            return *this;
        }
        self operator+(difference_type n) const{
            self tmp = *this;
            return tmp += n;
        }
        self operator-=(difference_type n){ return *this += -n; }
        self operator-(difference_type n) const{
            self tmp = *this;
            return tmp -= n;
        }
        reference operator[] (difference_type n) const{ return *(*this + n); }

        bool operator==(const self& x) const { return cur == x.cur; }
        bool operator!=(const self& x) const { return !(*this == x);  }
        bool operator<(const self& x) const{
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };



    template <class T,class Alloc = alloc,size_t BufSize = 0>
    class deque{
    public:
        typedef T                                               value_type;
        typedef value_type&                                     reference;
        typedef ptrdiff_t                                       difference_type;
        typedef value_type*                                     pointer;
        typedef size_t                                          size_type;
        typedef _deque_iterator<T, T&, T*, BufSize>             iterator;
        typedef _deque_iterator<T,const T&,const T*,BufSize>    const_iterator;

    protected:
        // 元素指针
        typedef pointer *map_pointer;
        typedef allocator<T ,Alloc>  data_allocator;
        typedef allocator<pointer ,Alloc>   map_allocator;

        static size_type buffer_size(){
            return _deque_buffer_size(BufSize,sizeof(value_type));
        }
        static size_type initial_map_size(){ return 8; }
    protected:
        iterator start;   // start.cur 指向最前面的元素
        iterator finish;  // finish.cur 指向最后元素的下一个节点

        map_pointer map; // 指向map，map 是一个连续空间，每个元素都是一个指针，指向一块缓冲区
        size_type map_size;


    public:
        // 构造函数和析构函数
        deque():start(),finish(),map_size(0),map(0){
            create_map_and_nodes(0);
        }

        deque(const deque& x):start(),finish(),map(0),map_size(0){
            create_map_and_nodes(x.size());
            uninitialized_copy(x.begin(),x.end(),start);
        }
        deque(size_type n, const value_type& value)
           :start(),finish(),map(0),map_size(0){
            fill_initialize(n,value);
           // deque_aux(n,val,typename std::is_integral::)
        }
        deque(int n, const value_type& value)
                :start(),finish(),map(0),map_size(0){
            fill_initialize(n,value);
        }
        deque(size_type n)
                :start(),finish(),map(0),map_size(0){
            fill_initialize(n,value_type());
        }
        ~deque(){
            destroy(start,finish);
            destroy_map_and_nodes();
        }

    protected:
        void fill_initialize(size_type n, const value_type& value);
        void create_map_and_nodes(size_type num_elements);
        void destroy_map_and_nodes();


    public:
        iterator  begin() { return start; }
        iterator  end()   { return finish;  }
        const_iterator begin() const { return start;  }
        const_iterator  end()  const { return finish;  }
        reference operator[](size_type n){
            return start[difference_type(n)];
        }
        reference front() { return *start;  }
        reference back(){
            iterator tmp = finish;
            --tmp;
            return *tmp;
        }

        size_type size() const { return finish - start; }
        bool empty() const { return finish==start;  }

    public:   // push and pop
        void push_back(const value_type& t){
            if (finish.cur!=finish.last-1){
                construct(finish.cur,t);
                ++finish.cur;
            } else
                push_back_aux(t);
        }

        void push_front(const value_type& t){
            if (start.cur!=start.first){
                construct(start.cur,t);
                --finish.cur;
            } else
                push_front_aux(t);
        }

        void pop_back(){
            if (finish.cur!=finish.first){
                --finish.cur;
                destroy(finish.cur);
            } else
                pop_back_aux();
        }

        void pop_front(){
            if (start.cur!=start.last-1){
                destroy(start.cur);
                ++start.cur;
            } else
                pop_front_aux();
        }

    public:   // insert

        iterator insert(iterator position, const value_type& x){
            if (position.cur == start.cur){
                push_front(x);
                return start;
            } else if (position.cur == finish.cur){
                push_back(x);
                iterator tmp = finish;
                --tmp;
                return tmp;
            } else
                return insert_aux(position,x);
        }

    protected:
        void push_back_aux(const value_type& t);
        void push_front_aux(const value_type& t);
        void pop_back_aux();
        void pop_front_aux();

        iterator insert_aux(iterator pos,const value_type& x);


        void reserve_map_at_back(size_type nodes_to_add = 1){
            if (nodes_to_add + 1>map_size-(finish.node-map)){
                reallocate_map(nodes_to_add, false);
            }
        }

        void reserve_map_at_front(size_type nodes_to_add = 1){
            if (nodes_to_add > start.node - map)
                reallocate_map(nodes_to_add, true);
        }

        void reallocate_map(size_type nodes_to_add,bool add_at_front);

    public:

        // allocate deallocate
        pointer allocate_node(){ return data_allocator::allocate(buffer_size());    }
        void deallocate_node(pointer n){
            data_allocator::deallocate(n,buffer_size());
        }
    };

    template <class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::fill_initialize(size_type n, const value_type &value) {
        create_map_and_nodes(n);
        map_pointer cur;
        for (cur = start.node;cur<finish.node;++cur)
            uninitialized_fill(*cur,*cur+buffer_size(),value);
        uninitialized_fill(finish.first,finish.cur,value);
    }

    template <class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::create_map_and_nodes(size_type num_elements) {
        size_type num_nodes = num_elements / buffer_size() + 1;
        map_size = std::max(initial_map_size(),num_nodes + 2);
        map = map_allocator::allocate(map_size);

        map_pointer nstart = map + (map_size - num_nodes)/2;
        map_pointer nfinish = nstart + num_nodes - 1; // 指向最后一个节点，有数据

        map_pointer cur;
        for (cur = nstart;cur <= nfinish; ++cur)
            *cur = allocate_node();

        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;   // 指向第一个元素
        finish.cur = finish.first + num_elements%buffer_size(); //指向最后一个元素
    }

    template <class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::destroy_map_and_nodes() {
        for (map_pointer cur = start.node;cur<=finish.node;++cur){
            deallocate_node(*cur);
        }
        map_allocator::deallocate(map,map_size);
    }

    // push
    // 当 finish.cur == finish.last - 1 时
    template <class T,class Alloc,size_t BuffSize>
    void deque<T,Alloc,BuffSize>::push_back_aux(const value_type &t) {
        value_type t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = allocate_node();

        construct(finish.cur,t_copy);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }

    template <class T,class Alloc,size_t BuffSize>
    void deque<T,Alloc,BuffSize>::push_front_aux(const value_type &t) {
        value_type t_copy = t;
        reserve_map_at_front();
        *(start.node - 1) = allocate_node();

        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        construct(start.cur,t_copy);
    }

    template <class T,class Alloc,size_t BuffSize>
    void deque<T,Alloc,BuffSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;
        if (map_size > 2*new_num_nodes){
            // 空间还有剩余不需要分配空间
            new_nstart = map + (map_size-new_num_nodes)/2
                    + (add_at_front?nodes_to_add:0);
            if (new_nstart<start.node)
                std::copy(start.node,finish.node+1,new_nstart);
            else
                // 第二个参数需要 +1，因为copy判断的是小于号，后面+old_num_nodes表示原来的长度
                std::copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
        } else{
            size_type new_map_size = map_size + std::max(map_size,nodes_to_add) + 1;

            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size-new_num_nodes)/2
                    + (add_at_front?nodes_to_add:0);
            std::copy(start.node,finish.node+1,new_nstart);
            map_allocator::deallocate(map,map_size);

            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart+old_num_nodes-1);
    }



}

#endif //MYSTL_DEQUE_H
