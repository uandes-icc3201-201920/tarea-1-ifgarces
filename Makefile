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
	
# runs: clean
	# g++ -o server server.cpp $(MYFLAGS)
	# ./server

# runc: clean
	# g++ -o client client.cpp $(MYFLAGS)
	# ./client
	
# test: clean
	# g++ -o T TEST.cpp $(MYFLAGS)
	# ./T
