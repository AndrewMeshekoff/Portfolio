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
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

#define PORT 49948
#define RBUFF 200
#define WBUFF 500

void createServer(int & sockfd1, sockaddr_in & hostAddr) { //initializes server
	sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd1<0) {
		cerr << "Error: Unable to create socket.\n";
		exit(-1);
	}

	memset(&hostAddr, 0, sizeof(hostAddr));
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(PORT);
	hostAddr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd1, (struct sockaddr *) &hostAddr, sizeof(hostAddr) )<0) {
		cerr << "Error: could not bind socket.\n";
		exit(-1);
	}

}

char * getLast(char * dir, string & dirEnd) { //gets last element of path
	int endDir;
	for (int i = 0; dir[i]; ++i)
		if (dir[i] == '/' && dir[i+1] != 0)
			endDir = i+1;

	 return dir+endDir;
}

void handshake(int sockfd, char * answer) { //tests socket to ensure 
	answer[0]=0;			    //synchronization.
	if(read(sockfd, answer, RBUFF)<0) {
		cerr << "Error: handshake not recieved.\n";
		exit(-1);
	}
	if(strcmp(answer, "$ready")!=0) {
		cerr << "Client/Server not synchronized, exiting.\n";
		exit(-1);
	}
}

void sendDir(int sockfd2, char * dirReq, char * dataPack) {
	DIR * folder = opendir(dirReq);
	dirent * entity = 0;
	struct stat info;
	string baseDir(dirReq);
	string direc;
	string dirSend;
	string dirEnd(getLast(dirReq, dirEnd));
	int ifd; 

	while (entity = readdir(folder)) { //sequences through directory.
		direc = (baseDir+"/"+entity->d_name);
		if(stat(direc.c_str(), &info)==0)
			if(S_ISREG(info.st_mode)) { //sends size and contents.
				ifd = -1;
				if((ifd = open(direc.c_str(), O_RDONLY, 0)) >= 0) {
					dirSend = (dirEnd+"/"+entity->d_name);
					strcpy(dataPack, dirSend.c_str());
					if(write(sockfd2, dataPack, WBUFF)<0)
						cerr << "Error\n";
					sprintf(dataPack, "%d", info.st_size);
					if(write(sockfd2, dataPack, WBUFF)<0)
						cerr << "Error\n";
					sendfile(sockfd2, ifd, 0, info.st_size);
					handshake(sockfd2, dirReq);
					close(ifd);
				}
				else
					cout << "Error: file " << direc << 
					" could not be opened." << endl;
			}
	}
	strcpy(dataPack, "$");	//instructs client to end.
	write(sockfd2, dataPack, WBUFF);
}

void handleChild (int sockfd2) { //initializes child socket.
	char dirReq[RBUFF];
	char dataPack[WBUFF];
	int wBuffSz = WBUFF;
	int rBuffSz = RBUFF;

	if (sockfd2<0) {
		cerr << "Error: could not accept client.\n";
		exit(-1);
	}

	if (setsockopt(sockfd2, SOL_SOCKET, SO_RCVLOWAT, 
	(int *) &rBuffSz, sizeof(rBuffSz))<0) {
		cerr << "Error: could not set socket options.\n";
		exit(-1); 
	}

	read(sockfd2, dirReq, RBUFF);
	sendDir(sockfd2, dirReq, dataPack);
	close(sockfd2);
	_Exit(0);
}

void handleClients (int sockfd1, sockaddr_in hostAddr) {
	int sockfd2;
	int addrSize = sizeof(hostAddr);
	int children = 0;
	pid_t pid;
	int status;

	if(listen(sockfd1, 5)<0) {
		cerr << "Error: could not establish listen queue.\n";
		exit(-1);
	}

	fcntl(sockfd1, F_SETFL, O_NONBLOCK);
	while(1) { //loop that forks upon request.
		sockfd2 = accept(sockfd1, 0, 0);
		if(sockfd2>0) {
			pid=fork();
			if (pid == 0)
				handleChild(sockfd2);
			else if (pid>0)
				children++;
			else if (pid<0) {
				cerr << "Error: Could not branch request.\n";
				exit(-1);
			}
		}
		if(children>0)
			if(waitpid(-1, &status, WNOHANG)>0)
				children--;	
	}
}

void conServer() { //main subroutines.
	int sockfd1;
	sockaddr_in hostAddr;

	createServer(sockfd1, hostAddr);

	handleClients(sockfd1, hostAddr);
}

int main() {

	conServer();

	return 0;
}
