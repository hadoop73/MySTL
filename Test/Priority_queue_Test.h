//
// Created by hadoop on 7/7/16.
//

#ifndef MYSTL_PRIORITY_QUEUE_TEST_H
#define MYSTL_PRIORITY_QUEUE_TEST_H


#include "../Priority_queue.h"

#include <iostream>

using namespace std;

namespace TEST{

 void priority_queue_test(){
     int ia[9] = {0,1,2,3,4,8,9,3,5};
     MySTL::priority_queue<int> ipq(ia,ia+9);
     std::cout << "size=" << ipq.size() << endl;

     for (int i=0;i<ipq.size();++i)
         std::cout<< ipq.top() << ' ';
     std::cout << endl;

     while(!ipq.empty()){
         std::cout << ipq.top() << ' ';
         ipq.pop();
     }
     cout << endl;
 }

}

#endif //MYSTL_PRIORITY_QUEUE_TEST_H
