#include "WDGraph.h"

using namespace std;

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
        throw "add_edge: ver1 or ver2 are not vertices in the graph";
    }
    WDEdge edge(i1, i2, distance, cost);
    edges[i1].push_back(edge);
}

int WDGraph::get_vertex_index(const WDVertex& ver) const
{
    for (int i = 0; i < vertices.size(); i++) {
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

int WDGraph::SearchForCodeIndex(const char* code) const
{
    for (int i = 0; i < vertices.size(); i++)
        if (vertices[i].CompareCode(code))
            return i;
    return -1;
}

const WDVertex& WDGraph::SearchForCode(const char* code) const
{
    for (int i = 0; i < vertices.size(); i++)
        if (vertices[i].CompareCode(code))
            return vertices[i];
    throw std::string("Error in SearchForCode: could not find code");
}

Path WDGraph::GetShortestPath(int originIndex, int destIndex, bool doPrint) const
{
    if (originIndex == destIndex) //return empty path if origin and dest are the same
        return Path();

    //should really use thread_local global vars here so a memory isn't allocated everytime the function is called
    
    std::vector<int> vertexBestDists; //Dijkstra's algorithm, calculate best distance from origin for each vertex
    vertexBestDists.resize(vertices.size());
    for (int i = 0; i < vertexBestDists.size(); i++)
        vertexBestDists[i] = -1;
    std::vector<int> vertexBestDirections; //Because graph is directed, this stores the smallest dist node that connects towards this node
    vertexBestDirections.resize(vertices.size());
    for (int i = 0; i < vertexBestDirections.size(); i++)
        vertexBestDirections[i] = -1;

    std::vector<int> vertexCheckStack; //Nodes to check
    vertexCheckStack.push_back(originIndex);

    while (vertexCheckStack.size() != 0) //evaluate next node on stack
    {
        //pop last vertex
        int vertex = vertexCheckStack.back();
        vertexCheckStack.pop_back();
        
        //best distance of current vertex
        int vDist = vertexBestDists[vertex];
        const std::vector<WDEdge>& vEdges = edges[vertex];

        for (int i = 0; i < vEdges.size(); i++)
        {
            int nextVert = vEdges[i].dest;
            
            int& nextBest = vertexBestDists[nextVert];


            if (nextBest < 0 || vDist+vEdges[i].distance < nextBest) //update best path if it could be shorter
            {
                nextBest = vDist+vEdges[i].distance; //update vertexBestDists
                vertexBestDirections[nextVert] = vertex;
                if (nextVert != destIndex) //don't add destination vertex to check stack
                    vertexCheckStack.push_back(nextVert);
            }
            //if potential path was longer than best, do not add vertex to check stack
        }
    }

    if (vertexBestDirections[destIndex] == -1) //origin node isn't connected to destination node
        return Path();

    //shortest path between origin and destination
    Path bestPath;
    bestPath.vertices.push_back(destIndex);
    
    while (true) //follow shortest vertexes to destination node
    {
        bestPath.vertices.push_back(vertexBestDirections[bestPath.vertices.back()]);
        if (bestPath.vertices.back() == originIndex)
            break;
    }
    //reverse the path
    for (int i = 0; i < bestPath.vertices.size()/2; i++)
    {
        int temp = bestPath.vertices[i];
        bestPath.vertices[i] = bestPath.vertices[bestPath.vertices.size()-1-i];
        bestPath.vertices[bestPath.vertices.size()-1-i] = temp;
    }
    //get edges
    for (int i = 0; i < bestPath.vertices.size()-1; i++)
    {
        int curV = bestPath.vertices[i];
        int nextV = bestPath.vertices[i+1];
        const std::vector<WDEdge>& vEdges = edges[curV];
        for (int i = 0; i < vEdges.size(); i++)
        {
            if (vEdges[i].dest == nextV)
            {
                bestPath.edges.push_back(i);
                break;
            }
        }
    }

    if (doPrint)
    {
        PrintPath(bestPath);
        std::cout << '\n';
    }

    return bestPath;
}

std::vector<Path> WDGraph::GetShortestPathsToState(int originIndex, const char* stateCode, const AirportParser& apParser, bool doPrint) const
{
    //thread_local/static would again be nice, but this is simpler
    std::vector<Path> paths;

    //go through all state associations in parser
    for (int i = 0; i < apParser.GetStCodes().size()/2; i++) //if airport at i is in input state, find shortest path to that airport
        if (stateCode[0] == apParser.GetStCodes()[(i*2)+0] && stateCode[1] == apParser.GetStCodes()[(i*2)+1])
            paths.push_back(GetShortestPath(originIndex,SearchForCodeIndex( &(apParser.GetApCodes()[i*3]) ), false));
    
    if (doPrint)
    {
        std::cout << "Shortest paths from " << std::string(vertices[originIndex].airportCode, 3) << " to " <<
            std::string(stateCode, 2) << " state airports are:\n\n";
        PrintPaths(paths);
        std::cout << '\n';
    }
    
    return paths;
}

int WDGraph::GetPathDist(const Path& path) const
{
    int dist = 0;
    for (int i = 0; i < path.edges.size(); i++)
        dist += edges[path.vertices[i]][path.edges[i]].distance;
    return dist;
}

void WDGraph::PrintPath(const Path& path) const
{
    if (path.vertices.size() == 0)
    {
        std::cout << "There is no route from " << std::string(vertices[path.vertices.front()].airportCode,3) << " to " <<
            std::string(vertices[path.vertices.back()].airportCode,3) << ".\n";
        return;
    }
    std::cout << "Shortest route from " << std::string(vertices[path.vertices.front()].airportCode,3) << " to " <<
        std::string(vertices[path.vertices.back()].airportCode,3) << ": ";
    int dist = 0;
    int cost = 0;
    for (int i = 0; i < path.edges.size(); i++)
    {
        std::cout << std::string(vertices[path.vertices[i]].airportCode, 3) << " -> ";
        dist += edges[path.vertices[i]][path.edges[i]].distance;
        cost += edges[path.vertices[i]][path.edges[i]].cost;
    }
    std::cout << std::string(vertices[path.vertices.back()].airportCode, 3) << ". The Length is " << dist <<
        ". The cost is " << cost << ".\n";
}

void WDGraph::PrintPaths(const std::vector<Path>& paths) const
{
    //get maximum vertices in path to determine size of path column
    int maxVert = 0;
    for (int i = 0; i < paths.size(); i++)
        if (paths[i].vertices.size() > maxVert)
            maxVert = paths[i].vertices.size();
    if (maxVert == 0) //message if no paths exist
    {
        std::cout << "No such paths exist\n";
        return;
    }

    int pathColLength = 5+3+((maxVert-1)*5); //how many characters wide the path column is
    int lengthColLength = 5+12; //misname, should be distance column
    
    std::cout << "Path";
    for (int i = 0; i < pathColLength-4; i++)
        std::cout << ' ';
    std::cout << "Length";
    for (int i = 0; i < lengthColLength-6; i++)
        std::cout << ' ';
    std::cout << "Cost\n";


    for (int i = 0; i < paths.size(); i++)
    {
        const Path& path = paths[i];
        if (path.vertices.size() == 0) //skip empty paths
            continue;

        int dist = 0;
        int cost = 0;
        for (int i = 0; i < path.edges.size(); i++)
        {
            std::cout << std::string(vertices[path.vertices[i]].airportCode, 3) << "->";
            dist += edges[path.vertices[i]][path.edges[i]].distance;
            cost += edges[path.vertices[i]][path.edges[i]].cost;
        }
        std::cout << std::string(vertices[path.vertices.back()].airportCode, 3);
        for (int i = 0; i < pathColLength- (3+((path.vertices.size()-1)*5) ); i++)
            std::cout << ' ';
        std::cout << dist;
        int distSize = 1;
        for (int d = dist; d > 9; d /= 10)
            distSize++;
        for (int i = 0; i < lengthColLength-distSize; i++)
            std::cout << ' ';
        std::cout << cost << '\n';
    }
}

void WDGraph::clean_visited()
{
	for (int i = 0; i < vertices.size(); i++)
		vertices[i].visited = false;
}