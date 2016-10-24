cmake_minimum_required(VERSION 3.6)
project(markov)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(include)
add_library(markov_chain SHARED src/markov_chain.cpp)

add_executable(chain_creator src/learning.cpp)
add_executable(chain_exec src/executing.cpp)
target_link_libraries(chain_creator markov_chain)
target_link_libraries(chain_exec markov_chain)