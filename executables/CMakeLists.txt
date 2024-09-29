add_executable(main main.cpp)
target_link_libraries(main PRIVATE engine board move utils perft evaluation cxx_setup)