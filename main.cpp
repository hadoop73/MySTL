#include <iostream>


#include "Test/Map_Test.h"

using namespace std;

int main() {

    TEST::map_insert_test();
    /*
    // 测试 vector 插入数据，扩容问题
    MySTL::vector<int> svec;
    cout << "svec.capacity(): " << svec.capacity() << endl;
    svec.push_back(1);
    cout << "svec.capacity(1): " << svec.capacity() << endl;
    svec.push_back(2);
    cout << "svec.capacity(2): " << svec.capacity() << endl;
    svec.push_back(3);
    cout << "svec.capacity(3): " << svec.capacity() << endl;
    svec.push_back(4);
    cout << "svec.capacity(4): " << svec.capacity() << endl;
    svec.push_back(5);
    cout << "svec.capacity(5): " << svec.capacity() << endl;
    svec.push_back(6);
    cout << "svec.capacity(6): " << svec.capacity() << endl;
    svec.push_back(7);
    cout << "svec.capacity(7): " << svec.capacity() << endl;
    svec.push_back(8);
    cout << "svec.capacity(8): " << svec.capacity() << endl;
    svec.push_back(9);
    cout << "svec.capacity(9): " << svec.capacity() << endl;
    svec.push_back(10);
    cout << "svec.capacity(10): " << svec.capacity() << endl;*/


    //for (int i = 0; i < 1000000; ++i) {
        //cout << i << "  hello world " << (i)* sizeof(int) << endl;
        //auto p = MySTL::alloc::allocate( (i)* sizeof(int)  );
        //int array[100];
        //MySTL::uninitialized_fill_n(array,100,50);
    //for (int i = 0; i < 100; ++i) {
     //   MySTL::uninitialized_fill(array,array+100,i);
    //}
      //  for(auto n: array){
      //      cout << n << endl;
      //  }
        //auto p = MySTL::allocator<int>::allocate();
        //MySTL::allocator<int>::construct(p,i);
        //MySTL::allocator<int>::destroy(p);
        //MySTL::allocator<int>::deallocate(p);
    //}
    //system("pause");
    return 0;
}