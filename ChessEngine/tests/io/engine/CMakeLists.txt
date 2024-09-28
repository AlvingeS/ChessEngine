add_executable(engine_tests
    Engine_test.cpp
)

target_link_libraries(engine_tests PRIVATE engine GTest::gtest_main)
gtest_discover_tests(engine_tests)