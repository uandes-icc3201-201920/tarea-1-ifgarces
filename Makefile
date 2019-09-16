FLAGS = -Wall

all: timer *.cpp *.h
	g++ -o client client.cpp $(FLAGS) -pthread
	g++ -o server server.cpp $(FLAGS)

timer:
	#g++ -o timer timer.cpp  $(FLAGS)

delete_socket:
	rm -f socket

clean: delete_socket
	rm -f client server timer *.exe.stackdump *.o *.exe

test: clean all
	./server

# Ver cap. 30.2 libro
# https://www.geeksforgeeks.org/socket-programming-cc/