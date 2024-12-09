
add_compile_options(-g)

add_library(GateLib
    sync.cpp
    wire.cpp
    and.cpp
    not.cpp
    df.cpp
    or.cpp
    clk.cpp
    mux.cpp
    nand.cpp
    nor.cpp
    xor.cpp
    xnor.cpp)

target_include_directories(GateLib PUBLIC
    "${CMAKE_SOURCE_DIR}/src/include"
)

