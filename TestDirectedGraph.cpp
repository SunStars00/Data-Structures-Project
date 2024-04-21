#include "WDGraph.h"
#include "WDVertex.h"
#include "AirportParser.h"
#include "WUDGraph.h"

#include <iostream>
#include <sstream>

using namespace std;

string inputFilePath = "../airports.csv";
//string inputFilePath = "airports.csv";

void CreateGraphFromFile(WDGraph& graph, AirportParser& ap)
{
    FILE* inputFile = fopen(inputFilePath.c_str(), "r");
    int lineIndex = 0;
    while (!feof(inputFile) && inputFile != nullptr)
    {
        lineIndex++;

        char line[1024];
        fgets(line, 1024, inputFile);

        // Don't parse the header line
        if (lineIndex == 1) continue;

        stringstream ss(line);
        string originAirport;
        string destinationAirport;
        string originCity;
        string originState;
        string destinationCity;
        string destinationState;
        string distanceString;
        string costString;

        getline(ss, originAirport, ',');
        getline(ss, destinationAirport, ',');

        getline(ss, originCity, ',');
        originCity = originCity.substr(1, originCity.size() - 1); // Removes extra quotes and spaces
        getline(ss, originState, ',');
        originState = originState.substr(1, originState.size() - 2); // Removes extra quotes and spaces

        getline(ss, destinationCity, ',');
        destinationCity = destinationCity.substr(1, destinationCity.size() - 1); // Removes extra quotes and spaces
        getline(ss, destinationState, ',');
        destinationState = destinationState.substr(1, destinationState.size() - 2); // Removes extra quotes and spaces

        getline(ss, distanceString, ',');
        getline(ss, costString, ',');

//        cout << endl;
//        cout << "originAirport: " << originAirport << endl;
//        cout << "destinationAirport: " << destinationAirport << endl;
//        cout << "originCity: " << originCity << endl;
//        cout << "originState: " << originState << endl;
//        cout << "destinationCity: " << destinationCity << endl;
//        cout << "destinationState: " << destinationState << endl;
//        cout << "distance: " << distanceString << endl;
//        cout << "cost: " << costString << endl;

        WDVertex originVertex = WDVertex(originAirport.c_str());
        WDVertex destinationVertex = WDVertex(destinationAirport.c_str());

        graph.add_vertex(originVertex);
        graph.add_vertex(destinationVertex);
        graph.add_edge(originVertex, destinationVertex, stoi(distanceString), stoi(costString));

        ap.AddAirportAndState(originAirport.c_str(), new char [2] {originState[0], originState[1]});
        ap.AddAirportAndState(destinationAirport.c_str(), new char [2] {destinationState[0], destinationState[1]});
    }
}

int main()
{
    WDGraph graph;
    AirportParser ap;
    CreateGraphFromFile(graph, ap);

//    Path p = graph.GetShortestPath(graph.SearchForCodeIndex("ATL"), graph.SearchForCodeIndex("SLC"));
//    Path p1 = graph.GetShortestPath(graph.SearchForCodeIndex("IAD"), graph.SearchForCodeIndex("MIA"));

//    vector<Path> p3 = graph.GetShortestPathsToState(graph.SearchForCodeIndex("ATL"), "FL", ap);
//    vector<Path> p4 = graph.GetShortestPathsToState(graph.SearchForCodeIndex("ATL"), "NY", ap);
//    vector<Path> p5 = graph.GetShortestPathsToState(graph.SearchForCodeIndex("ATL"), "TX", ap);
//    vector<Path> p6 = graph.GetShortestPathsToState(graph.SearchForCodeIndex("MCO"), "TX", ap);

    int a1 = graph.SearchForCodeIndex("ATL");
    int a2 = graph.SearchForCodeIndex("ORD");
    int a3 = graph.SearchForCodeIndex("IAD");
    int a4 = graph.SearchForCodeIndex("MIA");

    //     main tree weight = 24961

    WUDGraph wudGraph(&graph);
    bool connected = wudGraph.IsGraphConnected();
    WDGraph primMST = wudGraph.PrimMST();
    WDGraph kruskalMST = wudGraph.KruskalMST();

    cout << "The graph is " << ((connected) ? "" : "not ") << "connected" << endl;

    cout << endl;

    wudGraph.PrintMST(primMST, "Minimum Spanning Tree (Prim's Algorithm)");
    cout << endl;
    wudGraph.PrintMST(kruskalMST, "Minimum Spanning Tree (Kruskal's Algorithm)");

    return 0;
};