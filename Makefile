MYFLAGS = -Wall

all: clean *.cpp *.h
	reset
	g++ -o client client.cpp $(MYFLAGS) -pthread
	g++ -o server server.cpp $(MYFLAGS)
	#g++ -o timer timer.cpp  $(MYFLAGS)
	
delete_socket:
	rm -f socket

clean: delete_socket
	rm -f client server timer *.exe.stackdump *.o *.exe
	
runs: clean
	g++ -o server server.cpp $(MYFLAGS)
	./server

runc: clean
	g++ -o client client.cpp $(MYFLAGS)
	./client
	
test: clean
	g++ -o T TEST.cpp $(MYFLAGS)
	
	# FORZAR QUE NO SE REPITA UNA LLAVE PARA UN VALOR
	# NO OLVIDAR QUE connect DEL CLIENTE REQUIERE DE LA RUTA DEL SOCKET
	./T

# Ver cap. 30.2 libro
# https://www.geeksforgeeks.org/socket-programming-cc/
# https://www.geeksforgeeks.org/mutex-vs-semaphore/