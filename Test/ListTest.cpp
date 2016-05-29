//
// Created by JQK on 2016/5/29.
//

#include "../List.h"

#include <iostream>

using namespace std;

void push_back_test(){

    MySTL::list<int> l;
    MySTL::list<int>::iterator iter;

    l.push_back(1);
    cout << "size : 1" << l.size() << endl;
    for (iter = l.begin();iter != l.end(); iter++)
        cout << *iter << " ";
    cout << endl;

    l.push_back(2);
    cout << "size : 2" << l.size() << endl;
    for (iter = l.begin();iter != l.end(); iter++)
        cout << *iter << " ";
    cout << endl;

    l.push_back(3);
    cout << "size : 3" << l.size() << endl;
    for (iter = l.begin();iter != l.end(); iter++)
        cout << *iter << " ";
    cout << endl;

    l.push_back(4);
    cout << "size : 4" << l.size() << endl;
    for (iter = l.begin();iter != l.end(); iter++)
        cout << *iter << " ";
    cout << endl;

}
