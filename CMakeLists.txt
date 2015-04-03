cmake_minimum_required(VERSION 3.0)
project(par2)


set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror ")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Werror")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -Wall -Werror")
add_definitions(-D_FILE_OFFSET_BITS=64)

add_subdirectory(lib)
add_subdirectory(app)



