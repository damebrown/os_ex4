cmake_minimum_required(VERSION 3.12)
project(ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(os_ex4
        MemoryConstants.h
        PhysicalMemory.h
        PhysicalMemory.cpp
        VirtualMemory.h
        VirtualMemory.cpp
        SimpleTest.cpp)