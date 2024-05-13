add_library(search
    Searcher.cpp
    SearchMemory.cpp
)
target_link_libraries(search PUBLIC movegen game evaluation cxx_setup)