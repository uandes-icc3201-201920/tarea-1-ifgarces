#include <iostream>
#include <memory>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"

using namespace std;

int main(int argc, char** argv)
{
	string cmd = "";
	
	while (cmd != "quit")
	{
		cout << "client >";		
		cin >> cmd;
		
		if (cmd == "connect")
		{
			
		}
		
		if (cmd == "disconnect")
		{
			
		}
		
		if (cmd == "quit")
		{
			break;
		}
		
		if (cmd.find("insert") != string::npos)
		{
			if (cmd.find(",") == string::npos)     // un argumento
			{
				
			}
			else      // dos argumentos
			{
				
			}
		}
		
		if (cmd.find("get") != string::npos)
		{
			
		}
		
		if (cmd.find("peek") != string::npos)
		{
			
		}
		
		if (cmd.find("update") != string::npos)
		{
			
		}
		
		if (cmd.find("delete") != string::npos)
		{
			
		}
		
		if (cmd == "list")
		{
			
		}
		
		else
		{
			cout << "[!] Error, comando no reconocido";
		}
	}

	return 0;	
}