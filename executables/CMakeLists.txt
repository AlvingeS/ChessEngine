add_executable(main main.cpp)
target_link_libraries(main PRIVATE uci perft perft io cxx_setup Threads::Threads)