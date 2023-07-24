/*
Program to test CGI.py
fork and exec CGI.py
*/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
    int pid;
    int status;
    char* args[3];
    args[0] = (char*)malloc(10);
    args[1] = (char*)malloc(10);
    args[2] = NULL;
    strcpy(args[0], "python");
    strcpy(args[1], "CGI.py");
    pid = fork();
    if(pid == 0){
        execvp(args[0], args);
    }
    else{
        wait(&status);
    }
    return 0;
}