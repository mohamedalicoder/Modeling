cmake_minimum_required(VERSION 3.10)
project(RNG_Suite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rng_suite 
    src/main.cpp
    src/generators/icg.cpp
    src/generators/mrg.cpp
    src/generators/lfg.cpp
    src/generators/msm.cpp
    src/generators/lcg.cpp
    src/generators/mcg.cpp
    src/tests/randomness_tests.cpp
    src/menu/menu_handler.cpp
)

target_include_directories(rng_suite PRIVATE include)