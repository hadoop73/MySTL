#include <iostream>
#include <memory>

#include "Alloc.h"
#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunctions.h"

using namespace std;

int main() {
    //for (int i = 0; i < 1000000; ++i) {
        //cout << i << "  hello world " << (i)* sizeof(int) << endl;
        //auto p = MySTL::alloc::allocate( (i)* sizeof(int)  );
        int array[100];
        //MySTL::uninitialized_fill_n(array,100,50);
    for (int i = 0; i < 100; ++i) {
        MySTL::uninitialized_fill(array,array+100,i);
    }
        for(auto n: array){
            cout << n << endl;
        }
        //auto p = MySTL::allocator<int>::allocate();
        //MySTL::allocator<int>::construct(p,i);
        //MySTL::allocator<int>::destroy(p);
        //MySTL::allocator<int>::deallocate(p);
    //}
    //system("pause");
    return 0;
}