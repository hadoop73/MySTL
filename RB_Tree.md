
##  插入
首先，查找要插入的位置
```cpp
while (x != 0){
     y = x;
     cmp = key_compare(KeyOfValue()(v),key(x));  //  v 小于 x,则插入左边,否则放在右边
     x = cmp?left(x):right(x);
} // 找到 v 的插入点
```
加入新节点，重新连接，把新的节点连接到y
```cpp
parent(z) = y;
left(z) = 0;
right(z) = 0;
```
当y 为最左或者最右时，需要更新为 z

再调整平衡 `rb_tree_rebalance(rb_tree_node_base* x,rb_tree_node_base*& root)`
* 先判断插入点的父亲是在左，还是在右(因为只有此时父亲为red，我们需要判断伯父节点，而伯父节点需要根据父亲节点的在左右判断)
* 如果存在伯父节点，而且为red。此时最为简单，父亲的父亲必定为black，把父亲的父亲变为red，父亲和伯父变为黑色就可以了
* 如果伯父节点不存在或者为黑色；1)新节点在父亲节点的右侧，则先右旋 2)新节点在父亲节点左侧，则左旋 父亲的父亲变成父亲的右子节点
  并变成red，父亲变成black，父亲的右子节点变成了父亲的父亲的左子节点，最后他们到父亲的黑节点数目并没有改变
  左旋或者右旋，他的第一个参数为中间的那个节点

```cpp
inline void rb_tree_rebalance(rb_tree_node_base* x,rb_tree_node_base*& root){
    x->color = rb_tree_red;
    while (x != root && x->parent->color == rb_tree_red){
        if (x->parent == x->parent->parent->left){  //  判断父亲节点是否在左侧
            rb_tree_node_base* y = x->parent->parent->right;
            if (y && y->color == rb_tree_red){  // 判断伯父节点是否为red
                   y->color = rb_tree_black;
                   x->parent->color = rb_tree_black;
                   x->parent->parent->color = rb_tree_red;
                   x = x->parent->parent;
            } else{  // 伯父节点为黑色
                   if (x == x->parent->right){  // 插入的节点在父亲的左侧，需要左旋
                       x = x->parent;
                       _rb_tree_rotate_left(x,root);
                   } // 此时，x 已经改为父亲节点，为了和下面的代码重用
                   x->parent->parent->color = rb_tree_red;
                   x->parent->color = rb_tree_black;
                   _rb_tree_rotate_right(x->parent,root);
            }
        }
    }
    root->color = rb_tree_black;
}
```
```cpp
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
            }else // 用于判断 可能存在的重复节点，因为 10 15 再插入10,如果 j--必定为10
                --j;
        if (key_compare(key(j.node), KeyOfValue()(v))) // 此时 10 < 10 不成立，插入出错
            return pair<iterator,bool>(_insert(x,y,v), true);

        return pair<iterator,bool>(j, false);
    };
```

