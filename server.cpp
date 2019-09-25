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

#define MAX_CONNECTIONS 30
#define BUFF_SIZE 3000
#define DATABASE_SIZE 100000

using namespace std;

KVStore DB;   // map<int, char*>

char socket_path[200];
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
ColorMaster mycolor = ColorMaster();  // para imprimir colores

map<pid_t, string> ClientsMap;

int main(int argc, char** argv)
{
	strcpy(socket_path, "/tmp/DB.tuples.sock");
	KV_fill(&DB);  // [TEST] lo relleno para cachar. En verdad debe partir vacío.
	
	int opt;
	struct sockaddr_un addr;

    while ((opt = getopt(argc, argv, "s:")) != -1)
	{
		if (opt == 's')      // flag "-s"
		{
			cout << "[test] \"-s\" flag case executed" << endl;
			strcpy(socket_path, argv[1]);  // intentando poner path dado por usuario
			//unlink(socket_path);
			break;
		}
		else { return EXIT_FAILURE; }
		*addr.sun_path = *socket_path;
    }
	
	if (my_socket == -1)
	{
		perror("Socket error");
		return 1;
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);
	
	if (bind(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)  // llamado al socket, creación
	{
		perror("Bind error");
		return 1;
	}
	
	if (listen(my_socket, MAX_CONNECTIONS) == -1)
	{
		perror("Listen error from server");
		printf("Demasiados clientes conectados. Número máximo: %d", MAX_CONNECTIONS);
		return 1;
	}
	

	
	/*--- COMIENZA CICLO PARA PROCESAR INSTRUCCIONES DE LOS CLIENTES ---*/
	char buffer[BUFF_SIZE];
	int new_socket, read_status, send_status;
	int j;
	char decode[4][150] = {}; // forma: { tipo_instrucción, arg1, arg2 }
	char server_answer[BUFF_SIZE] = "";
	char* aux = (char*)malloc(sizeof(char*)*500);
	
	while (true)  // siempre está esperando la conexión de algún cliente
	{
		printf("Awaiting connection...\n");
		new_socket = accept(my_socket, (struct sockaddr*)&addr, (socklen_t*)&addr);
		if (new_socket < 0)
		{
			perror("Connection accept error");
			exit(EXIT_FAILURE);
			return 1;
		}
		
		mycolor.print_str("green", "Connection successful with client");
		
		while (true)  // ciclo para procesar instrucciones mientras el cliente esté conectado
		{
			//mycolor.print_str("cyan", "\nAwaiting instructions from client...");   //printf("\033[1;36mAwaiting instructions from client...\033[0m\n");
			printf("\nAwaiting instructions from client...\n");
			read_status = read(new_socket, buffer, sizeof(buffer));
			//read_status = listen(my_socket, MAX_CONNECTIONS);
			//read_status = receive(new_socket, buffer, sizeof(buffer), 0);
			
			if (read_status < 0) { break; }
			if (read_status == -1) { perror("Socket read error in server"); return 1; }
			if (read_status == 0) { perror("Connection closed"); close(new_socket); return 1; }  // retorna?
			
			printf("Server read from buffer:\n");
			string auxStr(buffer);
			mycolor.print_str("yellow", auxStr);
			
			/* Leyendo buffer, separando args por : dados por el cliente */
			aux = strtok(buffer, ":");
			if (aux != NULL)
			{
				strcpy(decode[0], aux);   // con el separador ":" el servidor reconoce qué comando y con qué parámetros el cliente lo puso en el buffer
			}
			j=1;
			while (true)
			{
				aux = strtok(NULL, ":");
				if (aux == NULL) { break; }
				strcpy(decode[j], aux);
				j++;
			}
			
			/*--- EJECUTANDO COMANDOS DEL CLIENTE ---*/
			
			strcpy(server_answer, "");   // vacía la respuesta para rellenarla con los elementos.
			
			/* (1°) COMANDO: insert(value) */
			if (strcmp(decode[0], "insert_1") == 0)
			{   // decode ~ { insert_1, <value>, null, PID }
				if (! KV_searchKey(&DB, atoi(decode[1])) )
				{
					sprintf(server_answer, "%d", KV_append(&DB, (char*)decode[1]));
				}
				else
				{   // si la llave ya existe, asignada a un valor, no se puede. Tendría que usar update.
					sprintf(server_answer, "[!] Error, ya existe la llave %s en la base de datos.", decode[1]);
				}
			}
			
			/* (2°) COMANDO: insert(key, value) */
			else if (strcmp(decode[0], "insert_2") == 0)
			{   // decode ~ { insert_2, <key>, <value>, PID }
				if (! KV_searchKey(&DB, atoi(decode[1])) )
				{
					DB[atoi(decode[1])] = (char*)malloc(sizeof(decode[2]));
					strcpy(DB[atoi(decode[1])], decode[2]);  //DB[atoi(decode[1])] = (char*)decode[2];
				}
				else
				{
					sprintf(server_answer, "[!] Error, ya existe un par llave-valor con la misma llave");
				}
				
			}
			
			/* (3°) COMANDO: get(key) */
			else if (strcmp(decode[0], "get") == 0)
			{   // decode ~ { get, <key>, null, PID }
				if (! KV_searchKey(&DB, atoi(decode[1])) )
				{
					sprintf(server_answer, "[!] Error, la llave %s no existe en la base de datos", decode[1]);
				}
				else
				{
					sprintf(server_answer, "%s", DB[atoi(decode[1])]);
				}
			}
			
			/* (4°) COMANDO: peek(value) */
			else if (strcmp(decode[0], "peek") == 0)
			{   // decode ~ { peek, <value>, null, PID }
				if (! KV_searchKey(&DB, atoi(decode[1])) )
				{
					sprintf(server_answer, "false");
				}
				else
				{
					sprintf(server_answer, "true");
				}
			}
			
			/* (5°) COMANDO: update(key, value) */
			else if (strcmp(decode[0], "update") == 0)
			{   // decode ~ { update, <key>, <value>, PID }
				if ( ! KV_searchKey(&DB, atoi(decode[1])) )
				{
					sprintf(server_answer, "[!] Error, la llave %s no existe en la base de datos", decode[1]);
				}
				else
				{
					DB[atoi(decode[1])] = (char*)malloc(sizeof(decode[2]));
					strcpy(DB[atoi(decode[1])], decode[2]);  //DB[atoi(decode[1])] = (char*)decode[2];
				}
			}
			
			/* (6°) COMANDO: delete(key) */
			else if (strcmp(decode[0], "delete") == 0)
			{   // decode ~ { delete, <key>, null, PID }
				if (! KV_searchKey(&DB, atoi(decode[1])) )
				{
					sprintf(server_answer, "[!] Error, la llave %s no existe en la base de datos", decode[1]);
				}
				else
				{
					DB.erase(atoi(decode[1]));
				}
			}
			
			/* (7°) COMANDO: list */
			else if (strcmp(decode[0], "list") == 0)
			{   // decode ~ { list, null, null, PID }
				strcpy(server_answer, KV_get_printable(&DB, 500));
				//printf("%s\n", server_answer);  // [TEST]
				
				if (strlen(server_answer) > BUFF_SIZE)
				{
					printf("[!] Error, buffer overflow: server_answer is too large: %s (lenght: %d)", server_answer, (int)strlen(server_answer));
					return 1;
				}
			}
			
			// a este punto hizo lo que el cliente pidió y lo puso en server_answer -
			// para colocarlo a continuación en el buffer y mandar la respuesta.
			strcpy(buffer, server_answer);
			send_status = send(new_socket, buffer, sizeof(buffer), 0);
			//send_status = write(new_socket, buffer, strlen(buffer));
			if (send_status == -1)
			{
				perror("Socket send error in server");
				return 1;
			}
			///printf("Server answered on buffer \"%s\"\n", buffer);
		}
		printf("The server does not detect any connection\n");
	}
	
	free(aux);
	DB.clear();
	return 0;
}
