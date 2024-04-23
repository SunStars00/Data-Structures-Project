cmake_minimum_required(VERSION 3.26)
project(Data_Structures_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Data_Structures_Project
        TestDirectedGraph.cpp
        AirportStatePair.cpp
        AirportStatePair.h
        MinHeap.h
        Path.h
        WDEdge.h
        WDVertex.h
        WDGraph.cpp
        WDGraph.h
        WUDEdge.h
        WUDVertex.h
        WUDGraph.cpp
        WUDGraph.h)