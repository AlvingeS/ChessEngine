add_library(search
    Searcher.cpp
    SearchMemory.cpp
)
target_link_libraries(search PUBLIC evaluation makemove movegen cxx_setup)