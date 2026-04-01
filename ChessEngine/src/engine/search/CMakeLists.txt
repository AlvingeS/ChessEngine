add_library(search
    searcher.cpp
    search.cpp
    negamax.cpp
    quiesence.cpp
    time_manager.cpp
    transposition_table.cpp
    game_history.cpp
)
target_link_libraries(search PUBLIC io eval makemove movegen uci cxx_setup Threads::Threads)