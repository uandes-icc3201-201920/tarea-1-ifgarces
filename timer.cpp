#include <iostream>
#include <unistd.h>

bool clientConnected = false;     // se hace verdadero por el cliente cuando se conecta al servidor
bool out_of_time = false

int main()
{
	string output = "";
	unsigned int sleepyTime = 10000;   // en milisegundos
	usleep(sleepyTime);
	
	if (! clientConnected)     // no se conectó a tiempo
	{
		out_of_time = true;
		//output << "[!] Error, tiempo de conexión a cliente ha expirado";
	}
	
	return 0;
}