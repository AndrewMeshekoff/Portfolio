default: all

all: conServer threadClient

threadClient: threadClient.cpp
	g++ -pthread threadClient.cpp -o threadClient.out

conServer: conServer.cpp
	g++ conServer.cpp -o conServer.out

clean:
	rm *.out
	rm -r *files/
