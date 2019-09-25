#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "database_struct.h"
#include "ColorMaster.cpp"
#include <arpa/inet.h>

#define MAX_CONNECTIONS 30
#define BUFF_SIZE 3000

using namespace std;

char* socket_path = (char*)"/tmp/DB.tuples.sock";
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);

ColorMaster mycolor = ColorMaster();  // para imprimir colores

// diccionario con PIDs y estado (conectado/desconectado del servidor).
map<int, string> ClientsMap;  // contiene { <socket>, <estado de conexión> } para cada cliente
map<pid_t, string> ClientsMapPID; // contiene { <PID>, <estado de conexión> }. Sirve para que no trate de ejecutar un comando si no está conectado primero.

int main(int argc, char** argv)
{
	if (my_socket == -1)
	{
		perror("Socket error");
		return 1;
	}
	struct sockaddr_un addr;
	*addr.sun_path = *socket_path;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);	
	
	char buffer[BUFF_SIZE];
	int read_status, send_status;
	pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
	//pthread_cond_t request_processed_by_server = PTHREAD_COND_INITIALIZER;
	
	char CMD[200] = "";
	char inputVals[4][50] = {};  // formato: { comando_cliente, argumento1_instrucc, argumento2_instrucc, clientePID }
	//char* temp_aux;
	
	while (strcmp(CMD, "quit") != 0)
	{
		printf("\nclient > ");
		pthread_mutex_lock(&client_mutex);
		scanf("%s", CMD);
		
		if (strcmp(CMD, "connect") == 0)  // conectarse al servidor
		{
			if (connect(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
			{
				perror("Connect error");
				return 1;
			}
			printf("Client PID %d connected successfully with server\n", getpid());
			
			ClientsMapPID.insert(pair<pid_t, string>(getpid(), "connected")); //ClientsMap[getpid()] = "connected";
			pthread_mutex_unlock(&client_mutex);
			continue;
		}
		
		else if ( (ClientsMapPID[getpid()] != "connected" && ClientsMapPID[getpid()] != "processing") && (strcmp(CMD, "quit") != 0) )
		{   // se ejecuta si trata de hacer algo distinto de connect y no está conectado
			printf("[!] Error, el cliente PID %d no está conectado. Conéctese para ejecutar los otros comandos.\n", (int)getpid());
			pthread_mutex_unlock(&client_mutex);
			continue;
		}

		else if (strcmp(CMD, "disconnect") == 0)    // desconectarse del servidor
		{
			unlink((const char*)(struct sockaddr*)&addr);
			printf("Client PID %d disconnected from server\n", getpid());
			strcpy(buffer, "disconnect:null:null");
			send(my_socket, buffer, sizeof(buffer), 0);
			
			ClientsMap[getpid()] = "disconnected";
			pthread_mutex_unlock(&client_mutex);
			continue;
		}
		
		else if (strcmp(CMD, "quit") == 0)  // terminar programa
		{
			unlink((const char*)(struct sockaddr*)&addr);
			//kill(getpid(), 1);
			break;  // termina proceso cliente
		}
		
		else if (strstr(CMD, "insert") != nullptr)
		{
			if (strstr(CMD, ",") == nullptr)     // un argumento: insert(value)
			{
				strcpy(inputVals[0], "insert_1");
				strcpy(inputVals[1], strtok(CMD, "("));
				strcpy(inputVals[1], strtok(NULL, ")"));  // <value>
				strcpy(inputVals[2], "null");    // no existe segundo argumento de la instrucción insert(value). Para que el servidor cache, se vuelve "null" por convención
			}
			
			else   // dos argumentos: insert(key, value)
			{
				strcpy(inputVals[0], "insert_2");
				strcpy(inputVals[1], strtok(CMD, "("));
				strcpy(inputVals[1], strtok(NULL, ","));  // <key>
				strcpy(inputVals[2], strtok(NULL, ")"));  // <value>
			}
		}
		
		else if (strstr(CMD, "get") != nullptr)   // CMD ~ get(<key>)
		{			
			strcpy(inputVals[0], "get");
			strcpy(inputVals[1], strtok(CMD, "("));
			strcpy(inputVals[1], strtok(NULL, ")"));
			strcpy(inputVals[2], "null");
		}
		
		else if (strstr(CMD, "peek") != nullptr)    // CMD ~ peek(<key>)
		{			
			strcpy(inputVals[0], "peek");
			strcpy(inputVals[1], strtok(CMD, "("));
			strcpy(inputVals[1], strtok(NULL, ")"));
			strcpy(inputVals[2], "null");
		}
		
		else if (strstr(CMD, "update") != nullptr)   // CMD ~ update(<key>, <value>)
		{
			strcpy(inputVals[0], "update");
			strcpy(inputVals[1], strtok(CMD, "("));
			strcpy(inputVals[1], strtok(NULL, ","));
			strcpy(inputVals[2], strtok(NULL, ")"));
			// [TEST]
			printf("%s(<%s>,<%s>)", inputVals[0], inputVals[1], inputVals[2]);
		}
		
		else if (strstr(CMD, "delete") != nullptr)
		{	// CMD ~ delete(<key>)
			strcpy(inputVals[0], "delete");
			strcpy(inputVals[1], strtok(CMD, "("));
			strcpy(inputVals[1], strtok(NULL, ")"));
			strcpy(inputVals[2], "null");
		}
		
		else if (strcmp(CMD, "list") == 0)
		{
			strcpy(inputVals[0], "list");
			strcpy(inputVals[1], "null");
			strcpy(inputVals[2], "null");
		}
				
		else
		{
			printf("[!] Error, comando \"%s\" no reconocido (use minísculas y sin espacios dentro de lo posible, por favor, caballero)\n", CMD);
			pthread_mutex_unlock(&client_mutex);
			continue;
		}
		
		sprintf(inputVals[3], "%d", getpid());
		
		/*--- PONIENDO LA PETICIÓN DEL CLIENTE EN EL BUFFER HACIA EL SERVIDOR ---*/
		sprintf(buffer, "%s:%s:%s:%s", inputVals[0], inputVals[1], inputVals[2], inputVals[3]);
		
		send_status = send(my_socket, buffer, sizeof(inputVals), 0);
		
		///printf("Client wrote on buffer \"%s\" with status %d\n", buffer, send_status);
		
		if (send_status == -1)
		{
			perror("Socket send error in client");
			return 1;
		}
		
		/*--- ESPERANDO RESPUESTA DEL SERVIDOR ---*/
		//mycolor.print_str("cyan", "Awaiting server answer...");  //cout << "\033[1;36mAwaiting server answer...\033[0m" << endl;
		printf("\nAwaiting server answer...\n");
		read_status = read(my_socket, buffer, sizeof(buffer));
		//read_status = listen(my_socket, MAX_CONNECTIONS);
		if (read_status == -1)
		{
			perror("Socket read error in client");
			return 1;
		}
		printf("Server answered:\n");
		mycolor.print_str("yellow", (char*)buffer);
		
		pthread_mutex_unlock(&client_mutex);
	}

	return 0;	
}