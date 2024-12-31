add_library(search
    Searcher.cpp
    SearchMemory.cpp
)
target_link_libraries(search PUBLIC io evaluation makemove movegen cxx_setup)