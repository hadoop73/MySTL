//
// Created by hadoop on 7/6/16.
//

#ifndef MYSTL_DEQUETEST_H
#define MYSTL_DEQUETEST_H

#include "../Deque.h"
#include <iostream>

using namespace std;

namespace TEST{
    void deque_push_test(){
        MySTL::deque<int,MySTL::alloc,8> dq;
        for (int i = 0; i < 8; ++i) {
            dq.push_back(i);
        }
        cout << dq.size()<< endl;
        for (int j = 5; j < 10; ++j) {
            dq.push_front(j*j);
        }
        cout << dq.size()<< endl;
    }
}


#endif //MYSTL_DEQUETEST_H
