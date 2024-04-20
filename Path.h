#ifndef PATH_H
#define PATH_H

#include <vector>

class Path
{
public:

	std::vector<int> vertices; //vertexes in path
	std::vector<int> edges; //indexes to edges in a vertex's edge vector
};

#endif