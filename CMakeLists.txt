cmake_minimum_required(VERSION 3.30)

project(CustomClangTidyChecks)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(CustomClangTidyChecks SHARED)

target_link_libraries(CustomClangTidyChecks PRIVATE
  clang-cpp
  rapidfuzz::rapidfuzz
)

add_subdirectory(3rdparty/RapidFuzz)
add_subdirectory(bugprone)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
