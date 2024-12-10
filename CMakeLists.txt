cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(DSMProject VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# configure a header file to pass some of the CMake settings
# to the source code
add_compile_options(-g)

add_subdirectory(src/lib)
add_subdirectory(src/lib/gates)

# add the executable
add_executable(test1 test/test1.cpp)
add_executable(test2 test/test2.cpp)
add_executable(test3 test/test3.cpp)
add_executable(test4 test/test4.cpp)

# link_directories(${PROJECT_SOURCE_DIR}/dependency)

target_link_libraries(test1 TALib GateLib)
target_link_libraries(test2 TALib GateLib)
target_link_libraries(test3 TALib GateLib)
target_link_libraries(test4 TALib GateLib)

target_include_directories(test1 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/include"
                          )
target_include_directories(test2 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/include"
                          )
target_include_directories(test3 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/include"
                          )
target_include_directories(test4 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/include"
                          )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
