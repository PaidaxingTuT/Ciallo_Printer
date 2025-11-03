cmake_minimum_required(VERSION 3.28)
project(Ciallo_Printer)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ciallo_Printer src/main.cpp) 


target_link_libraries(Ciallo_Printer libeasyx.a)

target_link_libraries(Ciallo_Printer easyx winmm)