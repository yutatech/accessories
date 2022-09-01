cmake_minimum_required(VERSION 3.13)

enable_language(CXX)
project(remoconCrawler)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(rpi4)

add_executable(remoconCrawler remoconCrawler.cpp ../remoconEmulator/remoconEmulator.cpp)
target_link_libraries(remoconCrawler RPI4)