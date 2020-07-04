cmake_minimum_required(VERSION 3.16)
project(Assignment_4)

set(CMAKE_CXX_STANDARD 11)

set(Proj_sources kampmeier_aaron_BinarySearchTree.h kampmeier_aaron_FrequencyAnalyzer.cpp kampmeier_aaron_FrequencyAnalyzer.h)

option(Proj_test "Build tests." ON)

if (Proj_test)
    add_executable(Assignment_4_Test tests.cpp ${Proj_sources})
endif()


add_executable(Assignment_4 kampmeier_aaron_hw4.cpp ${Proj_sources})
