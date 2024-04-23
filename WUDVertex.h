#ifndef WUDVERTEX_H
#define WUDVERTEX_H

#include "WUDEdge.h"
#include "WDVertex.h"

class WUDVertex : public WDVertex
{
public:
    vector<WUDEdge> connectedEdges;

    WUDVertex(const char* airportCodeChars, vector<WUDEdge> connectedEdgesVector) : WDVertex(airportCodeChars)
    {
        connectedEdges = connectedEdgesVector;
    }
};

#endif