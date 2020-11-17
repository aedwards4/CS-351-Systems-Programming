#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

	//Error checking - num of args
	if (argc < 3) {
		printf("Usage: %s FILENAME\n", argv[0]);
		exit(1);
	}

	//Setting up command args for execp
	char *args[argc-2];
	int index=2;
	for(int i=0; i<argc-2;i++){
		args[i] = argv[index];
		index++;
	}

	//Setting up variables
	pid_t pid;
	int status;

	//Fork # of times given
	for(int i=0; i<=atoi(argv[1])-1; i++){
		pid = fork();

	//Child process - call function
	if (pid == 0){
		if(execvp(args[0], args) < 0){
			printf("%s: Command not found\n", args[0]);
			fflush(stdout);
			exit(1);
		}
	}
	//Parent process - wait for child
	else{
		waitpid(pid,&status, WUNTRACED | WCONTINUED);
	}

}

	//Output success message
	printf("SUCCESS in running ");
	for(int i=0; i<argc; i++){
		printf("%s ", argv[i]);
	}
	printf("\n");

	exit(0);
}
