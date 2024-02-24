add_library(search
    Searcher.cpp
)
target_link_libraries(search PUBLIC game evaluation cxx_setup)