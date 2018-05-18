mkdir build_test
cd build_test
cmake .. -Dtest_build=TRUE
ctest -V
