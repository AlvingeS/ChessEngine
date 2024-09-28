add_executable(masks_test
    RayBitmasks_test.cpp
    KnightBitmasks_test.cpp
    KingBitmasks_test.cpp
    PawnBitmasks_test.cpp
)

target_link_libraries(masks_test PRIVATE masks GTest::gtest_main)
gtest_discover_tests(masks_test)