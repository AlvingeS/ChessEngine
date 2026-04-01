add_library(uci
    uci.cpp
)
target_link_libraries(uci PUBLIC search cxx_setup Threads::Threads)