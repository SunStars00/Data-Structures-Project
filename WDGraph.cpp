#include "WDGraph.h"

void WDGraph::add_vertex(const WDVertex& ver)
{
    if (get_vertex_index(ver) == -1) {
        vertices.push_back(ver);
        std::vector<WDEdge> tmp;
        edges.push_back(tmp);
    }
}

void WDGraph::add_edge(const WDVertex& ver1, const WDVertex& ver2, int distance, int cost)
{
    int i1 = get_vertex_index(ver1);
    int i2 = get_vertex_index(ver2);
    if (i1 == -1 || i2 == -1) {
        throw std::string("add_edge: ver1 or ver2 are not vertices in the graph");
    }
    WDEdge edge(i1, i2, distance, cost);
    edges[i1].push_back(edge);
}

int WDGraph::get_vertex_index(const WDVertex& ver) const
{
    for(int i = 0; i < vertices.size(); i++) {
        if (vertices[i].CompareCode(ver.airportCode)) {
            return i;
        }
    }

    return -1;
}

const WDVertex& WDGraph::GetVertexAtIndex(int index) const
{
    return vertices[index];
}

int WDGraph::SearchForCodeIndex(char* code)
{
    for (int i = 0; i < vertices.size(); i++)
        if (vertices[i].CompareCode(code))
            return i;
    return -1;
}

const WDVertex& WDGraph::SearchForCode(char* code)
{
    for (int i = 0; i < vertices.size(); i++)
        if (vertices[i].CompareCode(code))
            return vertices[i];
    throw std::string("Error in SearchForCode: could not find code");
}

void WDGraph::print() const
{
}

void WDGraph::clean_visited()
{
	for (int i = 0; i < vertices.size(); i++)
		vertices[i].visited = false;
}
