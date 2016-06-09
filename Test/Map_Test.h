//
// Created by hadoop on 6/8/16.
//

#ifndef MYSTL_MAP_TEST_H
#define MYSTL_MAP_TEST_H

#include "../Map.h"
#include <iostream>
using namespace std;

namespace TEST{

    void map_insert_test(){
        MySTL::map<string,int> smap;
        smap[string("jjhou")] = 1;
        smap[string("jerry")] = 2;

        cout << "size£º" << smap.size() << endl;
    }
}

#endif //MYSTL_MAP_TEST_H
