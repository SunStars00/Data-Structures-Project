#ifndef WDVERTEX_H
#define WDVERTEX_H

class WDVertex
{
public:
	char state[2];
	bool visited;
	
	WDVertex()
	{
		state[0] = 'A';
		state[1] = 'A';

		visited = false;
	}
	WDVertex(char* stateChars) //stateChars should be two chars representing a state
	{
		state[0] = stateChars[0];
		state[1] = stateChars[1];
	
		visited = false;
	}



};

#endif