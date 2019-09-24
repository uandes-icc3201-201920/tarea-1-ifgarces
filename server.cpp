#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "database_struct.cpp"   // el "util.h"


#define MAX_CONNECTIONS 10
#define BUFF_SIZE 200
#define DATABASE_SIZE 1000

using namespace std;

KVStore DB[DATABASE_SIZE];
unsigned int DB_size = 0;

char socket_path[200];
int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);


int main(int argc, char** argv)
{
	strcpy(socket_path, "/tmp/DB.tuples.sock");
	DB[0] = KVStore(0, (char*)"hola");
	DB_size++;
	DB[1] = KVStore(1, (char*)"compañero");
	DB_size++;
	
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
		else
		{
			return EXIT_FAILURE;
		}
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
		return 1;
	}
	
	
	
	/*--- COMIENZA CICLO PARA PROCESAR INSTRUCCIONES DE LOS CLIENTES ---*/
	
	char buffer[BUFF_SIZE];
	int new_socket, read_status, send_status;
	int search_result, j;
	char decode[3][50] = {}; // forma: { tipo_instrucción, arg1, arg2 }
	char server_answer[200] = "", line[50] = "";
	char* aux;
	
	while (true)  // siempre está esperando la conexión de algún cliente
	{
		cout << "Awaiting connection..." << endl;
		new_socket = accept(my_socket, (struct sockaddr*)&addr, (socklen_t*)&addr);
		if (new_socket < 0)
		{
			perror("Connection accept error");
			exit(EXIT_FAILURE);
			return 1;
		}
		
		cout << "Connection successful with client" << endl;
		
		while (true)  // ciclo para procesar instrucciones mientras el cliente esté conectado
		{
			cout << "\033[1;36mAwaiting instructions from client...\033[0m" << endl;
			read_status = read(new_socket, buffer, sizeof(buffer));
			//read_status = listen(my_socket, MAX_CONNECTIONS);
			//read_status = receive(new_socket, buffer, sizeof(buffer), 0);
			
			if (read_status < 0) { break; }
			if (read_status == -1) { perror("Socket read error in server"); return 1; }
			if (read_status == 0) { perror("Connection closed"); close(new_socket); return 1; }  // retorna?
			
			cout << "  (i) Server read from buffer \"" << buffer << "\" with status " << read_status << "\n" << endl;
			
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
			
			/* (1°) COMANDO: insert(value) */
			if (strcmp(decode[0], "insert_1") == 0)
			{                                   // decode = { "insert_1", <valor (char*)>, "" }
				DB[DB_size+1] = KVStore(DB_size+1, decode[1]);
				sprintf(server_answer, "%d", DB_size+1);
			}
			
			/* (2°) COMANDO: insert(key, value) */
			else if (strcmp(decode[0], "insert_2") == 0)
			{                                    // decode = { "insert_2", <llave>, <valor> }
				if (Search_key(DB, DB_size, atoi(decode[1])) == -1)
				{
					DB[DB_size+1] = KVStore(atoi(decode[1]), decode[2]);
				}
				else
				{
					sprintf(server_answer, "[!] Error, ya existe un par llave-valor con la misma llave");
				}
				
			}
			
			/* (3°) COMANDO: get(key) */
			else if (strcmp(decode[0], "get") == 0)
			{
				search_result = Search_key(DB, DATABASE_SIZE, atoi(decode[1]));
				if (search_result == -1)
				{
					sprintf(server_answer, "[!] Error, la llave %s no existe en la base de datos", decode[1]);
				}
				else
				{
					sprintf(server_answer, "%s", DB[search_result].value);
				}
			}
			
			/* (4°) COMANDO: peek(value) */
			else if (strcmp(decode[0], "peek") == 0)
			{
				if (Search_key(DB, DATABASE_SIZE, atoi(decode[1])) == -1)
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
			{
				search_result = Search_key(DB, DATABASE_SIZE, atoi(decode[1]));
				if (search_result == -1)
				{
					sprintf(server_answer, "[!] Error, la llave %s no existe en la base de datos", decode[1]);
				}
				else
				{
					DB[search_result].value = decode[2];
				}
			}
			
			/* (6°) COMANDO: delete(key) */
			else if (strcmp(decode[0], "delete") == 0)
			{
				
			}
			
			/* (7°) COMANDO: list */
			else if (strcmp(decode[0], "list") == 0)
			{
				strcpy(server_answer, "");   // vacía la respuesta para rellenarla con los elementos.
				
				cout << "[test]\n  KEY\tVALUE" << endl;   // [TEST]
				for (unsigned int k=0; k<DB_size; k++)
				{
					if (strlen(server_answer) > BUFF_SIZE)
					{
						printf("[!] Error, buffer overflow: server_answer is too large: %s (lenght: %d)", server_answer, (int)strlen(server_answer));
						return 1;
					}
					printf("  %d\t%s\n", DB[k].key, DB[k].value);   // [TEST]
					sprintf(line, "%d,%s;", DB[k].key, DB[k].value);  // formato respuesta (cada línea): {<key>, <value>;}
					strcat(server_answer, line);
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
			cout << "  (i) Server answered on buffer \"" << buffer << "\"\n" << endl;
		}
	}
	return 0;
}
