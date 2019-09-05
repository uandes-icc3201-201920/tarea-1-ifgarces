client_exe: server_exe timer_exe
	g++ -o client_exe client.cpp

server_exe:
	g++ -o server_exe server.cpp
	
timer_exe:
	g++ -o timer_exe timer.cpp
	
clean:
	rm -f client_exe server_exe timer_exe *.exe.stackdump

compress:
	tar -zcvf Garces_Ignacio_Tarea1-1.tar.gz Makefile *.cpp *.h *.pdf