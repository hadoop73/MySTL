//
// Created by hadoop on 6/4/16.
//

#ifndef MYSTL_RB_TREE_TEST_H
#define MYSTL_RB_TREE_TEST_H

#include "../RB_Tree.h"
#include <iostream>

using namespace std;

namespace TEST{
    void rb_insert_test(){
        MySTL::rb_tree<int,int,_Identity<int>,less<int>> itree;
        cout << "Size: " << itree.size() << endl;

        itree.insert_unique(10);
        itree.insert_unique(7);
        itree.insert_unique(8);

        itree.insert_unique(15);
        itree.insert_unique(5);
        itree.insert_unique(6);

        itree.insert_unique(11);

        itree.insert_unique(13);
        itree.insert_unique(12);

        cout  << "size :" << itree.size() << endl;
        auto itr1 = itree.begin();
        auto itr2 = itree.end();

        MySTL::rb_tree_base_iterator rb;
        for (;itr1 != itr2; ++itr1){
            //rb = MySTL::rb_tree_base_iterator(itr1);
            cout << *itr1 << "  ";
        }
        cout << endl;

    }
}


#endif //MYSTL_RB_TREE_TEST_H
