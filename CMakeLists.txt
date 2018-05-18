cmake_minimum_required (VERSION 2.8.11)

option (test_build "test_build" FALSE)

include (Sources.cmake)

if (NOT ${test_build})
set (CMAKE_TOOLCHAIN_FILE ${toolchain}/compilers/arm-none-eabi.cmake)
else (NOT ${test_build})
set (CMAKE_TOOLCHAIN_FILE ${toolchain}/compilers/host-gcc.cmake)
endif (NOT ${test_build})

project (arduino-template CXX)

set (build_test_enabled TRUE)
set (unit_test_enabled TRUE)
set (complexity_test_enabled TRUE)
set (memcheck_enabled TRUE)
set (unit_test_main test/test_main.cpp)

if (NOT ${test_build})
include (BuildModule.cmake)
else (NOT ${test_build})
include (TestModule.cmake)
endif (NOT ${test_build})
