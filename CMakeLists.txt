cmake_minimum_required(VERSION 3.30)

project(CustomClangTidyChecks)

set(CMAKE_CXX_STANDARD 23)

add_library(CustomClangTidyChecks SHARED)

add_subdirectory(bugprone)