
##  插入
首先，查找要插入的位置
```cpp
while (x != 0){
     y = x;
     cmp = key_compare(KeyOfValue()(v),key(x));  //  v 小于 x,则插入左边,否则放在右边
     x = cmp?left(x):right(x);
} // 找到 v 的插入点
```