cmake_minimum_required(VERSION 3.6)
project(markov)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)
set(SOURCE_FILES src/main.cpp src/markovchain.cpp)


add_executable(chain_creator ${SOURCE_FILES})