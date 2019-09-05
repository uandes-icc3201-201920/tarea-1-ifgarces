#include <iostream>
#include <unistd.h>

bool clientConnected = false;     // [shared] se hace verdadero por el cliente cuando se conecta al servidor

int main()
{
	string output = "";
	unsigned int sleepyTime = 10000;     // en milisegundos
	usleep(sleepyTime);
	
	if (clientConnected)    // conectado a tiempo
	{
		output << "OK"
	}
	
	else     // no se conectó a tiempo
	{
		output << "[!] Error, tiempo de conexión a cliente ha expirado";
		return 1;
	}
	
	return 0;
}