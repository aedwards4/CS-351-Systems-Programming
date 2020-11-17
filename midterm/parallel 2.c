#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{

	if (argc < 3) {
		printf("Error: Too few arguments");
    exit(1);
  }
	if (size(argv[1]) != sizeof(int){
		printf("Error: Incorrect type of argument")
		exit(1);
	}


	numIter = log2(argv[1]);
	//Command = argv[2]
	//Command Arguments = argv[3]

	//Attempting to fork and execute command appropriate number of times
	for(int i=0; i<numIter; i++){
		if (fork() == 0){
			exec(argv[2],argv[3]);
		}
		else{
			wait(NULL);
		}
	}
	return 0;
}
