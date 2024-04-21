cmake_minimum_required(VERSION 3.24)
project(Data_Structures_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Data_Structures_Project
    AirportParser.cpp
    AirportParser.h
    Path.h
    TestDirectedGraph.cpp
    WDEdge.h
    WDGraph.cpp
    WDGraph.h
    WDVertex.h
        WUDGraph.cpp
        WUDGraph.h
        WUDEdge.h
        WUDVertex.h
)