#include <stdio.h>
void bubblesort(int [], int);
void swap(int*,int*);


void swap(int *x,int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

void bubblesort(int [] nums, int length){
	int x,y;
	for(int i=0; i<(length-1); i++){
		for(int m=0; m<((length-1)-i); m++){
			if (nums[i] > nums[m+1]){
				swap(&arr[m], &arr[m+1]);
			}
		}
	}

}

int main(int argc, char *argv[])
{
	//Read file from I/O redirection
	int fileArray[10];
	for (int i=0; i<10; i++){
		//Copy numbers into array
	}
	bubblesort(fileArray,fileArraylength);

	return 0;
}
