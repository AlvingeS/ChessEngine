add_executable(masks_test
    RayBitMasks_test.cpp
    KnightBitMasks_test.cpp
    KingBitMasks_test.cpp
    PawnBitMasks_test.cpp
)

target_link_libraries(masks_test PRIVATE masks GTest::gtest_main)
gtest_discover_tests(masks_test)