#ifndef WDGRAPH_H
#define WDGRAPH_H

#include <vector>

#include "WDVertex.h"
#include "WDEdge.h"

class WDGraph
{
public:
    WDGraph() {}

    void add_vertex(const WDVertex& ver);
    void add_edge(const WDVertex& ver1, const WDVertex& ver2, int distance, int cost); //connect ver1 to ver2
    
    int get_vertex_index(const WDVertex& ver) const;
    const WDVertex& GetVertexAtIndex(int index) const;

    int SearchForCodeIndex(char* code); //code should point to three characters
    const WDVertex& SearchForCode(char* code); //code should point to three characters


    void print() const;

    std::vector<WDVertex> vertices;
    std::vector<std::vector<WDEdge>> edges;

    void clean_visited();
};

#endif