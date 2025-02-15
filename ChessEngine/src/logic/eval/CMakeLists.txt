add_library(eval
    Evaluator.cpp
)

target_link_libraries(eval PUBLIC board cxx_setup)