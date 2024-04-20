#ifndef WDGRAPH_H
#define WDGRAPH_H

#include <iostream>
#include <vector>

#include "WDVertex.h"
#include "WDEdge.h"
#include "Path.h"
#include "AirportParser.h"

class WDGraph
{
public:
    WDGraph() {}

    void add_vertex(const WDVertex& ver);
    void add_edge(const WDVertex& ver1, const WDVertex& ver2, int distance, int cost); //connect ver1 to ver2
    
    int get_vertex_index(const WDVertex& ver) const;
    const WDVertex& GetVertexAtIndex(int index) const;

    int SearchForCodeIndex(const char* code) const; //code should point to three characters
    const WDVertex& SearchForCode(const char* code) const; //code should point to three characters, return value is liable to invalidation

    Path GetShortestPath(int originIndex, int destIndex, bool doPrint = true) const;
    std::vector<Path> GetShortestPathsToState(int originIndex, const char* stateCode, const AirportParser& apParser, bool doPrint = true) const;
    
    int GetPathDist(const Path& path) const;
    void PrintPath(const Path& path) const;
    void PrintPaths(const std::vector<Path>& path) const;

    std::vector<WDVertex> vertices;
    std::vector<std::vector<WDEdge>> edges;

    void clean_visited();
};

#endif