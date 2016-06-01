//
// Created by JQK on 2016/5/29.
//

#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

#include "Iterator.h"
#include "Allocator.h"
#include "Alloc.h"

namespace MySTL{
    //  list 的节点
    template <class T>
    struct _list_node{
        typedef void*   void_pointer;
        void_pointer prev;
        void_pointer next;
        T data;
    };

    // list_iterator
    template <class T,class Ref,class Ptr>
    struct _list_iterator{
        typedef _list_iterator<T, T&, T*>   iterator;
        typedef _list_iterator<T, Ref, Ptr> self;

        typedef bidirectional_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef Ptr                             pointer;
        typedef Ref                             reference;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;
        typedef _list_node<T>*                  link_type;

        link_type    node;
        //  构造函数
        _list_iterator(link_type x):node(x) {}
        _list_iterator() {}
        _list_iterator(const iterator& x):node(x.node) {}

        bool operator==(const self& x){ return node == x.node; }
        bool operator!=(const self& x){ return node != x.node; }

        self& operator++(){
            node = (link_type)node->next;
            return *this;
        }

        self operator++(int){
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--(){
            node = (link_type)node->prev;
            return *this;
        }

        self operator--(int){
            auto tmp = *this;
            --*this;
            return tmp;
        }

        reference operator*() const {
            return (*node).data;
        }
        pointer operator->() const {
            return &(operator*());
        }
    };

    template <class T,class Alloc = alloc>
    class list{
    protected:
        typedef _list_node<T>       list_node;
        typedef allocator<list_node,Alloc>    list_node_allocate;

    public:
        typedef list_node*      link_type;
        typedef T               value_type;
        typedef const T&        const_reference;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        typedef _list_iterator<T, T&, T*>    iterator;
        typedef _list_iterator<T, const T&, const T*>    const_iterator;
    protected:
        link_type   node;

    protected:
        link_type get_node() { return list_node_allocate::allocate();}
        void put_node(link_type p) { list_node_allocate::deallocate(p);}

        void empty_initialize(){
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        // 产生一个新的节点
        link_type create_node(const T& x){
            link_type p = get_node();
            construct(&p->data,x);
            return p;
        }



    public:
        list(){ empty_initialize(); }

        iterator    begin() { return (link_type)((*node).next); }
        const_iterator  begin() const { return (link_type)((*node).next); }
        iterator    end()   { return node; }
        const_iterator    end()  const  { return node; }

        bool empty() const { return node->next == node; }
        size_type size(){
            size_type result = 0;
            link_type tmp = node;
            while (tmp->next != node){
                result++;
                tmp = (link_type)tmp->next;
            }
            return result;
        }

        void push_back(const T& x){
            insert(end(),x);
        }

        void push_front(const T& x){
            insert(begin(),x);
        }

        iterator insert(iterator position,const T& x){
            link_type p = create_node(x);
            p->next = position.node;
            p->prev = position.node->prev;
            ((link_type)(position.node->prev))->next = p;
            position.node->prev = p;
            return p;
        }

    };
}


#endif //MYSTL_LIST_H
