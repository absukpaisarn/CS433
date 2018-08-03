/* =======================================================================
Authors: Amy Sukpaisarn and Mariana Martinez
Date Written: 9/22/17
Course: CS433
Assignment: Unix Shell

Description: Program is a shell, it will accept user commands and implement them.
Will include being able to see the previous process, so like a history tab. 
Plus there will be ampersands at the end of each command to have it run in
the background.

file: myshell.h
======================================================================*/
#ifndef MYSHELL_H
#define MYSHELL_H

#include <iostream>
#include "mystck.h"

class MyShell{
	private:
		//directory stack
		MyStack dStck;
		//history stack
		MyStack hStck;
		//current status of child process
		int currStat;
		//holds PID of child process
		pid_t child_proc;
		//array to hold the user input
		char userStr[254];
		//determines if exit command has been called
		bool next;

	public:
		MyShell(); //sets next to true
		string displayCurrDirect(string route); //displays current directory
		void executePrev(char *arg[]); //executes previous command, by reading it from history
		void getArgs(char *argList[]); //gets the command and arguments from user
		void executeCom(char *arg[]); //executes the command requested by user
		void exitShell(); 
		void changeDirectory(char *argList[]);
		void pushDirectory(char *argList[]);
		void popDirectory(char *argList[]);
		void displayHistory(); //displays history of commands used
		void displayAllDirect(); //displays all directories in the stack
		void executeFile(char *argList[]); //executes built in commands in UNIX
		bool getNext(); //returns the current status of the next variable
};

#endif
