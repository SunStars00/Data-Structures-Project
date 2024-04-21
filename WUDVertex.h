#ifndef WUDVERTEX_H
#define WUDVERTEX_H

#include "WUDEdge.h"

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