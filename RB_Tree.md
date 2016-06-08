
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

##  迭代器递增/递减
```cpp
// 底层的 operator++，符合中序遍历
void increment(){
    if(node->right != 0){  // 迭代器左子树存在，则获取左子树的最小值
           node = node->right;
           while (node->left != 0){
                 node = node->left;
           }
    } else{ // 左子树不存在
           base_ptr y = node->parent;
           while (node == y->right){ // 是否是在右子树，只有在左子树时，才能找到比自己大的节点
                 node = y;
                 y = y->parent;
           }
           if (node->right != y) // 用于判断是否为根节点
                node = y;
    }
}
```
## 删除
首先，查找要删除节点的下一个节点 x，x可以为null
* y->left = 0  x = y->right
* y->right = 0  x = y-> left
* y 左右字节点都在，x 取下一个节点，可以是大于y的下一个节点
  也可以是小于y的下一个节点，此时 y 变成要替换 z 的节点

然后，把y替换到z的位置
```cpp
if (y != z) {                 // relink y in place of z.  y is z's successor
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right) { // 如果 y 原来是 z 的右子节点，则x_parent需重新设置
      x_parent = y->parent;
      if (x) x->parent = y->parent;
      y->parent->left = x;      // y must be a left child
      y->right = z->right;
      z->right->parent = y;
    }
    else
      x_parent = y;
    if (root == z)
      root = y;
    else if (z->parent->left == z)
      z->parent->left = y;
    else
      z->parent->right = y;
    y->parent = z->parent;
    __STD::swap(y->color, z->color);
    y = z;
    // y now points to node to be actually deleted
}
```
平衡调节，因为删除的是y也就是x以前的父亲，调节从x出发
* 先判断x的左右，因为需要根据x的兄弟--原来的伯父，来判断
* 如果伯父节点为red，则x_parent为black，此时y删除，导致
  左侧少了一个black节点，所以把x_parent左旋，伯父节点到了x_parent位置
  并且由red变成black，补充缺失的black，因为伯父的左子树，现在在x_parent右子树，也就
  会比x_parent的左子树多一个black节点，把x_p=red，问题转化到x_p的右节点
  所以 `w = x_parent->right` 继续处理
* 如果伯父为black，左右子树也为black或者为null；这是w=red，相当于x_p左右子树都
  减少一个black，还是平衡的，问题转化为继续调整x_parent， `x = x_parent;`
* 如果伯父为black，左右子树存在red节点，把这个red变成black来补充删除的black
  如果伯父的右子节点red，变成black，把w变成x_p的颜色，x_p变成black，再左旋， black节点还是平衡的  调整结束
  如果伯父的左节点为red，现右旋，变成black，w变成red，和上一种一样
  也就是现考虑用w的右red来补充black，如果没有，则把左边通过旋转到右侧来处理

总结:删除的调整过程，如果伯父有red节点，则拿来补充，否则继续往上递归


