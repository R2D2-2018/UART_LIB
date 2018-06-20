include (CTest)

include (${build_environment}/flags.cmake)

include_directories (src/)

add_definitions (-DBMPTK_TARGET_test
                 -DBMPTK_TARGET=test
                 -DBMPTK_BAUDRATE=19200)

set (sources ${sources}
    ${unit_test_main}
    src/wrap-hwlib.cpp
    src/libc-stub.cpp
)

set (build_test build_test)
set (unit_test unit_test)
set (memcheck memcheck)
set (complexity_test complexity_test)
set (clangformat_test clangformat_test)

if (NOT DEFINED cyclomatic_complexity_warning)
SET(cyclomatic_complexity_warning 10)
endif (NOT DEFINED cyclomatic_complexity_warning)

if (test_build)
enable_testing ()

if (build_test_enabled)
add_test (
	NAME ${build_test}
	COMMAND ${CMAKE_COMMAND} --build .
)
endif (build_test_enabled)

if (unit_test_enabled)
add_executable (${unit_test} ${unit_test_main} ${sources})

add_test (
	NAME ${unit_test}
	COMMAND ./${unit_test}
)

set_tests_properties (
	${unit_test} PROPERTIES
	DEPENDS ${build_test}
)
endif (unit_test_enabled)

if (complexity_test_enabled)
add_test (
	NAME ${complexity_test}
	COMMAND lizard -C ${cyclomatic_complexity_warning} --warnings_only .
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
endif (complexity_test_enabled)

if (UNIX AND memcheck_enabled)
add_test (
	NAME ${memcheck}
	COMMAND valgrind --leak-check=full --error-exitcode=1 --errors-for-leak-kinds=definite,possible ./${unit_test}
)

set_tests_properties (
	${memcheck} PROPERTIES
	DEPENDS ${build_test}
)
endif (UNIX AND memcheck_enabled)
endif (test_build)

if (clang_format_test_enabled)
add_test (
	NAME ${clangformat_test}
	COMMAND python2 ${PROJECT_SOURCE_DIR}/.clang-format-compare.py ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/test
)
endif (clang_format_test_enabled)

# The target that is compiled for:
include (${toolchain}/targets/test/test.cmake)
