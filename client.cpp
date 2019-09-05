#include <iostream>
#include <memory>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "util.h"

using namespace std;

char *socket_path = "\0hidden";

int main(int argc, char** argv)
{
	char* cmd = "";
	
	struct sockaddr_un addr;
	
	int my_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);	
	
	char buffer[200];
	int new_socket, read_status;
	
	char* aux = "";
	int clientCMDid = 0;    // clientCMDid sirve para que server sepa qué operación debe hacer con los datos del buffer, con el o los números.
	int inputVals[3] = {0, 0, clientCMDid};
	string CMDstr;
	
	while (cmd != "quit")
	{
		cout << "client >";		
		scanf("%s", cmd);
		
		if (cmd == "connect")
		{
			if (connect(my_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
			{
				perror("Connect error");
				return -1;
			}
			cout << "Connection successful" << endl;
		}
		
		if (cmd == "disconnect")
		{
			unlink((const char*)(struct sockaddr*)&addr);
			cout << "Disconnected" << endl;
		}
		
		if (cmd == "quit")
		{
			//break;
			unlink((const char*)(struct sockaddr*)&addr);
			return 0;
		}
		
		CMDstr = cmd;
		if (CMDstr.find("insert") != string::npos)
		{
			if (CMDstr.find(",") == string::npos)     // un argumento
			{
				clientCMDid = 0;
				
				aux = (char*)strtok(cmd, "(");
				//inputVals = {(strtok(NULL, ")")), 0, clientCMDid};   // de la forma {input_key, input_value}
				inputVals[0] = atoi(strtok(NULL, ")"));
				inputVals[1] = 0;
				inputVals[2] = clientCMDid;
				buffer = (char)inputVals[0];
				
				if (send(my_socket, buffer, sizeof(inputVals[0]), 0) == -1)
				{
					perror("[!] Error, el cliente no pudo enviar la instrucción al servidor mediante el socket.");
					return -1;
				}
			}
			
			else      // dos argumentos
			{
				clientCMDid = 1;
				aux = (char*)strtok(cmd, "(");
				inputVals = {(int)strtok(NULL, ","), (int)strtok(NULL, ")"), clientCMDid};   // de la forma {input_key, input_value}
				buffer = (char*)inputVals;
				
				if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
				{
					perror("[!] Error, el cliente no pudo enviar la instrucción al servidor mediante el socket.");
					return -1;
				}
			}
		}
		
		if (CMDstr.find("get") != string::npos)
		{
			clientCMDid = 2;
			
			
		}
		
		if (CMDstr.find("peek") != string::npos)
		{
			clientCMDid = 3;
			
			
		}
		
		if (CMDstr.find("update") != string::npos)
		{
			clientCMDid = 4;
			
			
		}
		
		if (CMDstr.find("delete") != string::npos)
		{
			clientCMDid = 5;
			
			inputVals = {(int)strtok(NULL, ","), (int)strtok(NULL, ")"), clientCMDid};
			buffer = (char*)inputVals;
			if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
			{
				perror("[!] Error, el cliente no pudo enviar la instrucción al servidor mediante el socket.");
				return -1;
			}
		}
		
		if (CMDstr == "list")
		{
			clientCMDid = 6;
			
			inputVals = {0, 0, clientCMDid};
			buffer = (char*)inputVals;
			
			if (send(my_socket, buffer, sizeof(inputVals), 0) == -1)
			{
				perror("[!] Error, el cliente no pudo enviar la instrucción al servidor mediante el socket.");
				return -1;
			}
		}
		
		else
		{
			cout << "[!] Error, comando no reconocido" << endl;
		}
	}

	return 0;	
}