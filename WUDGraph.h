#ifndef WUDGRAPH_H
#define WUDGRAPH_H

#include "WDGraph.h"
#include "WUDEdge.h"
#include "WUDVertex.h"

using namespace std;

class WUDGraph {
public:
    WUDGraph(WDGraph* graph);

    bool IsGraphConnected() const;
    WDGraph PrimMST() const;
    WDGraph KruskalMST() const;

    void PrintMST(WDGraph mst, string title) const;

    vector<WUDVertex> vertices;
    vector<vector<WUDEdge>> edges;
};

#endif