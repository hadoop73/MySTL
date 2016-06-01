//
// Created by JQK on 2016/5/31.
//

#ifndef MYSTL_RB_TREE_H
#define MYSTL_RB_TREE_H

#include "Iterator.h"
#include "Alloc.h"
#include "Allocator.h"

namespace MySTL{
    typedef bool rb_tree_color_type;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    struct rb_tree_node_base{
        typedef rb_tree_color_type  color_type;
        typedef rb_tree_node_base*  base_ptr;

        color_type  color;
        base_ptr    parent;
        base_ptr    left;
        base_ptr    right;
    };

    template <class Value>
    struct rb_tree_node : public rb_tree_node_base{
        typedef rb_tree_node<Value>*    link_type;
        Value    value_field;
    };

    // 基层迭代器
    struct rb_tree_base_iterator{
        typedef bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                       difference_type;
    };

    // RB-tree 迭代器
    template <class Value,class Ref,class Ptr>
    struct rb_tree_iterator : public rb_tree_base_iterator{
        typedef     Value       value_type;
        typedef     Ref         reference;
        typedef     Ptr         pointer;

        typedef rb_tree_iterator<Value, Value&, Value*>     iterator;
        typedef rb_tree_iterator<const Value, const Value&, const Value*>     const_iterator;
        typedef rb_tree_iterator<Value, Ref, Ptr>           self;
    };

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
    struct rb_tree{

    protected:
        typedef rb_tree_node<Value>     node;
        typedef allocator<node,Alloc>   rb_tree_node_allocator;

    protected:
        typedef Key             key_type;
        typedef Value           value_type;
        typedef value_type*     pointer;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        typedef value_type&     reference;

        typedef rb_tree_node*   link_type;

    protected:
        link_type get_node(){ return rb_tree_node_allocator::allocate();    }
        void    put_node(link_type p){ rb_tree_node_allocator::deallocate(p);  }

        link_type create_node(const value_type& x){
            link_type p = get_node();
            construct(&p->value_field,x);
            return p;
        }

        void destroy_node(link_type x){
            destroy(&x->value_field);
            put_node(x);
        }

    protected:
        size_type node_count;    //  节点数量
        link_type header;
        Compare   key_compare;      // 节点的键值比较函数

    protected:
        link_type& root() const { return (link_type&) header->parent;  }
        link_type& leftmost() const { return (link_type&) header->left; }
        link_type& rightmost() const { return (link_type&) header->right; }

        static link_type& left(link_type x){
            return (link_type&) (x->left);
        }
        static link_type& right(link_type x){
            return (link_type&) (x->right);
        }
        static link_type& parent(link_type x){
            return (link_type&) (x->parent);
        }
        static reference value(link_type x){
            return x->value_field;
        }

    };

}



#endif //MYSTL_RB_TREE_H
