add_executable(test test_scan.cpp test_expr.cpp)
target_link_libraries(test rcc)

target_compile_features(test PRIVATE cxx_std_17)

include_directories(../src)
include_directories(${gtest_SOURCE_DIR}/include)
target_link_libraries(test gtest gtest_main)
