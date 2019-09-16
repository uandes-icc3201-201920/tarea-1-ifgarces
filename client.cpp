#include <iostream>
#include <memory>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "util.h"
#include <pthread.h>

using namespace std;

char* socket_path = (char*)"/tmp/DB.tuples.sock";
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
const int BuffSize = 200;

int main(int argc, char** argv)
{
	if (my_socket == -1)
	{
		perror("Socket error");
		return -1;
	}
	struct sockaddr_un addr;
	*addr.sun_path = *socket_path;
	//int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);	
	
	char buffer[BuffSize];
	int new_socket, read_status;
	pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	char* CMD;
	char* aux = (char*)"";
	char* clientCMDid = (char*)"0";    // clientCMDid sirve para que server sepa qué operación debe hacer con los datos del buffer, con el o los números.
	char* inputVals[3];  // contiene los parámetros de la función que se quiere, como el "key" y "value" del comando "insert(key, value)"
	string CMDstr;
	
	while (strcmp(CMD, "quit") != 0)
	{
		cout << "clinet > ";
		scanf("%s", CMD);
		CMDstr = CMD;
		
		if (strcmp(CMD, "connect") == 0)  // conectarse al servidor
		{
			if (connect(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
			{
				perror("Connect error");
				return -1;
			}
			cout << "Connection successful with server" << endl;
		}

		else if (strcmp(CMD, "disconnect") == 0)    // desconectarse del servidor
		{
			unlink((const char*)(struct sockaddr*)&addr);
			cout << "Disconnected from server" << endl;
		}
		
		else if (strcmp(CMD, "quit") == 0)    // cerrar programilla
		{
			unlink((const char*)(struct sockaddr*)&addr);
			return 0;
		}
		
		else if (CMDstr.find("insert") != string::npos)   // insertar cosa
		{
			pthread_mutex_lock(&client_mutex);
			if (CMDstr.find(",") == string::npos)     // un argumento insert(value)
			{
				clientCMDid = (char*)"insert_1";
				aux = (char*)strtok(CMD, "(");
				inputVals[0] = (char*)strtok(NULL, ")");  // "value" que quiere insertar
				inputVals[1] = NULL;
				inputVals[2] = NULL;
				sprintf(buffer, "%s:%s:%s:%s", clientCMDid, inputVals[0], inputVals[1], inputVals[2]);
				
				cout << "Client wrote on buffer: " << buffer << endl;
				
				if (send(my_socket, buffer, sizeof(inputVals[0]), 0) == -1)
				{
					perror("Socket send error from client");
					return -1;
				}
			}
			
			else   // dos argumentos insert(key, value)
			{
				clientCMDid = (char*)"insert_2";
				aux = (char*)strtok(CMD, "(");
				inputVals[0] = strtok(NULL, ",");
				inputVals[1] = strtok(NULL, ",");
				sprintf(buffer, "%s:%s:%s", clientCMDid, inputVals[0], inputVals[1]);
				
				if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
				{
					perror("Socket send error from client");
					return -1;
				}
			}
			
			// enviando a servidor
			if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
			{
				perror("Socket send error in client");
				return -1;
			}
			pthread_mutex_unlock(&client_mutex);
		}
		
		else if (CMDstr.find("get") != string::npos)
		{
			clientCMDid = (char*)"get";
			
			
		}
		
		else if (CMDstr.find("peek") != string::npos)
		{
			clientCMDid = (char*)"peek";
			
			
		}
		
		else if (CMDstr.find("update") != string::npos)
		{
			clientCMDid = (char*)"update";
			
			
		}
		
		else if (CMDstr.find("delete") != string::npos)
		{
			clientCMDid = (char*)"delete";
			
			//inputVals = {atoi(strtok(NULL, ",")), atoi(strtok(NULL, ")")), clientCMDid};
			inputVals[0] = strtok(NULL, ",");
			inputVals[1] = strtok(NULL, ")");
			inputVals[2] = clientCMDid;
			//buffer = (char*)inputVals;
			sprintf(buffer, "%s", inputVals[0]);
			strcat(buffer, ":");
			sprintf(buffer, "%s", inputVals[1]);
			strcat(buffer, ":");
			sprintf(buffer, "%s", inputVals[2]);
			strcat(buffer, ":");
			if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
			{
				perror("Socket send error from client");
				return -1;
			}
		}
		
		else if (CMDstr == "list")
		{
			clientCMDid = (char*)"list";
			
			//inputVals = {0, 0, clientCMDid};
			inputVals[0] = 0;
			inputVals[1] = 0;
			inputVals[2] = clientCMDid;
			//buffer = (char*)inputVals;
			sprintf(buffer, "%s", inputVals[0]);
			strcat(buffer, ":");
			sprintf(buffer, "%s", inputVals[1]);
			strcat(buffer, ":");
			sprintf(buffer, "%s", inputVals[2]);
			strcat(buffer, ":");
			
			if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
			{
				perror("Socket send error in client");
				return -1;
			}
		}
		else
		{
			cout << "[!] Error, unknown command \"" << CMD << "\""<< endl;
		}
	}

	return 0;	
}