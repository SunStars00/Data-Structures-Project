#ifndef WDEDGE_H
#define WDEDGE_H

class WDEdge
{
public:
    int src;
    int dest;
    int distance;
    int cost;

    WDEdge()
    {
        src = 0;
        dest = 0;
        distance = 0;
        cost = 0;
    }
    WDEdge(int srcInt, int destInt, int distanceInt, int costInt)
    {
        src = srcInt;
        dest = destInt;
        distance = distanceInt;
        cost = costInt;
    }
};

#endif