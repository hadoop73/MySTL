//
// Created by hadoop on 6/8/16.
//

#ifndef MYSTL_SET_TEST_H
#define MYSTL_SET_TEST_H

#include "../Set.h"

#include <iostream>

using namespace std;
namespace TEST{
    void set_insert_test(){
        MySTL::set<int> iset;
        cout << "size: " << iset.size() << endl;
        iset.insert(3);
        cout << "size: " << iset.size() << endl;
        iset.insert(3);
        cout << "size: " << iset.size() << endl;
        iset.insert(5);
        cout << "size: " << iset.size() << endl;
        iset.erase(3);
        cout << "size: " << iset.size() << endl;

    }


}

#endif //MYSTL_SET_TEST_H
