cmake_minimum_required(VERSION 3.23)
project(cpu6502)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(googletest)

add_executable(cpu6502 src/Computer.cpp src/Computer.h src/memory/Memory.cpp src/memory/Memory.h src/cpu/CPU.cpp src/cpu/CPU.h src/cpu/CPUexecute.cpp)
