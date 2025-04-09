#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void interact();
void execute();

int main(void){
	// isatty indicates if there is a line input is from the terminal. if yes, interact() function runs.

	if  (isatty(STDIN_FILENO)==1){
		interact(); }
	else {
		printf("nope");}

	return 0;}


void interact(){
	// interact function

	//declaring a pointer and setting it to NULL so that getline() allocates space for the line input later
	char *line = NULL; 
	size_t len = 0;

	while(1){ //loop started. $ is printed everytime and fflush ensures it is printed out everytime
		printf("$  ");
		fflush(stdout);

		//ssize_t is signed size. getline reads the input from the stdin and stores it in line variable. the size of the whole command in line is stored in nread. and len is updated to hold the allocated size. 
		ssize_t nread = getline(&line, &len, stdin);

		// if the size of nread is -1, the program exits
		if (nread == -1){
			printf("\nexiting \n");
			break;
		}
		//when the end of the input is \n, it is replaced with \0 so it doesn't get printed
		if (line[nread - 1]== '\n'){
			line[nread - 1] = '\0';
		}

		// strcmp is string compare. if line says "exit" then we exit. this is manually exiting out of the program
		if (strcmp(line,"exit")==0){
			break;
		}
		
		//parser. first we allocate memory for the args by creating an array
		char *args[64];
		int i = 0;

		//using strtok to divide the input in line based on \t , \r, \n. and then saving each to the args array and the last argument in args will be NULL
		char *token = strtok(line, " \t\r\n"); 
		while (token != NULL){
			args[i++] = token;
			token = strtok(NULL, " \t\r\n");}
		args[i] = NULL;




		//execute! pid_t is a datatype for processes. fork() creates a child process. The child process executes the commands from the line. While the child is executing, the parents just waits until it finishes. If the pid value is -1, the fork itself failed and there is no child process. 
		pid_t pid = fork();

		if (pid == 0){
			execvp(args[0], args);
			perror("exec failed");
			exit(EXIT_FAILURE);
		}
		else if (pid>0){
			wait(NULL);

		}
		else{
			perror("fork failed");
		}

	}
	//finally we free the line variable for the next command
	free(line);}
	

