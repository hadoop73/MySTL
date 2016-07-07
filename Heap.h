//
// Created by hadoop on 7/7/16.
//

#ifndef MYSTL_HEAP_H
#define MYSTL_HEAP_H

#include "Iterator.h"

namespace MySTL{


    template <class RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first,
                        RandomAccessIterator last){
        push_heap_aux(first,last,distance_type(first),value_type(first));
    }

    template <class RandomAccessIterator,class Distance,class T>
    inline void push_heap_aux(RandomAccessIterator first,
                        RandomAccessIterator last,Distance*,T*){
        _push_heap(first,Distance((last - first) - 1),Distance(0),T(*(last - 1)));
    };

    template <class RandomAccessIterator,class Distance,class T>
    inline void _push_heap(RandomAccessIterator first,Distance holeIndex,
                    Distance topIndex,T value){
        Distance parent = (holeIndex-1)/2;
        while (holeIndex > topIndex && *(first+parent) < value){  // ´ó¶¥¶Ñ
            *(first+holeIndex) = *(first+parent);
            holeIndex = parent;
            parent=(holeIndex-1)/2;
        }
        *(first+holeIndex) = value;
    };


}


#endif //MYSTL_HEAP_H
