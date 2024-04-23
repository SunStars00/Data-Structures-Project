#include "AirportStatePair.h"

void AirportStatePair::AddAirportAndState(const char* apCode, const char* stCode)
{
    if (FindAirport(apCode) != -1)
        return;
    apCodes.push_back(apCode[0]);
    apCodes.push_back(apCode[1]);
    apCodes.push_back(apCode[2]);

    stCodes.push_back(stCode[0]);
    stCodes.push_back(stCode[1]);
}

int AirportStatePair::FindAirport(const char* apCode)
{
    for (int i = 0; i < apCodes.size()/3; i++)
    {
        bool check = true;
        for (int j = 0; j < 3; j++)
        {
            if (apCode[j] != apCodes[(i*3)+j])
            {
                check = false;
                break;
            }
        }
        if (check == true)
            return i;
    }
    return -1;
}