/*
* Course: CS 100 Winter 2014
*
* First Name: Andrew
* Last Name: Meshekoff
* Username: amesh001
* email address: amesh001@ucr.edu
*
*
* Assignment: Homework #6
*
* I hereby certify that the contents of this file represent
* my own original individual work. Nowhere herein is there
* code from any outside resources such as another individual,
* a website, or publishings unless specifically designated as
* permissible by the instructor or TA. */

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

using namespace std;

void nullify(char ** list, char * cmd) { // generates c-string list and null
	int li = 0;			 // terminates items.
	if (cmd[0]&&cmd[0]!=' ')
		list[li++] = cmd;

	for(int i = 0; cmd[i]; i++) {
		if(cmd[i]==' ')
			cmd[i] = 0;
		else if(i>0 && cmd[i-1]==0)
			list[li++] = cmd+i;
	}

	list[li] = 0;
}

bool findAmp(char ** list) {
	for(int i=0; list[i]; ++i)
		if(*list[i]=='&') {
			list[i] = 0;
			return true;
		}
	return false;
}

bool checkIO(char ** list , char * & input, char * & output) { //looks for ins/ext operators
	for(int i = 0; list[i]; i++) {
		if((*list[i])=='<')
			input = list[i+1];
		else if((*list[i])=='>')
			output = list[i+1];
	}
}

int makeInputF(char * input) {
	int ifd = -1;
	if((ifd = open(input, O_RDONLY)) < 0) {
		cout<<"Error: file "<<input<<" could not be opened."<<endl;
		throw -1;
	}
	return ifd;
}

int makeOutputF(char * output) {
	int ofd = -1;
	if((ofd = open(output, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU|S_IRGRP|S_IROTH)) ==-1) {
		cout<<"Error: file "<<output<<" could not be created."<<endl;
		throw -1;
	}
	return ofd;
}

void redirectIn(int ifd) { //captures std in
	if(dup2(ifd, 0) != 0) {
		cout<<"Error duping standard input."<<endl;
		throw -1;
	}
	close(ifd);
}

void redirectOut(int ofd) { //captures std out
	if(dup2(ofd, 1) == -1) {
		cout<<"Error duping standard output."<<endl;
		throw -1;
	}
	close(ofd);
}

bool pathExec(char ** list) { // searches through path variables
	char pathenv[512];	// executing filename in each.
	pathenv[0] = 0;
	strcpy(pathenv, getenv("PATH"));
	char path[256];
	int start = 0, end = 0; // bounds of next address
	while(pathenv[start]) {
		if(pathenv[end]==':'||pathenv[end]==0) {
			strncpy(path, pathenv+start, end-start);
			path[end-start] = 0;
			strcat(path, "/");
			strcat(path, *list);	
			if(execv(path, list)!=-1)
				return true;
			start=end;
			if(pathenv[end])
				start++;
		}
		end++;
	}
	return false;
}

void processCmd(int ifd, int ofd, char ** list) { //general forking routine
	int status;
	pid_t process;

	process = fork();
	if(process == 0) {
		if(ifd!=-1) //checks for input/output redirection
			redirectIn(ifd);
		if(ofd!=-1)
			redirectOut(ofd);
		if(!pathExec(list)) {
			cout<<"Error executing command: "<<*list<<endl;
			exit (-1);
		}
		if(ifd!=-1)
			close(ifd);
		if(ofd!=-1)
			close(ofd);
		_Exit(0);
	}
	else if(process > 0) {
		if(ifd!=-1)
			close(ifd);
		if(ofd!=-1)
			close(ofd);
	}
	else if (process < 0){
		cout<<"Error: could not create command process";
		exit(-1);
	}
}

int countPipes(char ** list) {
	int count = 0;
	for(int i=0; list[i]; ++i) 
		if(*list[i]=='|')
			count++;
	return count;
}

void makeCmd(char ** list, char *** cmdList) { //creates command array
	int index=0;
	cmdList[index++] = list;
	for(int i=0; list[i]; ++i) {
		if(*list[i]=='|')
			cmdList[index++] = list+i+1;
	}
	cmdList[index] = 0;
}

void terminateList(char ** list) { //removes all IO operators
	for(int i=0; list[i]; ++i)
		if(*list[i]=='|'||*list[i]=='<'||*list[i]=='>')
			list[i] = 0;
}

void stop_cmd(int sig) {
	kill(-1*getpid(), SIGQUIT);
	cout<<endl;
}

void chainCmds(char *** cmdList, char * input, char * output, int & pipeSize, int & child_left) {
	int ifd;
	int ofd;

	int pipefd[pipeSize*2];
	for(int i=0; i<pipeSize; ++i)
		if(pipe(pipefd+i*2)==-1) {
			cerr<<"Error: could not create pipe."<<endl;
			throw -1;
		}

	for(int i=0; i<pipeSize+1; ++i) { //depending on position in pipeline assigns IO
		ifd = -1;
		ofd = -1;
		if (i==0) {
			if(input)
				ifd = makeInputF(input);
			if(pipeSize>0)
				ofd = pipefd[1];
			else
				if(output)
					ofd = makeOutputF(output);
		}
		else if(i>0&&i<pipeSize) {
			ifd = pipefd[(i-1)*2];
			ofd = pipefd[i*2+1];
		}
		else if(i==pipeSize) {
			if(output)
				ofd = makeOutputF(output);
			if(pipeSize>0)
				ifd = pipefd[(i-1)*2];
		}
		processCmd(ifd, ofd, cmdList[i]);
		child_left++;
	}
}

void my_shell(char ** list) { //main my_shell function
	signal(SIGINT, stop_cmd);
	int pipeSize = countPipes(list);
	char ** cmdList[pipeSize+2]; //lists each command array
	makeCmd(list, cmdList);

	char * input=0;
	char * output=0;

	checkIO(list, input, output);
	terminateList(list);

	int child_left = 0;

	chainCmds(cmdList, input, output, pipeSize, child_left);

	int status;
	
	while(child_left) //deallocates child info
	{
		if(wait(&status)<0)
			cerr<<"Unable to terminate child."<<endl;
		--child_left;
	}
}

void childShell(char ** list, int & children) { //creates background shell
	pid_t background = fork();
	if(background==0) {
		my_shell(list);
		_Exit(0);
	}
	else if(background>0)
		children++;
	else if(background<0) {
		cerr<<"Error: could not create background process."<<endl;
		exit(1);
	}
}

void shell_loop() {
	char cmd[1024];
	char * list[64];
	int children = 0;
	int status;

	while(1) {
		signal(SIGINT, SIG_IGN);
		cout.put('%');
		cin.getline(cmd, 1024);
		if(cmd[0]) {
			nullify(list, cmd);
			if(strcmp(*list,"exit") == 0) {
				while(children>0) {
					if(wait(&status)==-1)
						cerr<<"Error: Child process terminated abnormally."<<endl;
					children--;
				}
				exit(0);
			}
			if(findAmp(list)) {
				childShell(list, children);
			}
			else
				my_shell(list);
		}
		if(children>0) {
			if(waitpid(-1, &status, WNOHANG)>0)
				children--;
		}
	}
}

int main() {

	shell_loop();

	return 0;
}
