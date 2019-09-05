client_exe: server_exe timer_exe delete_socket
	g++ -o client_exe client.cpp

server_exe:
	g++ -o server_exe server.cpp
	
timer_exe:
	#g++ -o timer_exe timer.cpp
	
delete_socket:
	rm -f socket
	
clean: delete_socket
	rm -f client_exe server_exe timer_exe *.exe.stackdump *.o