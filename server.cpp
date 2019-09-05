#include <iostream>
#include <memory>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"
#include <string.h>

using namespace std;

KVStore db;
char* socket_path = "\0hidden";
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);

int main(int argc, char** argv)
{
	bool sflag = false;
	int opt;
	
	/*
	// Procesar opciones de linea de comando
    while ((opt = getopt(argc, argv, "s:")) != -1)
	{
        switch (opt)
		{
			// Procesar el flag s si el usuario lo ingresa
			case 's':
				cout << "[test] s case executed" << endl;
				sflag = true;
				*socket_path = *argv[1];    // intentando poner path dado por usuario
				unlink(socket_path);
				break;
			default:
				return EXIT_FAILURE;
        }	    	
    }	
	*/
	struct sockaddr_un addr;
	
	if (my_socket = socket(AF_UNIX, SOCK_STREAM, 0) == -1)
	{
		perror("Socket error");
		return -1;
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);

	
	if (bind(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)  // llamado al socket, creación
	{
		perror("[!] Error, no se pudo crear el socket para comunicar al cliente con el servidor.");
		return -1;
	}
	//cout << "[test] Socket created." << endl;
	
	int clientsQueueMaxLenght = 5;
	if (listen(my_socket, clientsQueueMaxLenght) == -1)  // esperando que otro programa (cliente) se conecte al socket
	{
		perror("[!] Error, el servidor no logró esperar una conexión de cliente.");
		return -1;
	}
	//cout << "[test] Listened." << endl;
	
	char buffer[200];    // tamaño máximo de mensajes servidor-cliente es de 200 caracteres char
	int new_socket, read_status;
	int iterations;
	int decode[3];
	while (true)
	{
		//cout << iterations << endl;
		if ( (new_socket = accept(my_socket, (struct sockaddr*)&addr, (socklen_t*)&addr) < 0) )
		{
			perror("[!] Error, servidor no pudo aceptar la conexión del cliente.");
			exit(EXIT_FAILURE);
			return -1;
		}
		
		while ( (read_status = read(new_socket, buffer, sizeof(buffer))) > 0 )
		{
			//cout << "Read by client successful. Current buffer: " << buffer << endl;
			decode = (int*)buffer;
			
			if (decode[2] == 0)
			{
				
			}
			if (decode[2] == 1)
			{
				
			}
			if (decode[2] == 2)
			{
				
			}
			if (decode[2] == 3)
			{
				
			}
			if (decode[2] == 4)
			{
				
			}
			if (decode[2] == 5)
			{
				
			}
			if (decode[2] == 6)
			{
				unsigned long keysToClient[sb.lenght];
				for (int k=0; k<db.lenght; k++)
				{   // no alcanzo a averiguar bien cómo se imprime lo que hay en bd. No sé que es un map. La cosa es que le manda al cliente todas las claves aquí.
					keysToClient[k] = db.keys[k];
				}
				
				buffer = (char*)keysToClient;
				
				if (send(my_socket, buffer, sizeof(buffer), 0) == -1)
				{
					perror("[!] Error del servidor al enviar respuesta al cliente por el socket.")
				}
			}
		}
		
		if (read_status == -1)
		{
			perror("[!] Error, el servidor no pudo leer exitosamente lo que había en el socket.");
			return -1;
		}
		
		else if (read_status == 0)
		{
			perror("Connection closed.");
			close(new_socket);
			//return -1;
		}
		iterations++;
	}
	
	
	
	
	// Uso elemental del almacenamiento KV:
	
	// Creamos un arreglo de bytes a mano
	byte data[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

	// Luego un vector utilizando el arreglo de bytes
	vector<byte> vdata(data, data + sizeof(data));
	
	// Creamos el valor
	Value val = { 5, vdata };
	
	// Insertamos un par clave, valor directamente
	// en el mapa KV
	
	// Nota: Debiera diseñarse una solución más robusta con una interfaz
	// adecuada para acceder a la estructura.
	int generated_key = rand()%9000 + 1000;    // aleatorio entre 1'000 y 10'000
	db.insert(std::pair<unsigned long, Value>(generated_key, val));
	
	// Imprimir lo que hemos agregado al mapa KV.
	cout << db[generated_key].size << " " << (int) db[generated_key].data[0] << endl;
	
	return 0;
}
