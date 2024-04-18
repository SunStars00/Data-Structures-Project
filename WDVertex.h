#ifndef WDVERTEX_H
#define WDVERTEX_H

#include <string>

class WDVertex
{
public:
	char airportCode[3];
	bool visited;
	
	WDVertex()
	{
		airportCode[0] = 'A';
		airportCode[1] = 'A';
		airportCode[2] = 'A';

		visited = false;
	}
	WDVertex(char* airportCodeChars) //stateChars should be two chars representing a state
	{
		airportCode[0] = airportCodeChars[0];
		airportCode[1] = airportCodeChars[1];
		airportCode[2] = airportCodeChars[2];
	
		visited = false;
	}



};

#endif