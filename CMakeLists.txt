cmake_minimum_required(VERSION 3.12)
project(ex4_os)

set(CMAKE_CXX_STANDARD 11)

add_executable(os_ex4
        MemoryConstants.h
        PhysicalMemory.h
        PhysicalMemory.cpp
        VirtualMemory.h
        VirtualMemory.cpp
        SimpleTest.cpp)