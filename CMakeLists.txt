cmake_minimum_required(VERSION 2.8.4)
project(ntrp)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(src)

file(GLOB_RECURSE LIB_SRC src/lib/*.cpp)
add_library(ntrp ${LIB_SRC})
target_link_libraries(ntrp PocoFoundation PocoJSON PocoData PocoDataSQLite)

file(GLOB_RECURSE SRV_SRC src/srv/*.cpp)
add_executable(ntrp-srv ${SRV_SRC})
target_link_libraries(ntrp-srv ntrp PocoFoundation PocoUtil PocoNet)

file(GLOB_RECURSE CL_SRC src/cl/*.cpp)
add_executable(ntrp-cl ${CL_SRC})
target_link_libraries(ntrp-cl ntrp PocoFoundation PocoUtil PocoNet)

