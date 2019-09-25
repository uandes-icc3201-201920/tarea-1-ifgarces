#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "ColorMaster.cpp"
#include "database_struct.h"

using namespace std;

typedef pair<int, char*> PAIR;

void print_it(const char* message)
{
	cout << message << endl;
}

int main(int argc, char** argv)
{
	char in[30] = "insert(key, value)";
	char coding[3][30];
	
	strcpy(coding[0], "insert");
	strcpy(coding[1], strtok(in, "("));
	strcpy(coding[1], strtok(NULL, ","));
	strcpy(coding[2], strtok(NULL, ")"));
	
	for (int i=0; i<3; i++)
	{
		cout << coding[i] << endl;
	}
	return 0;
}