#ifndef __DB__
#define __DB__

#include <map>
#include <vector>
#include <random>
#include <string.h>

using namespace std;

/*
typedef struct KeyValue
{
	size_t size;
	vector<unsigned char> data;
} KeyValue;
typedef map<unsigned long, KeyValue> KVStore;

void Fill_data_base(KVStore* database, unsigned int itemsNum)
{
	struct StructVal newStrData;
	char letters[26] = "abcdefghijklmnopqrstuvwxyz";
	char randWord[5];
	
	for (int index=0; index<itemsNum; index++)
	{
		randWord = "";
		for (int k=0; k<5; k++)
		{
			strcat(randWord, (char*)letters[rand()%25]);
		}
		//newStrData.data = (char)rand()%100;
		newStrData.data = randWord;
		newStrData.size = sizeof(newStrData.data);
		database->insert(KVStore(index, newStrData));
	}
}

char* Show_map_values(KVStore* database)    // https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
{
	KVStore::iterator ITR
	cout << ""
	for (ITR = KVStore->begin(); ITR != KVStore->end(); ++ITR)
	{
		cout << ITR->first << "\t" << ITR->second << "\n";
	}
	cout << endl;
}
*/

typedef class KeyValue
{
	public:
		int key;
		char* value;
		KeyValue(int, char*);
		KeyValue(int);
		KeyValue();
} KVStore;

int Search_key(KVStore db[], unsigned int db_size, int key_searched)
{   // retorna índice del elemento en DB, a menos que no exista, en cuyo caso retorna -1
	for (unsigned int n=0; n<db_size; n++)
	{
		if (db[n].key == key_searched) { return n; }
	}
	return -1;
}

void Print_codifications(char lines[][50], int linesNum)
{
	for (int w=0; w<linesNum; w++)
	{
		cout << "\"" << lines[w] << "\"" << endl;
	}
}

#endif