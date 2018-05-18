# Toolchain:

set (build_environment "$ENV{R2D2_BUILD_ENVIRONMENT}") # Your path to kvasir_toolchain

if (build_environment)
set (toolchain ${build_environment}/toolchain)
else (build_environment)
set (build_environment ../..)
set (toolchain ../../toolchain)
endif (build_environment)

# Libraries:

link_libraries (gcc)

set (hwlib ${build_environment}/libraries/hwlib)
include_directories (${hwlib}/library)
add_definitions (-DBMPTK_TARGET_arduino_due
                 -DBMPTK_TARGET=arduino_due
                 -DBMPTK_BAUDRATE=19200)

set (catch ${build_environment}/libraries/Catch2)
include_directories (${catch}/single_include)

# Source Files:

set (sources
    src/wrap-hwlib.cpp
    src/libc-stub.cpp
    src/main.cpp
)
