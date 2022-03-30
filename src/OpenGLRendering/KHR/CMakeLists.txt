cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(KHR_LIB INTERFACE)
target_include_directories(KHR_LIB INTERFACE ${CMAKE_CURRENT_LIST_DIR})