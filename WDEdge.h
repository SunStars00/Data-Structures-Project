#ifndef WDEDGE_H
#define WDEDGE_H

class WDEdge
{
public:
	int src;
	int dest;
	int weight;
	int cost;

	WDEdge()
	{
		src = 0;
		dest = 0;
		weight = 0;
		cost = 0;
	}
	WDEdge(int srcInt, int destInt, int weightInt, int costInt)
	{
		src = srcInt;
		dest = destInt;
		weight = weightInt;
		cost = costInt;
	}
};

#endif