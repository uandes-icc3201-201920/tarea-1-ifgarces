#include "ColorMaster.h"

ColorMaster::ColorMaster() {}

void ColorMaster::print_charasterisco(const char* color, char* what_to_print)
{
	start = (char*)malloc(sizeof(char)*20);
	end = (char*)malloc(sizeof(char)*20);
	strcpy(end, "\033[0m");
	int colorcode;
	if (strcmp(color, "black") == 0)       { colorcode = 30; }
	else if (strcmp(color, "red") == 0)    { colorcode = 31; }
	else if (strcmp(color, "green") == 0)  { colorcode = 32; }
	else if (strcmp(color, "yellow") == 0) { colorcode = 33; }
	else if (strcmp(color, "blue") == 0)   { colorcode = 34; }
	else if (strcmp(color, "purple") == 0) { colorcode = 35; }
	else if (strcmp(color, "cyan") == 0)   { colorcode = 36; }
	else if (strcmp(color, "white") == 0)  { colorcode = 37; }
	else { printf("ColorError\n"); return; }   // error
	sprintf(start, "\033[1;%dm", colorcode);  // format: "\033[<STYLE>;<COLOR>m<MY_TEXT>\033[0m"
	printf("%s%s%s\n", start, what_to_print, end);
	free(start);
	free(end);
}

void ColorMaster::print_str(std::string color, std::string what_to_print)
{
	start = (char*)malloc(sizeof(char)*20);
	end = (char*)malloc(sizeof(char)*20);
	strcpy(end, "\033[0m");
	
	int colorcode;
	if (color == "black")       { colorcode = 30; }
	else if (color == "red")    { colorcode = 31; }
	else if (color == "green")  { colorcode = 32; }
	else if (color == "yellow") { colorcode = 33; }
	else if (color == "blue")   { colorcode = 34; }
	else if (color == "purple") { colorcode = 35; }
	else if (color == "cyan")   { colorcode = 36; }
	else if (color == "white")  { colorcode = 37; }
	else { printf("ColorError\n"); return; }   // error
	sprintf(start, "\033[1;%dm", colorcode);  // format: "\033[<STYLE>;<COLOR>m<MY_TEXT>\033[0m"
	std::cout << start << what_to_print << end << std::endl;
	free(start);
	free(end);
}