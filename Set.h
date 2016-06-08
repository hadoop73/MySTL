//
// Created by hadoop on 6/8/16.
//

#ifndef MYSTL_SET_H
#define MYSTL_SET_H

#include "Alloc.h"
#include "Allocator.h"
#include "RB_Tree.h"
#include <utility>

namespace MySTL {
    template<class Key, class Compare = less<Key>, class Alloc = alloc>
    class set {

    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;

    private:
        typedef rb_tree<key_type, value_type, _Identity< value_type>,key_compare ,Alloc>
        rep_type;

        rep_type t;  // rb-tree

    public:
        typedef typename rep_type::const_pointer pointer;
        typedef typename rep_type::const_pointer const_pointer;
        typedef typename rep_type::const_reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::const_iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::difference_type difference_type;
        typedef typename rep_type::size_type size_type;

        // allocatin/deallocation

        set() : t(Compare()) { }

        explicit set(const Compare &comp) : t(comp) { }


        // set 操作，rb-tree 都有提供
        //key_compare key_comp() const { return t.key_comp(); }
        iterator begin() const { return t.begin(); }

        iterator end() const { return t.end(); }

        size_type size() { return t.size(); }


        // insert and erase
        pair<iterator, bool> insert(const value_type &x) {
            pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
            return pair<iterator, bool>(p.first, p.second);
        };

        size_type erase(const key_type &x) {
            return t.erase(x);
        }

    };
}

#endif //MYSTL_SET_H
