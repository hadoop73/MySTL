cmake_minimum_required(VERSION 3.2)
project(MySTL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Test/RB_Tree_Test.h Set.h Test/Set_Test.h Map.h Test/Map_Test.h Deque.h Test/DequeTest.h Heap.h Test/Heap_Test.h Priority_queue.h Test/Priority_queue_Test.h)
add_executable(TinySTL ${SOURCE_FILES} TypeTraits.h Construct.h Alloc.h Allocator.h malloc_alloc.h UninitializedFunctions.h Iterator.h Vector.h List.h Test/ListTest.h RB_Tree.h)