add_executable(movegen_tests)

target_sources(movegen_tests PRIVATE
    check_detection_test.cpp
)

target_link_libraries(movegen_tests PRIVATE makemove perft movegen position test_support GTest::gtest_main)

gtest_discover_tests(movegen_tests)
