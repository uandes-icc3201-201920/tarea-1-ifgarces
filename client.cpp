#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "database_struct.cpp"    // el "util.h"
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CONNECTIONS 10
#define BUFFSIZE 200

using namespace std;

char* socket_path = (char*)"/tmp/DB.tuples.sock";
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);

void PleaseCleanThisString(char* s)
{
	for (unsigned int i=0; i<strlen(s); i++)
	{
		s[i] = '\0';
	}
}

int main(int argc, char** argv)
{
	if (my_socket == -1)
	{
		perror("Socket error");
		return 1;
	}
	struct sockaddr_un addr;
	*addr.sun_path = *socket_path;
	//int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);	
	
	char buffer[BUFFSIZE];
	int new_socket, read_status;
	int send_status;
	pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
	
	char CMD[100] = "";
	char inputVals[3][50] = {};  // formato: {comando_cliente, argumento1_instrucc, argumento2_instrucc}
	
	while (strcmp(CMD, "quit") != 0)
	{
		cout << "client > ";
		//pthread_mutex_lock(&client_mutex);
		scanf("%s", CMD);
		
		if (strcmp(CMD, "connect") == 0)  // conectarse al servidor
		{
			if (connect(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
			{
				perror("Connect error");
				return 1;
			}
			cout << "Connection successful with server" << endl;
		}

		else if (strcmp(CMD, "disconnect") == 0)    // desconectarse del servidor
		{
			unlink((const char*)(struct sockaddr*)&addr);
			cout << "Disconnected from server" << endl;
		}
		
		else if (strcmp(CMD, "quit") == 0)  // terminar programa
		{
			unlink((const char*)(struct sockaddr*)&addr);
			return 0;
		}
		
		else if (strstr(CMD, "insert") != nullptr)
		{
			if (strstr(CMD, ",") == nullptr)     // un argumento: insert(value)
			{
				strcpy(inputVals[0], "insert_1");
				strcpy(inputVals[1], strtok(CMD, "("));
				strcpy(inputVals[1], strtok(NULL, ")"));  // <value>
				strcpy(inputVals[2], "");
			}
			
			else   // dos argumentos: insert(key, value)
			{
				strcpy(inputVals[0], "insert_2");
				strcpy(inputVals[1], strtok(CMD, "("));
				strcpy(inputVals[1], strtok(NULL, ","));  // <key>
				strcpy(inputVals[2], strtok(NULL, ")"));  // <value>
			}
		}
		
		else if (strstr(CMD, "get") != nullptr)
		{			
			strcpy(inputVals[0], "get");
			
		}
		
		else if (strstr(CMD, "peek") != nullptr)
		{			
			strcpy(inputVals[0], "peek");
			
		}
		
		else if (strstr(CMD, "update") != nullptr)
		{			
			strcpy(inputVals[0], "update");
			
		}
		
		else if (strstr(CMD, "delete") != nullptr)
		{			
			strcpy(inputVals[0], "delete");
			strcpy(inputVals[1], strtok(CMD, ","));
			strcpy(inputVals[2], strtok(NULL, ")"));
		}
		
		else if (strcmp(CMD, "list") == 0)
		{
			strcpy(inputVals[0], "list");
			strcpy(inputVals[1], "");
			strcpy(inputVals[2], "");
			
			
		}
				
		else
		{
			cout << "[!] Error, comando \"" << CMD << "\"" << " no reconocido (use minísculas)." << endl;
		}
		
		/*--- PONIENDO LA PETICIÓN DEL CLIENTE EN EL BUFFER HACIA EL SERVIDOR ---*/
		sprintf(buffer, "%s:%s:%s", inputVals[0], inputVals[1], inputVals[2]);
		// formato buffer: <comando_cliente>:<argumento1_instrucc>:<argumento2_instrucc>
		
		send_status = send(my_socket, buffer, sizeof(inputVals), 0);
		
		cout << "  (i) Client wrote on buffer \"" << buffer << "\" with status " << send_status << "\n" << endl;
			
		if (send_status == -1)
		{
			perror("Socket send error in client");
			return 1;
		}
		
		
		/*--- ESPERANDO RESPUESTA DEL SERVIDOR ---*/
		cout << "\033[1;36mAwaiting server answer...\033[0m" << endl;
		read_status = read(my_socket, buffer, BUFFSIZE);
		//read_status = listen(my_socket, MAX_CONNECTIONS);
		if (read_status == -1)
		{
			perror("Socket read error in client");
			return 1;
		}
		cout << "  (i) Client read server answer \"" << buffer << "\"\n" << endl;
		
		
		//pthread_mutex_unlock(&client_mutex);
	}

	return 0;	
}