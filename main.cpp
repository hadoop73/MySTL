#include <iostream>
#include <memory>

#include "Alloc.h"
#include "Allocator.h"
#include "Construct.h"

using namespace std;

int main() {
    for (int i = 28; i < 1000; ++i) {
        cout << i << "  hello world " << (i%128)* sizeof(int) << endl;
        //auto p = MySTL::alloc::allocate( (i%128)* sizeof(int)  );
        auto p = MySTL::allocator<int>::allocate();
        MySTL::allocator<int>::construct(p,i);
        MySTL::allocator<int>::destroy(p);
        MySTL::allocator<int>::deallocate(p);
    }
    //system("pause");
    return 0;
}