/* =======================================================================
Authors: Amy Sukpaisarn and Mariana Martinez
Date Written: 9/22/17
Course: CS433
Assignment: Unix Shell

Description: Program is a shell, it will accept user commands and implement them.
Will include being able to see the previous process, so like a history tab. 
Plus there will be ampersands at the end of each command to have it run in
the background.

file: myshell.cpp
======================================================================*/
#ifndef MYSHELL_CPP
#define MYSHELL_CPP

#include <iostream>
#include <string>
#include <stdlib.h>
#include "myshell.h"
using namespace std;

/* Sets next to true */
MyShell::MyShell(){
	next = true;
}
/* Returns the current name directory */
string MyShell::displayCurrDirect(string route){
	return route.substr(route.find_last_of('/') + 1);
}
/* Gets the command and arguments from user */
void MyShell::getArgs(char *argList[]){
	/* Fills an arg with null to clear the arg array */
	memset(&argList[0], '\0', sizeof(argList));
	int i = 0;
	cin.getline(userStr, 254);

	if(strcmp(userStr, "") == 0){
		cout << "Cannot send blank commands." << endl;
		getArgs(argList);
	}
	char *coin = strtok(str, " ");
	string myStr = "";

	while(coin != NULL){
		argList[i] = coin;
		coin = strtok(NULL, " ");
		myStr = myStr + std::string(argList[i]) + " ";
		i++;
	}
	if(strcmp(argList[0], "history") != 0 && argList[0][0] != '!'){
		hStck.push(myStr);
	}
	argList[i] = '\0';
}

/* Executes previous command, by reading it from history */
void MyShell::executePrev(char *arg[]){
	/* user string from the input*/
	char userStr[2];
	memcpy(userStr, &arg[0][1], 2);
	userStr[2] = '\0';

	/* Root is needed to find the history*/
	int root = 0;
	if(userStr[0] == '#'){
		root = -1;
	}
	else if(userStr[0] >= '0' && userStr[0] <= '9'){
		root = atoi(userStr);
	}
	else
		cout << "Invalid Command" << endl;
		return;
	/* We now have the root number, now to return the root to char array*/
	char *order;
	try{
		order = (char*)hist.returnElem(root).c_str();
	}
	catch(stack::invalid){
		cout << "Cannot find command in history!" << endl;
		return;
	}
	/* Splits string into coins/tokens and store the command into arg list */
	char *coin = strtok(order, " ");
	memset(&arg[0], '\0', sizeof(arg));
	int i = 0;
	while(coin != NULL){
		arg[i] = coin;
		coin = strtok(NULL, " ");
		i++;
	}
	/* Ends the array with a null*/
	arg[i] = '\0';
	executeCom(arg);
}
/* Executes the command requested by user */
void MyShell::executeCom(char *arg[]){
	if(strcmp(arg[0], "quit") == 0){
		exitShell();
	}
	else if(strcmp(arg[0], "history") == 0){
		displayHistory();
	}
	else if(strcmp(arg[0], "cd") == 0){
		changeDirectory();
	}
	else if(strcmp(arg[0], "all direct") == 0){
		displayAllDirect();
	}
	else if(strcmp(arg[0], "pop direct") == 0){
		popDirectory();
	}
	else if(strcmp(arg[0], "push direct") == 0){
		pushDirectory();
	}
	/* ! in front means to repeat the command from history*/
	else if(arg[0][0] == '!'){
		executePrev(arg);
	}
	else
		executeFile(arg);
}
/* Exits the shell */
void MyShell::exitShell(){
	next = false;
	exit(1);
}
/* Changes the directory */
void MyShell::changeDirectory(char *argList[]){
	/* Checks to see if its a valid directory */
	int changeD = chdir(argList[1]);
	if(changeD == -1){
		perror("Failed");
		return;
	}
}
/* Pushes the directory */
void MyShell::pushDirectory(char *argList[]){
	string currD = getcwd(NULL, 0);
	int changeD = chdir(argList[1]);
	if(changeD == -1){
		perror("Failed");
		return;
	}
	dStck.push(currD);
}
/* Pops the directory */
void MyShell::popDirectory(char *argList[]){
	if(dStck.isEmpty()){
		cout << "Shell push: Stack is empty" << endl;
		return;
	}
	int changeD = chdir("..");
	if(changeD == -1){
		perror("Failed");
		return;
	}
	dStck.pop();
}
/* Displays the commands that have been recently used */
void MyShell::displayHistory(){
	if(hStck.isEmpty()){
		cout << "No recent commands" << endl;
		return;
	}
	hStck.displayAllDirect();
}
/* Displays all directories*/
void MyShell::displayAllDirect(){
	if(dStck.isEmpty()){
		cout << "Cannot display: stack is empty" << endl;
		return;
	}
	dStck.displayAllDirect();
}
/* Executes built in commands in UNIX */
void MyShell::executeFile(char *argList[]){
	int i = 0;
	while(argList[i] != NULL){
		i++;
	}
	if(std::string(argList[i - 1]) == "&"){
		argList[i - 1] = '\0';
	}

	child_proc = fork();
	if(child_proc == 0){
		execvp(argList[0], argList);
		perror("Failed");
		exit(0);
	}
	else if(child_proc == -1){
		cout << "Error" << endl;
		return;
	}

	if(argList[i - 1] != '\0'){
		wait(&currStat);
	}
}
/* Returns the current status of the next variable */
bool MyShell::getNext(){
	return next;
}
#endif
