#ifndef PATH_H
#define PATH_H

#include <vector>

using namespace std;

class Path
{
public:
    vector<int> vertices; //vertexes in path
    vector<int> edges; //indexes to edges in a vertex's edge vector
};

#endif