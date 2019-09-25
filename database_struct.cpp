// SIN USAR

# include "database_struct.h"

KeyValue::KeyValue(int k, char* v)   // constructor
{
	this->key = k;
	this->value = v;
}

KeyValue::KeyValue(int k)   // constructor con value aleatorio
{
	this->key = k;
	const char letters[27] = "abcdefghijklmnopqrstuvwxyz";
	for (int u=0; u<5; u++)
	{
		this->value[u] = letters[ rand()%24 ];
	}
}

KeyValue::KeyValue() {}