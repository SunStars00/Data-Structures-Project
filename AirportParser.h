#ifndef AIRPORTPARSER_H
#define AIRPORTPARSER_H

#include <vector>
#include <string>

class AirportParser
{
private:
	std::vector<char> apCodes; //3*number of elements, each element is unique
	std::vector<char> stCodes; //2*number of elements
public:
	const std::vector<char>& GetApCodes() const { return apCodes; }
	const std::vector<char>& GetStCodes() const { return stCodes; }

	void AddAirportAndState(const char* apCode, const char* stCode); //checks for already existing airport

	char* GetAirport(int index) { return &(apCodes[index*3]); } //liable to invalidation if vector is inserted to
	char* GetState(int index) { return &(stCodes[index*2]); }

	int FindAirport(const char* apCode); //ptr to 3 chars
};

#endif