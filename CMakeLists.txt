cmake_minimum_required(VERSION 3.2)
project(MySTL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Test/RB_Tree_Test.h)
add_executable(TinySTL ${SOURCE_FILES} TypeTraits.h Construct.h Alloc.h Allocator.h malloc_alloc.h UninitializedFunctions.h Iterator.h Vector.h List.h Test/ListTest.h RB_Tree.h)