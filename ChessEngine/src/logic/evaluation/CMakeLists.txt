add_library(evaluation
    Evaluator.cpp
)

target_link_libraries(evaluation PUBLIC board cxx_setup)