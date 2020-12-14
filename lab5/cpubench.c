#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#define MSG "* running cpubench %s using %s with size %s and %s threads...\n"

#define USAGE "usage: ./cpubench <mode> <type> <size> <threads> \n" \
"     - mode: flops / matrix \n" \
"     - type: single / double \n" \
"     - size: 10 / 100 / 1000 / 1024 / 4096 / 16386 \n" \
"     - threads: 1 / 2 / 4 \n"

#define GIGAFLOPS 1000000000
#define GIGABYTES 1024*1024*1024
#define MAX_THREADS 4

//Helper structures
typedef struct mat_args_int{
	int **mat1;
	int **mat2;
	int **res;
	int thread_id;
	int num_threads;
	long int size;
} mat_args_i;

typedef struct mat_args_double{
	double **mat1;
	double **mat2;
	double **res;
	int thread_id;
	int num_threads;
	long int size;
} mat_args_d;

typedef struct flop_args_int{
	int *A;
	int *B;
	int thread_id;
	int num_threads;
	long int size;
} flop_args_i;

typedef struct flop_args_double{
	double *A;
	double *B;
	int thread_id;
	int num_threads;
	long int size;
} flop_args_d;

//Matrix multiply helper functions
void *mat_mult_int(void *args)
{
	mat_args_i *targs;
	long int i, j, k, temp;

	targs = (mat_args_i*) args;

	for(i=(long int) targs->thread_id; i<targs->size;
			i+=(long int) targs->num_threads){
		for(k=0; k<targs->size; k++){
			temp = targs->mat1[i][k];
			for(j=0; j<targs->size; j++){
				targs->res[i][j] += temp * targs->mat2[k][j];
			}
		}
	}

	pthread_exit(NULL);
}

void *mat_mult_double(void *args)
{
	mat_args_d *targs;
	long int i, j, k;
	double temp;

	targs = (mat_args_d*) args;

	for(i=(long int) targs->thread_id; i<targs->size;
			i+=(long int) targs->num_threads){
		for(k=0; k<targs->size; k++){
			temp = targs->mat1[i][k];
			for(j=0; j<targs->size; j++){
				targs->res[i][j] += temp * targs->mat2[k][j];
			}
		}
	}

	pthread_exit(NULL);
}


// This function multiplies mat1[][] and mat2[][],
// and stores the result in res[][]
void multiply_int(int** mat1, int** mat2, int** res, int size, int n_threads)
{
	//implement integer matrix multiplication
	long int k, temp;
	int num_threads = n_threads;
	//long int i, j;
	mat_args_i args[MAX_THREADS];
	pthread_t threads[MAX_THREADS];

	//initialize threads and run multiplication in parallel
	for(k=0; k < num_threads; k++){
		args[k].mat1 = mat1;
		args[k].mat2 = mat2;
		args[k].res = res;
		args[k].thread_id = k;
		args[k].num_threads = num_threads;
		args[k].size = size;
	}

	for (k=0; k< num_threads;k++){
		temp = pthread_create(&threads[k], NULL, mat_mult_int, (void *) &args[k]);
		if (temp){
			printf("Error: Could not create thread %ld\n", k);
		}
	}

	for (k=0; k< num_threads; k++){
		temp = pthread_join(threads[k], NULL);
		if (temp){
			printf("Error: Could not join thread %ld\n",k);
		}
	}

}

// This function multiplies mat1[][] and mat2[][],
// and stores the result in res[][]
void multiply_double(double** mat1, double** mat2, double** res, int size, int n_threads)
{
	//implement integer matrix multiplication
	long int k, temp;
	int num_threads = n_threads;
	//long int i, j;
	mat_args_d args[MAX_THREADS];
	pthread_t threads[MAX_THREADS];

	//initialize threads and run multiplication in parallel
	for(k=0; k< num_threads; k++){
		args[k].mat1 = mat1;
		args[k].mat2 = mat2;
		args[k].res = res;
		args[k].thread_id = k;
		args[k].num_threads= num_threads;
		args[k].size = size;
	}

	for (k=0; k< num_threads;k++){
		temp = pthread_create(&threads[k], NULL, mat_mult_double, (void *) &args[k]);
		if (temp){
			printf("Error: Could not create thread %ld\n", k);
		}
	}

	for (k=0; k< num_threads; k++){
		temp = pthread_join(threads[k], NULL);
		if (temp){
			printf("Error: Could not join thread  %ld\n",k);
		}
	}

}


void *compute_flops_int(void *args)
{
	//setting up variables
	flop_args_i *targs;
	long int i, j, loops=GIGAFLOPS, temp=0;
	targs = (flop_args_i*) args;

	for (j=0; j<loops; j++){
		for (i=0; i<(targs->size/2); i++){
			temp += targs->A[i]+targs->B[i]*1;
		}
	}
	printf("Result = %ld\n", temp);
	return 0;
}

void *compute_flops_double(void *args)
{
	//setting up variables
	flop_args_d *targs;
	long int i, j, loops=GIGAFLOPS;
	long double temp=0;
	targs = (flop_args_d*) args;

	for (j=0; j<loops; j++){
		for (i=0; i<(targs->size/2); i++){
			temp += targs->A[i]+targs->B[i]*1;
		}
	}
	printf("Result = %Lf\n", temp);
	return 0;
}


int main(int argc, char **argv)
{
	time_t t;
	srand((unsigned) time(&t));

    if (argc != 5)
    {
        printf(USAGE);
        exit(1);
    }
    else
    {

		int mode = -1;
        if(strcmp(argv[1],"flops") == 0){
        	mode = 0;
				}
        else if(strcmp(argv[1],"matrix") == 0){
        	mode = 1;
				}
        else{
        	mode = -1;
				}

		int type = -1;
        if(strcmp(argv[2],"single") == 0){
        	type = 0;
				}
        else if(strcmp(argv[2],"double") == 0){
        	type = 1;
				}
        else{
        	type = -1;
				}

    unsigned long int size = strtoul(argv[3], NULL, 0);
    int num_threads = atoi(argv[4]);

		struct timeval start, end;

		if (mode == 0 && type == 0)
		{

			//allocating memory
			int *arrA, *arrB;
			arrA = (int*) malloc(sizeof(int) * size);
			arrB = (int*) malloc(sizeof(int) * size);
			//arrC = (double*) malloc(sizeof(double) * (size*GIGAFLOPS));
			if (arrA == NULL || arrB == NULL){
				printf("Malloc error: %s\n", strerror(errno));
			}

			//Start timer
			gettimeofday(&start, NULL);

			//Creating threads
			int k, temp;
			pthread_t threads[MAX_THREADS];
			flop_args_i args[MAX_THREADS];

			for(k=0; k < num_threads; k++){
				args[k].A = arrA;
				args[k].B = arrB;
				args[k].thread_id = k;
				args[k].num_threads = num_threads;
				args[k].size = size/num_threads;
			}

			for (k=0; k< num_threads;k++){
				temp = pthread_create(&threads[k], NULL, compute_flops_int, &args[k]);
				if (temp){
					printf("Error: Could not create thread %d\n", k);
				}
			}

			for (k=0; k< num_threads; k++){
				temp = pthread_join(threads[k], NULL);
				if (temp){
					printf("Error: Could not join thread %d\n",k);
				}
			}

			//End timer
		  gettimeofday(&end, NULL);

			//Free memory
			free(arrA);
			free(arrB);
		}
		else if (mode == 0 && type == 1)
		{

			//allocating memory
			double *arrA, *arrB;
			arrA = (double*) malloc(sizeof(double) * size);
			arrB = (double*) malloc(sizeof(double) * size);
			//arrC = (double*) malloc(sizeof(double) * (size*GIGAFLOPS));
			if (arrA == NULL || arrB == NULL){
				printf("Malloc error: %s\n", strerror(errno));
			}

			//Start timer
			gettimeofday(&start, NULL);

			//Creating threads
			int k, temp;
			pthread_t threads[MAX_THREADS];
			flop_args_d args[MAX_THREADS];

			for(k=0; k < num_threads; k++){
				args[k].A = arrA;
				args[k].B = arrB;
				args[k].thread_id = k;
				args[k].num_threads = num_threads;
				args[k].size = size/num_threads;
			}

			for (k=0; k< num_threads;k++){
				temp = pthread_create(&threads[k], NULL, compute_flops_double, &args[k]);
				if (temp){
					printf("Error: Could not create thread %d\n", k);
				}
			}

			for (k=0; k< num_threads; k++){
				temp = pthread_join(threads[k], NULL);
				if (temp){
					printf("Error: Could not join thread %d\n",k);
				}
			}

			//End timer
		  gettimeofday(&end, NULL);

			//Free memory
			free(arrA);
			free(arrB);
		}
		else if (mode == 1 && type == 0)
		{
			//allocating memory
			int **mat1;
			int **mat2;
			int **res;

			mat1 = (int**) malloc(sizeof(int*) * size);
			mat2 = (int**) malloc(sizeof(int*) * size);
			res = (int**) malloc(sizeof(int*) * size);

			if (mat1 == NULL || mat2 == NULL || res == NULL){
				printf("Malloc error\n");
			}
			for(int i=0; i<size; i++){
				mat1[i] = (int*) malloc(sizeof(int)*size);
				mat2[i] = (int*) malloc(sizeof(int)*size);
				res[i] = (int*) malloc(sizeof(int)*size);
				if (mat1[i] == NULL || mat2[i] == NULL || res[i] == NULL){
					printf("Malloc error\n");
				}
			}

			gettimeofday(&start, NULL);
			multiply_int(mat1,mat2,res,size,num_threads);
	    gettimeofday(&end, NULL);

			for(int i=0; i<size; i++){
				free(mat1[i]);
				free(mat2[i]);
				free(res[i]);
			}

			free(mat1);
			free(mat2);
			free(res);
		}
		else if (mode == 1 && type == 1)
		{
			//allocating memory
			double **mat1;
			double **mat2;
			double **res;

			mat1 = (double**) malloc(sizeof(double*) * size);
			mat2 = (double**) malloc(sizeof(double*) * size);
			res = (double**) malloc(sizeof(double*) * size);

			if (mat1 == NULL || mat2 == NULL || res == NULL){
				printf("Malloc error\n");
			}
			for(int i=0; i<size; i++){
				mat1[i] = (double*) malloc(sizeof(double)*size);
				mat2[i] = (double*) malloc(sizeof(double)*size);
				res[i] = (double*) malloc(sizeof(double)*size);
				if (mat1[i] == NULL || mat2[i] == NULL || res[i] == NULL){
					printf("Malloc error\n");
				}
			}

			gettimeofday(&start, NULL);
			multiply_double(mat1,mat2,res,size,num_threads);
	    gettimeofday(&end, NULL);

			for(int i=0; i<size; i++){
				free(mat1[i]);
				free(mat2[i]);
				free(res[i]);
			}

			free(mat1);
			free(mat2);
			free(res);
		}
		else
		{
        	printf(USAGE);
			printf("unrecognized option, exiting...\n");
        	exit(1);
		}

		double elapsed_time_sec = (((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)))/1000000.0;
		double num_giga_ops = 0;

		if (size*GIGAFLOPS < 0)
		{
			printf("error in size, check for overflow; exiting...\n");
			exit(1);
		}
		if (elapsed_time_sec < 0)
		{
			printf("error in elapsed time, check for proper timing; exiting...\n");
			exit(1);
		}
		if (elapsed_time_sec == 0)
		{
			printf("elapsed time is 0, check for proper timing or make sure to increase amount of work performed; exiting...\n");
			exit(1);
		}

		//size = size / 1000000000;

		if (mode == 0){
			num_giga_ops = size;
		}
		else if (mode == 1){
			num_giga_ops = size*size*size/(GIGABYTES);
		}
		double throughput = num_giga_ops/elapsed_time_sec;
		printf("mode=%s type=%s size=%ld threads=%d time=%lf throughput=%lf\n\n",argv[1],argv[2],size,num_threads,elapsed_time_sec,throughput);

    }

    return 0;
}

/*
Benchmarking processor --> Speed of running application OR (in absence of real application) how fast can it do artihmetic?

HIGH LEVEL
Besides measuring raw arithmatic ops/sec, we are also looking at an application that is common in CS --> matrix multiplication
Talking about these two ways to evaluate speed of processor
1st one only looks at arithmetic ops
2nd will have significant impact from mem hierarchy
3rd one (EC)
	download, configure, compile, and running well-known benchmark
	Linpack solves linear equations through matrix multiplication
Our implementation will expose us to real application kernel
	Results should compare to Linpack

Given skeleton code
 -Has CLI implemented w/several args
 	-- mode: flops/matrix
			---flops = just measuring how many arithmetic ops/sec can you do
					function runs for loop, and we are timing the throughput
					how many ops per iteration? Take total num ops and divide by that number to get actual number
					Figure out how to make it go faster? HINT: Can't make loop of 1mil iterations go faster without:
						The compiler might have optimizations you can give it:
							loop unrolling - takes a loop like 1mil iterations, it will unroll 10 iterations at once, loopimg 100,000 times instead of 1mil
							gcc has diff flags to give it like -o1 -o2 -o3 (Look to see which ones will make code faster)
						Run fewer iterations
							Put 2 arithmetic ops inside of a loop, reduces iterations by 2
							Put 10 arithmetic ops inside of loop, reduces iterations by 10
							(Instruction level parallelism)
						Instructions can run concurrently within one thread!!
					2 types of data:
						single precision (4 byte ints)
						double precision (8 byte doubles)
						*likely to see performance difference
						*larger precision -> more resources

			---matrix = matrix multiplication
				code itself is short (maybe 10 lines), simple algorithm, 3 nested for loops [O(n^3)]
				not a scalable algorithm
				in theory, should be able to do these computations really fast

	--size: diff num of ops to do
		---small
			flops --> 10 billion
			matrix --> 1024x1024
		---medium
			flops --> 100 billion
			matrix --> 4096x4096
		---large
			flops --> 1000 billion (1 trillion)
			matrix --> 16384x16384

		How to run this many ops?
			To run 10bil ops, current code won't run b/c integer can only go up to 2 or 4 billion (signed/unsigned)
				gonna have to deal with data types that can go beyond 32-bit value of int
			to do 1 trillion loops would take long time
				need to do multiple arithmetic ops in a single loop
		How to work for matrices?
			Need to use malloc to allocate mem for ALL sizes
			Large might be an issue for some, leave 1GB for OS then use whatever is leftover (if not enough mem)
			Don't use virtual memory! (can disable it by deleting swap partition `swapoff <device>`)

	--threads/concurrency
		use multiple threads to parallelize matrix multiplication

Some issues that may come up:
-Taking too long to run
	Will implement matrix multiplication and when try to run 16,000x16,0000 takes all day
	Prof gave example numbers of his implementation, which we will likely beat
-Theoretical Computing of Processor speed
	There are some links in pdf, talks about optimizations
	Wikipedia talks about structural parallelism
	formula is pretty simple (in pdf)
		sockets usually = 1
		cores per socket -> look up specifications on computer `cat /proc/cpuinfo`
		cycles/sec simple and straightforward
			Ex: 2GHz processor has 2.0 GigaCycles
		FLOPS per cycle depends on architecture of processor (Most Intel have 16 flops/cycle)
			Find the number in fine print from papers

**NOT ALOT OF CODE TO WRITE
**Majority of this lab is NOT about the code
**Hard part is to figure out how to make compiler and system run well together (like finely oiled machine)

Simple Example:
	The compiler optimization flags (on his code made diff of about 4x immediately) without any code changes
		Part of Assgn is to read about those flags
	Another thing is the compiler can pick up multiple arithmetic ops and if they have certain features can combine into single op
		Ex: most processors know how to do mult and add in one instruction called infusemultiplyadd
				a = b +c *d
				if you do this, you can count one addition and one multiplication
				the compiler will see that they can be combined
				if you have your variables in registers or cache, they can happen in one cycle
	Read what kind of artihmetic to give in order to optimize
	Example of what NOT to do:
		a = b+c
		d = a+b

Start with smallest thing you could do
	-Run as is
	-Only need 2-3 lines of code to complete the coding part of assgn
	-Read wikipedia article and read about compiler flags
*/
