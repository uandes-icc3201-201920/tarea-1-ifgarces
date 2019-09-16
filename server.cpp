#include <iostream>
#include <memory>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"
#include <string.h>
#include <random>

using namespace std;

//KVStore DB;
KVStore DB[100];
char* socket_path = (char*)"/tmp/DB.tuples.sock";
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
const int BuffSize = 200;

void Fill_data_base(KVStore* location, unsigned int itemsNum)
{
	struct StructVal newStrData;
	
	for (int index=0; index<itemsNum; index++)
	{
		newStrData.data = (unsigned char)rand()%100;
		newStrData.size = sizeof(data);
		location->insert(pair<unsigned long, struct StructVal>(index, newStrData));
	}
}

int main(int argc, char** argv)
{
	bool sflag;
	int opt;
	struct sockaddr_un addr;
	
    while ((opt = getopt(argc, argv, "s:")) != -1)
	{
		if (opt == 's')      // flag "-s"
		{
			cout << "[test] \"-s\" flag case executed" << endl;
			sflag = true;
			socket_path = argv[1];  // intentando poner path dado por usuario
			//unlink(socket_path);
			break;
		}
		else
		{
			sflag = false;
			return EXIT_FAILURE;
		}
		*addr.sun_path = *socket_path;
    }
	
	/* if (my_socket = socket(AF_UNIX, SOCK_STREAM, 0) == -1)
	{
		perror("Socket error");
		return -1;
	} */
	
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);
	
	if (bind(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)  // llamado al socket, creación
	{
		perror("Bind error");
		return -1;
	}
	
	int clientsQueueMaxLenght = 5;
	if (listen(my_socket, clientsQueueMaxLenght) == -1)  // esperando que otro programa (cliente) se conecte al socket
	{
		perror("Listen error from server");
		return -1;
	}
	
	char buffer[BuffSize];
	int new_socket, read_status;
	int iterations;
	char* decode[3];
	while (true)
	{
		cout << "Awaiting connection..." << endl;
		if ( (new_socket = accept(my_socket, (struct sockaddr*)&addr, (socklen_t*)&addr) < 0) )
		{
			perror("Connection accept error");
			exit(EXIT_FAILURE);
			return -1;
		}
		
		cout << "Connection successful with client" << endl;
		
		while ( (read_status = read(new_socket, buffer, sizeof(buffer))) > 0 )
		{
			decode[0] = strtok(buffer, ":");   // con el separador ":" el servidor reconoce qué comando y con qué parámetros el cliente lo puso en el buffer
			decode[1] = strtok(NULL, ":");
			decode[2] = strtok(NULL, ":");
			
			cout << "Read successful. Buffer: " << buffer << endl;
			
			if (strcmp(decode[0], "insert_1"))  // insert(value)
			{
				
			}
			if (strcmp(decode[0], "insert_2"))   // insert(key, value)
			{
				
			}
			if (strcmp(decode[0], "get"))
			{
				
			}
			if (strcmp(decode[0], "peek"))
			{
				
			}
			if (strcmp(decode[0], "update"))
			{
				
			}
			if (strcmp(decode[0], "delete"))
			{
				
			}
			if (strcmp(decode[0], "list"))    // list
			{
				for (int k=0; k<KVStore.size(); k++)
				{
					cout << "value: " << DB[k].data << ", size: " << DB[k].size << endl;
				}
				
				//buffer = (char*)keysToClient;
				
				if (send(my_socket, buffer, sizeof(buffer), 0) == -1)
				{
					perror("Socket send error in server");
				}
			}
		}
		
		if (read_status == -1)
		{
			perror("Socket read error in server");
			return -1;
		}
		
		else if (read_status == 0)
		{
			perror("Connection closed");
			close(new_socket);
			return -1;
		}
		iterations++;
	}
	
	
	
	/*
	//____________________//
	byte data[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
	vector<unsigned char> vdata(data, data + sizeof(data));
	Value val = { 5, vdata };
	int generated_key = rand()%9000 + 1000;    // aleatorio entre 1'000 y 10'000
	DB.insert(std::pair<unsigned long, Value>(generated_key, val));
	// Imprimir lo que hemos agregado al mapa KV.
	cout << DB[generated_key].size << " " << (int) DB[generated_key].data[0] << endl;
	*/
	return 0;
}
