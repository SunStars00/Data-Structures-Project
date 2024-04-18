#include "WDGraph.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string inputFilePath = "../airports.csv";

void CreateGraphFromFile(WDGraph& graph)
{
    cout << "CreateGraphFromFile" << endl;

    // Origin_airport,Destination_airport,Origin_city,Destination_city,Distance,Cost
    FILE* inputFile = fopen(inputFilePath.c_str(), "r");
    int lineIndex = 0;
    while (!feof(inputFile) && inputFile != nullptr)
    {
        lineIndex++;

        char line[1024];
        fgets(line, 1024, inputFile);

        if (lineIndex == 1) // Dont parse the header line
        {
            continue;
        }

        stringstream ss(line);
        string originAirport;
        string destinationAirport;
        string originCity;
        string originState;
        string destinationCity;
        string destinationState;
        string distance;
        string cost;

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

        getline(ss, distance, ',');
        getline(ss, cost, ',');

        cout << endl;
        cout << "originAirport: " << originAirport << endl;
        cout << "destinationAirport: " << destinationAirport << endl;
        cout << "originCity: " << originCity << endl;
        cout << "originState: " << originState << endl;
        cout << "destinationCity: " << destinationCity << endl;
        cout << "destinationState: " << destinationState << endl;
        cout << "distance: " << distance << endl;
        cout << "cost: " << cost << endl;

        graph.add_vertex(originAirport.c_str());
        graph.add_vertex(destinationAirport.c_str());
        graph.add_edge(originAirport.c_str(), destinationAirport.c_str(), stoi(distance), stoi(cost));

//        cout << "add vertex: " << originAirport << endl;
//        cout << "add vertex: " << destinationAirport << endl;
//        cout << "add edge: " << originAirport << " -> " << destinationAirport << " distance: " << distance << " cost: " << cost << endl;
    }
}

int main()
{
    WDGraph graph;
    CreateGraphFromFile(graph);

    return 0;
};