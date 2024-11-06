# TODO 14: Remove mysqrt.cxx from the list of sources

# TODO 1: Add a library called MathFunctions with sources MathFunctions.cxx
# and mysqrt.cxx

add_compile_options(-g)

add_library(TALib
    timed_automata.cpp
    regioned_timed_automata.cpp)

target_include_directories(TALib PUBLIC
    "${CMAKE_SOURCE_DIR}/src/include"
)

