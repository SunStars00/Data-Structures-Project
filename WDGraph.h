#ifndef WDGRAPH_H
#define WDGRAPH_H

#include <iostream>
#include <vector>

#include "WDVertex.h"
#include "WDEdge.h"
#include "Path.h"
#include "AirportStatePair.h"
#include "MinHeap.h"

//class to represent a directed graph using WDVertexs and WDEdges, edges are weighted by distance, but there is also a cost value
class WDGraph
{
public:
    WDGraph() {}

    //add stuff to the graph
    void add_vertex(const WDVertex& ver);
    void add_edge(const WDVertex& ver1, const WDVertex& ver2, int distance, int cost); //connect ver1 to ver2
    
    //SearchForCodeIndex is preferred
    int get_vertex_index(const WDVertex& ver) const;
    //Get a reference to an index at the vertex vector, liable to invalidation when adding vertices
    const WDVertex& GetVertexAtIndex(int index) const;

    //Gets a vertex or index with a specific airport code
    int SearchForCodeIndex(const char* code) const; //code should point to three characters
    const WDVertex& SearchForCode(const char* code) const; //code should point to three characters, return value is liable to invalidation

    //Get's shortest path between two airports
    Path GetShortestPath(int originIndex, int destIndex, bool doPrint = true) const;
    Path GetShortestPath(const char* originCode, const char* destCode, bool doPrint = true) const { return GetShortestPath(SearchForCodeIndex(originCode),SearchForCodeIndex(destCode),doPrint); }
    //Get's shortest paths between an origin airport and each airport in a state
    std::vector<Path> GetShortestPathsToState(int originIndex, const char* stateCode, const AirportStatePair& apParser, bool doPrint = true) const;
    std::vector<Path> GetShortestPathsToState(const char* originCode, const char* stateCode, const AirportStatePair& apParser, bool doPrint = true) const {return GetShortestPathsToState(SearchForCodeIndex(originCode),stateCode, apParser, doPrint);}
    //Unoptimally gets the shortest path with a specific number of stops.
    Path GetShortestPathFixedStops(int originIndex, int destIndex, int stopCount, bool doPrint = true) const;
    Path GetShortestPathFixedStops(const char* originCode, const char* destCode, int stopCount, bool doPrint = true) const {return GetShortestPathFixedStops(SearchForCodeIndex(originCode),SearchForCodeIndex(destCode),stopCount,doPrint);}
    
    //Get total distance of a path
    int GetPathDist(const Path& path) const;
    //Print out a path and its info
    void PrintPath(const Path& path) const;
    //Print out a bunch of paths in a table
    void PrintPaths(const std::vector<Path>& path) const;
    //Print out a path with its number of stops
    void PrintPathStops(const Path& path, int stops) const;

    std::vector<WDVertex> vertices;
    std::vector<std::vector<WDEdge>> edges;

    //unused, visited is also unused. The visited variable of WDVertex would make graph functions thread unsafe if they were used
    void clean_visited();
};

#endif