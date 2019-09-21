FLAGS = -Wall

all: clean *.cpp *.h
	g++ -o client client.cpp $(FLAGS) -pthread
	g++ -o server server.cpp $(FLAGS)
	#g++ -o timer timer.cpp  $(FLAGS)
	
delete_socket:
	rm -f socket

clean: delete_socket
	rm -f client server timer *.exe.stackdump *.o *.exe

tests: clean
	g++ -o server server.cpp
	./server

testc: clean
	g++ -o client client.cpp
	./client

# Ver cap. 30.2 libro
# https://www.geeksforgeeks.org/socket-programming-cc/
# https://www.geeksforgeeks.org/mutex-vs-semaphore/