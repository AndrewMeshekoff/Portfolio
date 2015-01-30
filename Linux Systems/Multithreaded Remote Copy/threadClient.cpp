/*
* Course: CS 100 Winter 2014
*
* First Name: Andrew
* Last Name: Meshekoff
* Username: amesh001
* email address: amesh001@ucr.edu
*
*
* Assignment: Homework #9
*
* I hereby certify that the contents of this file represent
* my own original individual work. Nowhere herein is there
* code from any outside resources such as another individual,
* a website, or publishings unless specifically designated as
* permissible by the instructor or TA. */
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <signal.h>
#include <pwd.h>

using namespace std;

#define PORT 49948
#define RBUFF 500
#define WBUFF 200
#define NUMTHREADS 10

struct threadInfo {
	int threadNum;
	int argc;
	char ** argv;
};

void makeThreadDir(char * dir, char * threadFld, int tNum) { //creates initial directory.
	int endDir;
	char cpyDir[60];

	sprintf(threadFld, "Thread%dfiles/", tNum);
	for (int i = 0; dir[i]; ++i)
		if (dir[i] == '/' && dir[i+1] != 0)
			endDir = i+1;

	strcpy(cpyDir, threadFld);
	mkdir(cpyDir, S_IRWXU|S_IRWXG|S_IRWXO);
	strcat(cpyDir, dir+endDir);
	mkdir(cpyDir, S_IRWXU|S_IRWXG|S_IRWXO);
}

void makeClient (int & sockfd, sockaddr_in & hostAddr, char * domain) { //initializes client socket.
	char servername[200];
	hostent * hostip;
	long ipAddr;

	strcpy(servername, domain);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0) {
		cerr << "Error: Unable to create socket.\n";
		exit(-1);
	}

	memset(&hostAddr, 0, sizeof(hostAddr));
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(PORT);
	hostip = gethostbyname(servername);
	if (hostip == 0) {
		cerr << "Error: Unable to get host IP.\n";
		exit(-1);
	}
	memcpy(&ipAddr, hostip->h_addr, hostip->h_length);
	hostAddr.sin_addr.s_addr = ipAddr;
}

bool getFilePath(int sockfd, char * output) { //returns false on end instruction.
	recv(sockfd, output, RBUFF, 0);
	return output[0] != '$';
}

void makePath (char * filePath, char * threadFld, char * output) {
	strcpy (filePath, threadFld);
	strcat (filePath, output);
}

int makeFile (char * fileDir) { //creates file descriptor to write to.
	int ofd = -1;
	if((ofd = open(fileDir, O_RDWR|O_CREAT|O_TRUNC, 
	S_IRWXU|S_IRGRP|S_IROTH)) == -1) {
		cout << "Error: file " << fileDir << " could not be created." << endl;
		exit(-1);
	}
	return ofd;
}

void handshake(int sockfd, char * question) { //tells server it is ready to receive.
	strcpy(question, "$ready");
	if(write(sockfd, question, WBUFF)<0) {
		cerr << "Error: could not send handshake.\n";
		exit(-1);
	}
}

void fillFile (int sockfd, int ofd, char * output, char * arg) { 
//retreives size of file and fills it with its number of bytes.
	read(sockfd, output, RBUFF);
	int totalB = atoi(output);
	int readB;
	while (totalB>0) {
		if((readB=read(sockfd, output, RBUFF))<0) {
			cerr << "Error: packet could not be recieved.\n";
			exit(-1);
		}
		if(write(ofd, output, readB)<0) {
			cerr << "Error: could not write to file.\n";
			exit(-1);
		}
		totalB -= readB;
	}
	close (ofd);
}


void callServer (threadInfo * data, int sockfd, sockaddr_in & hostAddr) {
//starts interaction with client by sending the desired directory.
	char arg[WBUFF];
	char output[RBUFF];
	char threadFld[30];
	char filePath[80];

	if (connect(sockfd, (struct sockaddr *) &hostAddr, sizeof(hostAddr))<0) {
		cerr << "Error: Unable to connect.\n";
		exit(-1);
	}

	for(int i = 2; i<(*data).argc; ++i) {
		strcpy(arg, (*data).argv[i]);
		makeThreadDir((*data).argv[i], threadFld, (*data).threadNum);
		write(sockfd, arg, WBUFF);
		while(getFilePath(sockfd, output)) {
			makePath(filePath, threadFld, output);
			fillFile(sockfd, makeFile(filePath), output, arg);
			handshake(sockfd, arg);
		}
	}
	close(sockfd);
}

void * conClient (void * arg) { //main subroutines.
	threadInfo * data = (struct threadInfo *) arg;
	int sockfd;
	sockaddr_in hostAddr;

	makeClient(sockfd, hostAddr, (*data).argv[1]);
	callServer(data, sockfd, hostAddr);

	pthread_exit(0);
}

void makeThreads(int argc, char ** argv) {//makes threads
	pthread_t threads[NUMTHREADS];
	threadInfo arguments[NUMTHREADS];
	int status;
	
	for (int i = 0; i<NUMTHREADS; i++) {
		arguments[i].threadNum = i+1;
		arguments[i].argc = argc;
		arguments[i].argv = argv;
	}

	for (int i = 0; i<NUMTHREADS; i++) {
		status = pthread_create(threads+i, NULL, conClient, (void *) (arguments+i));
		if (status<0) {
			cerr << "Error: Could not create thread.\n";
			exit(-1);
		}
	}

	for (int i = 0; i<NUMTHREADS; i++)
		pthread_join(threads[i], NULL);
}

int main(int argc, char * argv[]) {

	if(argc>2)
		makeThreads(argc, argv);
	else {
		cerr << "Error: too few arguments.\n";
		return -1;
	}

	return 0;
}
