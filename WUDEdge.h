#ifndef WUDEDGE_H
#define WUDEDGE_H

#include <vector>

using namespace std;

using namespace std;

class WUDEdge : public WDEdge
{
public:
    vector<int> vertexes;

    WUDEdge(WDEdge wdEdge) : WDEdge(wdEdge.src, wdEdge.dest, wdEdge.distance, wdEdge.cost)
    {
        vertexes.push_back(wdEdge.src);
        vertexes.push_back(wdEdge.dest);
    }

    WUDEdge(int srcInt, int destInt, int costInt) : WDEdge(srcInt, destInt, 0, costInt)
    {
        vertexes.push_back(srcInt);
        vertexes.push_back(destInt);
    }
};

#endif