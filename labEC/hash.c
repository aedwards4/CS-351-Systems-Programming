#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int hash(char*);

int main(int argc, char *argv[])
{

	//Error checking - num of args
	if (argc < 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    exit(1);
  }

	//call hash function
	hash(argv[1]);

	return 0;
}

int hash(char* c)
{
	//Setting up variables for loop
	int sum = 0;
	int len = strlen(c);

	//add together the integer value of each character
	for(int i=0; i < len; i++){
		sum = sum + (unsigned char)(c[i]);
	}

	//output sum to screen
	printf("%d\n",sum);
	return 1;
}
