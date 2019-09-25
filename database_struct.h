#ifndef este_es_el_util_punto_h
#define este_es_el_util_punto_h

#include <map>
#include <vector>
#include <random>
#include <string.h>
#include <iterator>

using namespace std;

/* struct KeyValue
{
	size_t size;
	char* data;
}; */
typedef map<int, char*> KVStore;
typedef pair<int, char*> KVpair;  // para los insert del map

void KV_fill(KVStore* database)
{
	database->insert(KVpair(0, (char*)"hola"));
	database->insert(KVpair(1, (char*)"compañero"));
	database->insert(KVpair(4, (char*)"safdgh"));
}

int KV_append(KVStore* database, char* new_value)    // agrega un elemento al final, en el primer key que tenga value NULL, si es que hay.
{                                                    // retorna key donde se insertó el elemento
	KVStore::iterator helper;
	for (helper = database->begin(); helper != database->end(); ++helper)
	{
		if (helper->second == NULL)
		{
			database->insert( KVpair(helper->first, new_value) );
			return helper->first;
		}
	}
	// llega hasta aquí si es que todas las llaves que hay tienen un value no nulo, válido
	
	database->insert( KVpair(database->size(), new_value) );
	return database->size()-1;
}

char* KV_get_printable(KVStore* database, int max_str_size)
{    // una especie de to_string
	char* output = (char*)malloc(sizeof(char)*max_str_size);
	char aux[100] = "";
	KVStore::iterator helper;
	strcpy(output, "KEY\tVALUE\n");
	for (helper = database->begin(); helper != database->end(); ++helper)
	{
		sprintf(aux, "%d\t%s\n", helper->first, helper->second);
		strcat(output, aux);
	}
	return output;
}

bool KV_searchKey(KVStore* database, int wanted_key)  // retorna true si existe
{
	KVStore::iterator helper;
	for (helper = database->begin(); helper != database->end(); ++helper)
	{
		if (helper->first == wanted_key)
		{
			return (helper->second != NULL);
		}
	}
	return (false);  // retorna false si la llave está asociada a un NULL o si no existe la llave
}


/*
	Referencias:
	- https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
*/

/* void Print_codifications(char lines[][50], int linesNum)
{
	for (int w=0; w<linesNum; w++)
	{
		cout << "\"" << lines[w] << "\"" << endl;
	}
} */
#endif