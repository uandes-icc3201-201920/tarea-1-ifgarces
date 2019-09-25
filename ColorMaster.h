#ifndef __COLOR__
#define __COLOR__

#include <stdlib.h>
#include <string.h>
#include <iostream>
//#define BLACK 30, RED 31, GREEN 32, YELLOW 33, BLUE 34, PURPLE 35, CYAN 36, WHITE 37

class ColorMaster
{
	private:
		char* start;  // ASCII scape code sequence header
		char* end; // ASCII scape code sequence foot
	public:
		ColorMaster();
		void print_charasterisco(const char*, char*);  // imprime texto en color válido (no incluye fondo)
		void print_str(std::string, std::string);
};
#endif