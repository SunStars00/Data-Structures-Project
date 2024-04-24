#include <algorithm>
#include <tuple>
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

bool SortTupleBySecondItem(tuple<int, int>& a, tuple<int, int>& b)
{
    return (get<1>(a) > get<1>(b));
}
void WDGraph::PrintAirportConnections() const
{
    // Save connections in a vector of tuples, index 0 of the tuple is the vertex index, index 1 is the connection count
    // Tuple is used to keep the vertex index and the connection count together
    vector<tuple<int, int>> connections(vertices.size(), make_tuple(0, 0));

    cout << "Airport          Connections" << endl;

    for (int i = 0; i < vertices.size(); i++)
    {
        int connectionCount = 0;

        // Inbound connections
        for (int j = 0; j < vertices.size(); j++)
        {
            for (int k = 0; k < edges[j].size(); k++)
            {
                if (edges[j][k].dest == i)
                {
                    connectionCount++;
                }
            }
        }

        // Outbound connections
        connectionCount += edges[i].size();

        connections[i] = make_tuple(i, connectionCount);
    }

    // Sort the connection count in descending order
    sort(connections.begin(), connections.end(), SortTupleBySecondItem);

    // Print the connection list
    for (int i = 0; i < connections.size(); i++)
    {
        cout << vertices[get<0>(connections[i])].airportCode << "              " << get<1>(connections[i]) << endl;
    }
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
    //if an invalid code was found, a reference can't be returned
    throw std::string("Error in SearchForCode: could not find code");
}

//Dijkstra's
Path WDGraph::GetShortestPath(int originIndex, int destIndex, bool doPrint) const
{
    if (originIndex == destIndex) //return empty path if origin and dest are the same
        return Path();

    //should really use thread_local global vars here so a memory isn't allocated everytime the function is called

    std::vector<int> vertexBestDists; //Dijkstra's algorithm, calculate best distance from origin for each vertex
    vertexBestDists.resize(vertices.size());
    for (int i = 0; i < vertexBestDists.size(); i++)
        vertexBestDists[i] = -1;
    std::vector<int> vertexBestDirections; //Because graph is directed, this stores the smallest dist vertex that connects towards this node
    vertexBestDirections.resize(vertices.size());
    for (int i = 0; i < vertexBestDirections.size(); i++)
        vertexBestDirections[i] = -1;

    struct VertexDistPair
    {
        int vertex = 0; int bestDist = 0;
        VertexDistPair() {}
        VertexDistPair(int vertexInt, int bestDistInt) { vertex = vertexInt; bestDist = bestDistInt; }
        bool operator<(const VertexDistPair& v) { return bestDist < v.bestDist; }
    };
    MinHeap<VertexDistPair> vertexCheckQueue; //Vertices to check
    vertexCheckQueue.AddItem(VertexDistPair(originIndex,0));

    while (vertexCheckQueue.data.size() != 0) //evaluate next vertex on stack
    {
        //pop last vertex
        int vertex = vertexCheckQueue.PopMin().vertex;

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
                    vertexCheckQueue.AddItem(VertexDistPair(nextVert,nextBest));
            }
            //if potential path was longer than best, do not add vertex to check stack
        }
    }

    Path bestPath = Path();

    //origin node isn't connected to destination node
    if (vertexBestDirections[destIndex] == -1)
    {
        goto printPath;
    }

    //shortest path between origin and destination
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

    label: printPath:
    if (doPrint)
    {
        if (bestPath.vertices.empty())
            std::cout << "Shortest route from " << std::string(vertices[originIndex].airportCode,3) << " to " <<
                      std::string(vertices[destIndex].airportCode,3) << ": None\n";
        else
            PrintPath(bestPath);

        std::cout << '\n';
    }

    return bestPath;
}

std::vector<Path> WDGraph::GetShortestPathsToState(int originIndex, const char* stateCode, const AirportStatePair& apParser, bool doPrint) const
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

//Not Dijkstra's. I had an idea for doing this with Dijkstra's, but it would only be simple if "path" implied no repeat vertices
//post-comment, apparently we can assume no repeats in a path. Very sad, the algorithm would've been super epic, have unfinished version saved somewhere else
Path WDGraph::GetShortestPathFixedStops(int originIndex, int destIndex, int stopCount, bool doPrint) const
{
    //iterating through every possible path of length minStopCount+2, starting at origin
    Path bestPath; //current best (valid) path to dest
    std::vector<float> bestDist; //track distance at each point of bestPath to potentially join path to it

    Path path;
    std::vector<int>& pVertices = path.vertices;
    std::vector<int>& pEdges = path.edges;
    std::vector<float> dist;

    //start path at origin and first edge (loop immediatly exits if no edges exist)
    pVertices.push_back(originIndex);
    pEdges.push_back(0);
    dist.push_back(0);

    while (pVertices.size() > 0)
    {
        if (pEdges.back() < edges[pVertices.back()].size())
        {
            const WDEdge& edge = edges[pVertices.back()][pEdges.back()];

            //remove this code to allow repeat vertices in path
            for (int i = 0; i < path.vertices.size(); i++)
                if (path.vertices[i] == edge.dest)
                {
                    pEdges.back()++;
                    continue;
                }

            //check if current path can be joined with best path to make a better path (optimization)
            if (bestPath.vertices.size() > 0)
            {
                int pathLength = pVertices.size();
                // if edge.dest would lead to the same vertex that is at bestPath and if it would reduce total distance
                if (bestPath.vertices[pathLength] == edge.dest && dist.back()+edge.distance < bestDist[pathLength])
                {
                    //prepare bestDists by replacing them with distance to node
                    for (int i = bestDist.size()-1; i > 0; i--)
                        bestDist[i] = bestDist[i]-bestDist[i-1];
                    //copy current path into start of bestPath
                    for (int i = 0; i < pathLength; i++)
                    {
                        bestPath.vertices[i] = path.vertices[i];
                        bestPath.edges[i] = path.edges[i];
                        bestDist[i] = dist[i];
                    }
                    //update bestDists for rest of path, note that bestDist[i] is change in dist while bestDist[i-1] is total dist
                    for (int i = pathLength; i < bestDist.size(); i++)
                        bestDist[i] = bestDist[i-1] + bestDist[i];
                    pEdges.back()++;
                    continue;
                }
            }

            if (pVertices.size() < stopCount+1) //add vertex if it won't be the final vertex
            {
                pVertices.push_back(edge.dest);
                pEdges.push_back(0);
                dist.push_back(dist.back()+edge.distance);
            }
            else //added vertex would be the final vertex in path
            {
                if (bestPath.vertices.size() == 0 && edge.dest == destIndex) //if final vertex is intended destination and best path doesn't exist yet, this is the first best path
                {
                    //copy path to bestPath, and finalize bestPath
                    bestPath = path;
                    bestDist = dist;
                    bestPath.vertices.push_back(destIndex);
                    bestDist.push_back(dist.back()+edge.distance);
                }
                pEdges.back()++;
            }
            continue;
        }
        //current edge to check is out of range, go back a vertex
        dist.pop_back();
        pVertices.pop_back();
        pEdges.pop_back();

        //check next edge on next iteration
        if (pEdges.size() > 0)
            pEdges.back()++;
    }
    //iterated through every path

    if (doPrint)
    {
        if (bestPath.vertices.size() == 0)
            std::cout << "Shortest route from " << std::string(vertices[originIndex].airportCode,3) << " to " <<
                      std::string(vertices[destIndex].airportCode,3) << " with " << stopCount << " stops: None\n";
        else
            PrintPathStops(bestPath, stopCount);

        std::cout << '\n';
    }

    return bestPath;
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
    //empty condition
    if (path.vertices.size() == 0)
    {
        std::cout << "Empty Path\n";
        return;
    }

    std::cout << "Shortest route from " << std::string(vertices[path.vertices.front()].airportCode,3) << " to " <<
              std::string(vertices[path.vertices.back()].airportCode,3) << ": ";
    int dist = 0;
    int cost = 0;
    //print out path
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

void WDGraph::PrintPathStops(const Path& path, int stops) const
{
    //empty condition
    if (path.vertices.size() == 0)
    {
        std::cout << "Empty Path\n";
        return;
    }
    std::cout << "Shortest route from " << std::string(vertices[path.vertices.front()].airportCode,3) << " to " <<
              std::string(vertices[path.vertices.back()].airportCode,3) << " with " << stops << " stops: ";
    int dist = 0;
    int cost = 0;
    //print paths
    for (int i = 0; i < path.edges.size(); i++)
    {
        std::cout << std::string(vertices[path.vertices[i]].airportCode, 3) << " -> ";
        dist += edges[path.vertices[i]][path.edges[i]].distance;
        cost += edges[path.vertices[i]][path.edges[i]].cost;
    }
    std::cout << std::string(vertices[path.vertices.back()].airportCode, 3) << ". The Length is " << dist <<
              ". The cost is " << cost << ".\n";
}

void WDGraph::clean_visited()
{
    for (int i = 0; i < vertices.size(); i++)
        vertices[i].visited = false;
}
