#include "WUDGraph.h"
#include "WUDEdge.h"

using namespace std;

//Create an undirected graph G_u from the original directed graph G using the following rules:
//1. For each pair of vertices u and v, if there is only one directed edge(either (u,v) or (v,u)) between them, you keep that single edge with its corresponding cost as an undirected weighted edge. You can ignore the distance on that edge.
//2. For each pair of vertices u and v, if there are two directed edges (u,v) and (v, u) between them, you keep the one with the minimum cost value as an undirected weighted edge. You can ignore the distance on that edge
WUDGraph::WUDGraph(WDGraph* graph)
{
    for (int i = 0; i < graph->vertices.size(); i++)
    {
        vertices.push_back(WUDVertex(graph->vertices[i].airportCode, vector<WUDEdge>()));
    }

    for (int i = 0; i < graph->edges.size(); i++)
    {
        edges.push_back(vector<WUDEdge>());
        for (int j = 0; j < graph->edges[i].size(); j++)
        {
            edges[i].push_back(graph->edges[i][j]);
            vertices[graph->edges[i][j].src].connectedEdges.push_back(WUDEdge(graph->edges[i][j]));
            vertices[graph->edges[i][j].dest].connectedEdges.push_back(WUDEdge(graph->edges[i][j]));
        }
    }

    // Print all vertices with connected edges
//    for (int i = 0; i < vertices.size(); i++)
//    {
//        cout << vertices[i].airportCode << ": ";
//        for (int j = 0; j < vertices[i].connectedEdges.size(); j++)
//        {
//            cout << vertices[i].connectedEdges[j].vertexes[0] << "->" << vertices[i].connectedEdges[j].vertexes[1] << " ";
//        }
//        cout << endl;
//    }

    // Remove duplicate edges
    for (int i = 0; i < edges.size(); i++)
    {
        for (int j = 0; j < edges[i].size(); j++)
        {
            for (int k = j + 1; k < edges[i].size(); k++)
            {
                if (edges[i][j].vertexes[1] == edges[i][k].vertexes[0] && edges[i][j].vertexes[0] == edges[i][k].vertexes[1])
                {
                    if (edges[i][j].cost > edges[i][k].cost)
                    {
                        edges[i].erase(edges[i].begin() + j);
                    }
                    else
                    {
                        edges[i].erase(edges[i].begin() + k);
                    }
                }
            }
        }
    }
}

bool WUDGraph::IsGraphConnected() const
{
    vector<bool> visited(vertices.size(), false);
    vector<int> stack;
    stack.push_back(0);
    visited[0] = true;

    while (!stack.empty())
    {
        int vertex = stack.back();
        stack.pop_back();

        for (int i = 0; i < vertices[vertex].connectedEdges.size(); i++)
        {
            for (int j = 0; j < vertices[vertex].connectedEdges[i].vertexes.size(); j++)
            {
                if (!visited[vertices[vertex].connectedEdges[i].vertexes[j]])
                {
                    stack.push_back(vertices[vertex].connectedEdges[i].vertexes[j]);
                    visited[vertices[vertex].connectedEdges[i].vertexes[j]] = true;
                }
            }
        }
    }

    for (int i = 0; i < visited.size(); i++)
    {
        if (!visited[i])
        {
//            cout << "Vertex " << i << " is not connected" << endl;
            return false;
        }
    }

    return true;
}

void WUDGraph::PrintMST(WDGraph mst, string title) const
{
    if (mst.vertices.empty())
    {
        return;
    }

    cout << title << ":" << endl;
    cout << "Edge                Weight" << endl;

    int totalCost = 0;
    for (int i = 0; i < mst.edges.size(); i++)
    {
        for (int j = 0; j < mst.edges[i].size(); j++)
        {
            totalCost += mst.edges[i][j].cost;
            cout << mst.vertices[mst.edges[i][j].src].airportCode << "->" << mst.vertices[mst.edges[i][j].dest].airportCode << "            " << mst.edges[i][j].cost << endl;
        }
    }

    cout << endl << "Total cost of MST: " << totalCost << endl;
}

WDGraph WUDGraph::PrimMST() const
{
    if (!IsGraphConnected())
    {
        cout << "Graph is not connected, can't generate MST using Prim's algoritm." << endl;
        return WDGraph();
    }

    WDGraph mst;
    vector<bool> visitedVertexes(vertices.size(), false);
    vector<WUDEdge> unvisitedEdges;
    unvisitedEdges.push_back(edges[0][0]);

    while (!unvisitedEdges.empty())
    {
        // Find smallest cost edge
        WUDEdge smallestEdge = unvisitedEdges[0];
        int minCost = INT_MAX;
        for (int i = 0; i < unvisitedEdges.size(); i++)
        {
            if (unvisitedEdges[i].cost < minCost)
            {
                minCost = unvisitedEdges[i].cost;
                smallestEdge = unvisitedEdges[i];
            }
        }

        // Remove smallest cost edge from unvisited edges
        for (int i = 0; i < unvisitedEdges.size(); i++)
        {
            if (unvisitedEdges[i].vertexes[0] == smallestEdge.vertexes[0] && unvisitedEdges[i].vertexes[1] == smallestEdge.vertexes[1])
            {
                unvisitedEdges.erase(unvisitedEdges.begin() + i);
                break;
            }
        }

        mst.add_vertex(vertices[smallestEdge.vertexes[0]]);
        mst.add_vertex(vertices[smallestEdge.vertexes[1]]);

        if (visitedVertexes[smallestEdge.vertexes[0]] && visitedVertexes[smallestEdge.vertexes[1]])
        {
            continue;
        }
        mst.add_edge(vertices[smallestEdge.vertexes[0]], vertices[smallestEdge.vertexes[1]], 0, smallestEdge.cost);

        for (int i = 0; i < smallestEdge.vertexes.size(); i++)
        {
            for (int j = 0; j < vertices[smallestEdge.vertexes[i]].connectedEdges.size(); j++)
            {
                if (!visitedVertexes[vertices[smallestEdge.vertexes[i]].connectedEdges[j].vertexes[0]])
                {
                    unvisitedEdges.push_back(vertices[smallestEdge.vertexes[i]].connectedEdges[j]);
                }
            }
        }

        visitedVertexes[smallestEdge.vertexes[0]] = true;
        visitedVertexes[smallestEdge.vertexes[1]] = true;
    }

    return mst;
}

WDGraph WUDGraph::KruskalMST() const
{
    WDGraph mst;
    vector<WUDEdge> edges;
    for (int i = 0; i < this->edges.size(); i++)
    {
        for (int j = 0; j < this->edges[i].size(); j++)
        {
            edges.push_back(this->edges[i][j]);
        }
    }
    for (int i = 0; i < edges.size(); i++)
    {
        for (int j = i + 1; j < edges.size(); j++)
        {
            if (edges[i].cost > edges[j].cost)
            {
                WUDEdge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }
    vector<int> components(vertices.size());
    for (int i = 0; i < components.size(); i++)
    {
        components[i] = i;
    }

    int totalCost = 0;
    for (int i = 0; i < edges.size(); i++)
    {
        if (components[edges[i].vertexes[0]] != components[edges[i].vertexes[1]])
        {
            totalCost += edges[i].cost;
            mst.add_vertex(vertices[edges[i].vertexes[0]]);
            mst.add_vertex(vertices[edges[i].vertexes[1]]);
            mst.add_edge(vertices[edges[i].vertexes[0]], vertices[edges[i].vertexes[1]], 0, edges[i].cost);
            int oldComponent = components[edges[i].vertexes[1]];
            for (int j = 0; j < components.size(); j++)
            {
                if (components[j] == oldComponent)
                {
                    components[j] = components[edges[i].vertexes[0]];
                }
            }
        }
    }
    return mst;
}