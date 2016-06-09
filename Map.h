//
// Created by hadoop on 6/8/16.
//

#ifndef MYSTL_MAP_H
#define MYSTL_MAP_H


#include "Alloc.h"
#include "RB_Tree.h"

#include <utility>

#include <functional>

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

namespace MySTL{

#ifndef __STL_LIMITED_DEFAULT_TEMPLATES
    template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
#else
    template <class Key, class T, class Compare, class Alloc = alloc>
#endif

    /*
    template <class Key,class T,
            class Compare = less<Key>,  // 默认递增排序
            class Alloc = alloc>*/
    class map{
    public:
        typedef Key                         key_type;
        typedef T                           data_type;
        typedef pair<const Key, T>          value_type;  // 元素型别(键值对)
        typedef Compare                     key_compare;  // 键值比较函数


    private:
        typedef rb_tree<key_type, value_type,
                _Select1st<value_type>, key_compare, Alloc> rep_type;
        rep_type t;  // 以红黑树作为底层实现

    public:
        typedef typename rep_type::pointer                  pointer;
        typedef typename rep_type::const_pointer            const_pointer;
        typedef typename rep_type::reference                reference;
        typedef typename rep_type::const_reference          const_reference;
        typedef typename rep_type::iterator                 iterator;
        typedef typename rep_type::const_iterator           const_iterator;
        typedef typename rep_type::size_type                size_type;
        typedef typename rep_type::difference_type          difference_type;

        // allocate / deallocate
        map() : t(Compare()) {}
        explicit map(const Compare& comp) : t(comp) {}

        map(const map<Key,T,Compare,Alloc>& x) : t(x.t) {}
        map<Key,T,Compare,Alloc>&  operator=(const map<Key,T,Compare,Alloc>& x){
            t = x.t;
            return *this;
        };

        // accesors,所有操作都是对 RB-Tree 的调用
        iterator begin() { return t.begin(); }
        iterator end() { return t.end(); }
        const_iterator begin() const { return t.begin(); }
        const_iterator end() const { return t.end(); }

        size_type size() { return t.size(); }
        //  下表操作符
        T& operator[](const key_type& k){
            return (*((insert(value_type(k,T()))).first)).second;
        }


        // 插入/删除数据
        pair<iterator,bool>insert(const value_type& x){
            return t.insert_unique(x);
        };

        void erase(iterator position) { t.erase(position); }
        size_type erase(const key_type& x) { return t.erase(x); }

        // find
        iterator find(const key_type& x){ return t.find(x); }
        iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
        const_iterator lower_bound(const key_type& x) const{
            return t.lower_bound(x);
        }
        iterator upper_bound(const key_type& x){
            return t.upper_bound(x);
        }
        const_iterator upper_bound(const key_type& x) const{
            return t.upper_bound(x);
        }
        pair<iterator,iterator> equal_rang(const key_type& x){
            return t.equal_range(x);
        };
        pair<const_iterator,const_iterator> equal_rang(const key_type& x) const{
            return t.equal_range(x);
        };



    };
}

#endif //MYSTL_MAP_H
