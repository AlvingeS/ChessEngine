add_executable(makemove_tests
    MakeUnmakeMove_test.cpp
)

target_link_libraries(makemove_tests PRIVATE movegen makemove test_support GTest::gtest_main)
gtest_discover_tests(makemove_tests)