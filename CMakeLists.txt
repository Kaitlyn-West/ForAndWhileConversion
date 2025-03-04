cmake_minimum_required(VERSION 3.25)

project(ForAndWhileEquivalence VERSION 1.0 DESCRIPTION "Reads file containing a for or a while loop and converts it to the")

add_executable(ForAndWhileEquivalence ForAndWhileEquivalence.cpp)
target_include_directories(ForAndWhileEquivalence PUBLIC "include/")