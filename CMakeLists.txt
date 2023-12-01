cmake_minimum_required(VERSION 3.25)
project(festo23)

set(CMAKE_CXX_STANDARD 23)

if (MSVC)
    add_compile_options(/arch:AVX2)
else()
    add_compile_options(-mavx2)
endif()

file(COPY inputs DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

set(utils utils/decoder.cpp utils/fraction.cpp utils/keyTools.cpp)
include_directories(utils)

add_executable(tutorial_puzzle1 ${utils} tutorial/puzzle1.cpp)
add_executable(tutorial_puzzle2 ${utils} tutorial/puzzle2.cpp)
add_executable(tutorial_puzzle3 ${utils} tutorial/puzzle3.cpp)

add_executable(chapter1_puzzle1 ${utils} chapter1/puzzle1.cpp)
add_executable(chapter1_puzzle2 ${utils} chapter1/puzzle2.cpp)
add_executable(chapter1_puzzle3 ${utils} chapter1/puzzle3.cpp)


add_executable(chapter2_puzzle1 ${utils}  chapter2/puzzle1.cpp)
add_executable(chapter2_puzzle2 ${utils} chapter2/puzzle2.cpp)
add_executable(chapter2_puzzle3 ${utils} chapter2/puzzle3.cpp)


add_executable(chapter3_puzzle1 ${utils} chapter3/puzzle1.cpp)
add_executable(chapter3_puzzle2 ${utils} chapter3/puzzle2.cpp)
add_executable(chapter3_puzzle3 ${utils} chapter3/puzzle3.cpp)


add_executable(final_puzzle1 final/puzzle1.cpp)
add_executable(final_puzzle2 final/puzzle2.cpp)
add_executable(final_puzzle3 final/puzzle3.cpp)
