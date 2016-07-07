//
// Created by hadoop on 7/7/16.
//

#ifndef MYSTL_HEAP_TEST_H
#define MYSTL_HEAP_TEST_H


#include <vector>
#include <iostream>
#include "../Heap.h"

using namespace std;
namespace TEST{
    void heap_test(){
        int myints[] = {10,20,30,5,15};
        std::vector<int> v(myints,myints+5);

        MySTL::make_heap (v.begin(),v.end());
        cout << "initial max heap   : " << v.front() << '\n';

        MySTL::pop_heap (v.begin(),v.end()); v.pop_back();
        std::cout << "max heap after pop : " << v.front() << '\n';

        v.push_back(99); MySTL::push_heap (v.begin(),v.end());
        std::cout << "max heap after push: " << v.front() << '\n';

        MySTL::sort_heap (v.begin(),v.end());

        std::cout << "final sorted range :";
        for (unsigned i=0; i<v.size(); i++)
            std::cout << ' ' << v[i];

        std::cout << '\n';
    }
}

#endif //MYSTL_HEAP_TEST_H
