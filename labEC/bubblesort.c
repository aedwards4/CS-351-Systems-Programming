#include <stdio.h>
#include <stdlib.h>

void bubblesort(int [], int);
void swap(int*,int*);

int main(int argc, char *argv[])
{
	//Setting up variables for loop
	int numArray[10];
	int index = 0;
	int k = 0;

	//store integers into array
	while (!feof(stdin)){
		scanf("%d",&k);
		numArray[index] = k;
		index++;
	}

	//run bubblesort
	bubblesort(numArray,10);

	//print out sorted array
	for(int i=0; i < 10; i++){
		printf("%d ", numArray[i]);
	}
	printf("\n");

	exit(0);
}

//simple swap function
void swap(int* x, int* y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

//bubblesort function
void bubblesort(int arr [], int num){

	//iterate over array for each element in array,
	//comparing the elements
	for (int i=0; i < (num-1); i++){
		for (int j=0; j < (num-i-1); j++){
			if (arr[j] > arr[j+1]){
				swap(&arr[j], &arr[j+1]);
			}
		}
	}
}
