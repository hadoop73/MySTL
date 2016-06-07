//
// Created by JQK on 2016/5/31.
//

#ifndef MYSTL_RB_TREE_H
#define MYSTL_RB_TREE_H

#include "Iterator.h"
#include "Alloc.h"
#include "Allocator.h"

#include <utility>

using namespace std;

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

        static base_ptr minimum(base_ptr x){
            while (x->left != 0) x = x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x){
            while (x->right != 0) x = x->right;
            return x;
        }
    };

    template <class Value>
    struct rb_tree_node : public rb_tree_node_base{
        typedef rb_tree_node<Value>*    link_type;
        Value    value_field;
    };

    // 基层迭代器
    struct rb_tree_base_iterator{
        typedef rb_tree_node_base::base_ptr     base_ptr;
        typedef bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                       difference_type;

        base_ptr node;    // 用来与容器之间产生一个连接关系

        // 底层的 operator++，符合中序遍历
        void increment(){
            if(node->right != 0){
                node = node->right;
                while (node->left != 0){
                    node = node->left;
                }
            } else{
                base_ptr y = node->parent;
                while (node == y->right){
                    node = y;
                    y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }
        // 底层的 operator--
        void decrement(){
            if (node->color == rb_tree_red &&
                    node->parent->parent == node)
                node = node->right;
            else if (node->left != 0){
                base_ptr y = node->left;
                while (y->right != 0)
                    y = y->right;
                node = y;
            } else{
                base_ptr y = node->parent;
                while (node == y->left){
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }
    };

    // RB-tree 迭代器
    template <class Value,class Ref,class Ptr>
    struct rb_tree_iterator : public rb_tree_base_iterator{
        typedef     Value       value_type;
        typedef     Ref         reference;
        typedef     Ptr         pointer;

        typedef rb_tree_iterator<Value, Value&, Value*>                         iterator;
        typedef rb_tree_iterator<const Value, const Value&, const Value*>       const_iterator;
        typedef rb_tree_iterator<Value, Ref, Ptr>           self;
        typedef rb_tree_node<Value>*    linke_type;

        rb_tree_iterator() {}
        rb_tree_iterator(linke_type x) { node = x; }
        rb_tree_iterator(const iterator& it) {  node = it.node; }

        reference operator*() const { return linke_type(node)->value_field; }
        pointer operator->() const { return &(operator*()); }

        self&  operator++(){
            increment();
            return *this;
        }
        self   operator++(int){
            auto tmp = *this;
            increment();
            return tmp;
        }

        self&  operator--(){
            decrement();
            return *this;
        }
        self  operator--(int){
            auto tmp = *this;
            decrement();
            return tmp;
        }
    };

    inline bool operator==(const rb_tree_base_iterator& x,
                            const rb_tree_base_iterator& y){
        return x.node == y.node;
    }
    inline bool operator!=(const rb_tree_base_iterator& x,
                           const rb_tree_base_iterator& y){
        return x.node != y.node;
    }

    inline void _rb_tree_rotate_left(rb_tree_node_base* x,rb_tree_node_base*& root){
        // x 为旋转点
        // root 为引用变量，引用的是 rb_tree_node_base* 类型变量
        rb_tree_node_base* y = x->right;
        x->right = y->left;
        if (y->left)  y->left->parent = x;
        y->left = x;
        if (x == root)
            root = y;
        else if (x = x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->parent = x->parent;
        x->parent = y;
    }

    inline void _rb_tree_rotate_right(rb_tree_node_base* x,rb_tree_node_base*& root){
        rb_tree_node_base* y = x->left;
        x->left = y->right;
        if (y->right)   y->right->parent = x;
        y->right = x;
        y->parent = x->parent;
        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        x->parent = y;
    }

    inline rb_tree_node_base*
    rb_tree_rebalance_for_erase(rb_tree_node_base* z,
                                rb_tree_node_base*& root,
                                rb_tree_node_base*& leftmost,
                                rb_tree_node_base*& rightmost){
        rb_tree_node_base* y = z;
        rb_tree_node_base* x = 0;
        rb_tree_node_base* x_parent = 0;
        if (y->left == 0)
            x = y->right;
        else if (y->right == 0)
            x = y->left;
        else {
            y = y->right;
            while (y->left != 0){
                y = y->left;
            }
            x = y->right;  // y 的下一个节点
        }
        // y != z 则调整 y 到 z 的位置
        if (y != z){
            z->left->parent = y;
            y->left = z->left;
            if (y != z->right){
                x_parent = y->parent;
                if (x) x->parent = y->parent;
                y->parent->left = x;
                y->right = z->right;
                z->parent->right = y;
            } else
                x_parent = y;
            if (z == root)
                root = y;
            else if (z == z->parent->left)
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
            rb_tree_color_type t = y->color;
            y->color = z->color;
            z->color = t;
            y = z;
        } else{  // y == z
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            if (z == root)
                root = x;
            else if (z == z->parent->left)
                z->parent->left = x;
            else
                z->parent->right = x;
            if (leftmost == z)
            if (z->right == 0)
                leftmost = z->parent;
            else
                leftmost = rb_tree_node_base::minimum(x);
            if (rightmost == z)
            if (z->left == 0)
                rightmost = z->parent;
            else
                rightmost = rb_tree_node_base::maximum(x);
        }
        if (y->color != rb_tree_red){
            while (x != root && (x == 0 || x->color == rb_tree_black)){
                if (x == x_parent->left){
                    rb_tree_node_base* w = x_parent->right;
                    if (w->color == rb_tree_red){
                        w->color = rb_tree_black;
                        x_parent->color = rb_tree_red;
                        _rb_tree_rotate_left(x_parent,root);
                        w = x_parent->right;
                    }
                    if ((w->left == 0 || w->left->color == rb_tree_black)&&
                        (w->right == 0 || w->right->color == rb_tree_black)){
                        w->color = rb_tree_red;  // 相当于把 w 也删除,这样 x_parent 两边黑色是平衡的,只要继续处理 x_parent
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else{
                        if (w->right == 0 || w->right->color == rb_tree_black){
                            if (w->left) w->left->color = rb_tree_black;
                            w->color = rb_tree_red;
                            _rb_tree_rotate_right(w,root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color; // 相当于把 w 子节点中的红色变成黑色补充到被删除的一边
                        x_parent->color = rb_tree_black;
                        if (w->right) w->right->color = rb_tree_black;
                        _rb_tree_rotate_left(x_parent,root);
                        break;
                    }
                }else{ // x == x_parent->right
                    rb_tree_node_base* w = x_parent->left;
                    if (w->color == rb_tree_red){
                        w->color = rb_tree_black;
                        x_parent->color = rb_tree_red;
                        _rb_tree_rotate_right(x_parent,root);
                        w = x_parent->left;
                    }
                    if ((w->right == 0 || w->right->color == rb_tree_black) &&
                        (w->left == 0 || w->left->color == rb_tree_black)){
                        x->color = rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else{
                        if (w->left == 0 || w->left->color == rb_tree_black){
                            if (w->right)  w->right->color = rb_tree_black;
                            w->color = rb_tree_red;
                            _rb_tree_rotate_left(w,root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = rb_tree_black;
                        if (w->left) w->left->color = rb_tree_black;
                        _rb_tree_rotate_right(x_parent,root);
                        break;
                    }
                }
                if (x) x->color = rb_tree_black;
            }
            return y;
        }
    }

    inline void rb_tree_rebalance(rb_tree_node_base* x,rb_tree_node_base*& root){
        x->color = rb_tree_red;
        while (x != root && x->parent->color == rb_tree_red){
            if (x->parent == x->parent->parent->left){
                rb_tree_node_base* y = x->parent->parent->right;
                if (y && y->color == rb_tree_red){
                    y->color = rb_tree_black;
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                } else{
                    if (x == x->parent->right){
                        x = x->parent;
                        _rb_tree_rotate_left(x,root);
                    }
                    x->parent->parent->color = rb_tree_red;
                    x->parent->color = rb_tree_black;
                    _rb_tree_rotate_right(x->parent->parent,root);
                }
            } else{
                rb_tree_node_base* y = x->parent->parent->left;
                if (y&&y->color == rb_tree_red){  // bug y==0
                    y->color = rb_tree_black;
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                } else{
                    if (x == x->parent->left){
                        x = x->parent;
                        _rb_tree_rotate_right(x,root);
                    }
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    _rb_tree_rotate_left(x->parent->parent,root);
                }
            }
        }
        root->color = rb_tree_black;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
    class rb_tree{

    protected:
        typedef rb_tree_node<Value>     node;
        typedef rb_tree_node_base*      base_ptr;
        typedef rb_tree_color_type      color_type;
        typedef allocator<node,Alloc>   rb_tree_node_allocator;

    protected:
        typedef Key             key_type;
        typedef Value           value_type;
        typedef value_type*     pointer;
        typedef const pointer   const_pointer;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        typedef value_type&     reference;
        typedef const reference const_reference;

        typedef rb_tree_node<Value>*   link_type;

    public:
        typedef rb_tree_iterator<value_type ,reference ,pointer >                       iterator;
        typedef rb_tree_iterator<value_type ,const_reference,const_pointer >            const_iterator;

    public:
        // allocation/deallocation
        rb_tree(const Compare& comp = Compare())
                : node_count(0), key_compare(comp) { init(); }
    private:
        iterator _insert(base_ptr x,base_ptr y, const value_type& v);
        void init(){
            header = get_node();
            color(header) = rb_tree_red;
            cout << "init"<<endl;
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        }

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
        static const Key& key(base_ptr x) {
            return KeyOfValue()(value(link_type(x)));
        }
        static color_type& color(base_ptr x){
            return (color_type&) (link_type(x))->color;
        }

    public:
        pair<iterator,bool> insert_unique(const value_type& v);
        void erase(iterator position);
        size_type erase(const key_type& x);
        void erase(iterator first, iterator last);
        void clear(){
            if (node_count != 0){

            }
        }

    public:              // set operation
        iterator find(const key_type& x);
        const_iterator find(const key_type& x) const;
        size_type count(const key_type& x) const;
        iterator lower_bound(const key_type& x);
        const_iterator lower_bound(const key_type& x) const;
        iterator upper_bound(const key_type& x);
        const_iterator upper_bound(const key_type& x) const;
        pair<iterator,iterator> equal_range(const key_type& x);
        pair<const_iterator,const_iterator> equal_range(const key_type& x) const;

    public:
        iterator begin(){ return leftmost();   }
        const_iterator begin() const{ return const_iterator(leftmost()); }
        iterator end(){ return header;  }
        const_iterator end() const { return const_iterator(header); }
        size_type size() const { return node_count; }

    };

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) {
        link_type y = header;
        link_type x = root();

        while (x != 0){
            if (!key_compare(key(x),k))
                y = x, x = left(x);
            else
                x = right(x);
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k,key(j.node)))? end():j;
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) const{
        link_type y = header;
        link_type x = root();

        while (x != 0){
            if (!key_compare(key(x),k))
                y = x, x = left(x);
            else
                x = right(x);
        }
        const_iterator j = const_iterator(y);
        return (j == end() || key_compare(k,key(j.node)))? end():j;
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type &k) const {
        pair<const_iterator,const_iterator> p = equal_range(k);
        size_type n = 0;
        const_iterator ss = p.first;
        const_iterator se = p.second;
        for (; ss != se ; ss++) {
            if (!key_compare(k,key(ss.node)))
                n++;
        }
        return n;
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k) {
        link_type y = header;    //  比 k 大或相等的最小节点
        link_type x = root();

        while (x != 0)
            if (!key_compare(key(x),k))
                y = x, x = left(x);
            else
                x = right(x);
        return iterator(y);
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k) const{
        link_type y = header;    //  比 k 大或相等的最小节点
        link_type x = root();

        while (x != 0)
            if (!key_compare(key(x),k))
                y = x, x = left(x);
            else
                x = right(x);
        return const_iterator(y);
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k) {
        link_type y = header;    // 比 k 大的最小节点
        link_type x = root();

        while (x != 0)
            if (key_compare(k,key(x)))
                y = x, x = left(x);
            else
                x = right(x);
        return iterator(y);
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k) const{
        link_type y = header;    // 比 k 大的最小节点
        link_type x = root();

        while (x != 0)
            if (key_compare(k,key(x)))
                y = x, x = left(x);
            else
                x = right(x);
        return const_iterator(y);
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
         typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k) {
        return pair<iterator,iterator>(lower_bound(k),upper_bound(k));
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k)const {
        const_iterator first = lower_bound(k);
        const_iterator end = upper_bound(k);
        return pair<const_iterator,const_iterator>(first,end);
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    void
    rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::erase(iterator position){
        link_type y = (link_type) rb_tree_rebalance_for_erase(position.node,
                                                                header->parent,
                                                                header->left,
                                                                header->right);
        destroy_node(y);
        node_count--;
    }

    template <class Key,class Value,class KeyOfValue,class Compare,class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
        return 1;
    }


    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,bool >
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        insert_unique(const Value& v){
        link_type y = header;
        link_type x = root();
        bool cmp = true;
        while (x != 0){
            y = x;
            cmp = key_compare(KeyOfValue()(v),key(x));  //  v 小于 x,则插入左边,否则放在右边
            x = cmp?left(x):right(x);
        } // 找到 v 的插入点
        iterator j = iterator(y);
        if (cmp)
            if (j == begin()){
                return pair<iterator, bool>(_insert(x, y, v), true);
            }else // 用于判断 可能存在的重复节点，因为
                --j;
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator,bool>(_insert(x,y,v), true);

        return pair<iterator,bool>(j, false);
    };

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        _insert(base_ptr x_, base_ptr y_, const Value &v) {
        link_type x = (link_type) x_;
        link_type y = (link_type) y_;
        link_type z;

        if (y == header || x != 0 || key_compare(KeyOfValue()(v),key(y))){
            z = create_node(v);
            left(y) = z;
            if (y == header){
                root() = z;
                rightmost() = z;
            } else if(y == leftmost())
                leftmost() = z;
        } else{
            z = create_node(v);
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }
        parent(z) = y;
        left(z) = 0;
        right(z) = 0;

        rb_tree_rebalance(z,header->parent);
        ++node_count;
        return iterator(z);
    }


}



#endif //MYSTL_RB_TREE_H
