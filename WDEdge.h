#ifndef WDEDGE_H
#define WDEDGE_H

class WDEdge
{
public:
	int src;
	int dest;
	int weight;

	WDEdge()
	{
		src = 0;
		dest = 0;
		weight = 0;
	}
	WDEdge(int srcInt, int destInt, int weightInt)
	{
		src = srcInt;
		dest = destInt;
		weight = weightInt;
	}
};

#endif