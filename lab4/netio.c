#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080

#define MSG "* running netio with method %s operation %s for %s number of ops as %s...\n"

#define USAGE "usage: ./netio <method> <operation> <num_ops> <client/server>\n" \
"     - method: function / pipe / socket / rpc \n" \
"     - operation: add / subtract / multiply / divide \n" \
"     - num_calls: 1000 | 1000000 \n" \
"			- client/server: C | S \n"

double multiply(double a, double b)
{
	return a*b;
}

double divide(double a, double b)
{
	if(b == 0.0)
		return 0;
	else
		return a/b;
}

double add(double a, double b)
{
	//printf("add(%f,%f)\n",a,b);

	return a+b;
}

double subtract(double a, double b)
{
	return a-b;
}


int main(int argc, char **argv)
{
	//Setting up variables
	time_t t;
	srand((unsigned) time(&t));
	int method = -1; //used to store what method to test
	int operation = -1; //used to store what operation to test
	int cl_ser = -1; // used to store whether running client or server

	char fn[] = "/tmp/pipe";	//to be used for pipes
	char fn2[] = "/tmp/pipe02";	//to be used for pipes
	int temp, rfd=0, wfd=0;	//to be used for pipes
	double buff[2], readBuf[1], nums[2]; //to be used for pipes
	double num1, num2;

	int server_fd, new_socket, valread, sock=0, opt=1;	//to be used for sockets
	struct sockaddr_in address;	//to be used for sockets
	struct sockaddr_in serv_addr;	//to be used for sockets
	int addrlen = sizeof(address);	//to be used for sockets
	//double buffer[1024] = {0};	//to be used for sockets

	char temp_buffer[128]; //to be used for rpcgen

    if (argc != 5)
    {
        printf(USAGE);
        exit(1);
    }
    else
    {
				//if(strcmp(argv[4],"C")==0){
				//	printf(MSG, argv[1], argv[2], argv[3], argv[4]);
				//}

        if(strcmp(argv[1],"function") == 0)
        	method = 0;
        else if(strcmp(argv[1],"pipe") == 0)
        	method = 1;
        else if(strcmp(argv[1],"socket") == 0)
        	method = 2;
        else if(strcmp(argv[1],"rpc") == 0)
        	method = 3;
        else
        	method = -1;

        if(strcmp(argv[2],"add") == 0)
        	operation = 0;
        else if(strcmp(argv[2],"subtract") == 0)
        	operation = 1;
        else if(strcmp(argv[2],"multiply") == 0)
        	operation = 2;
        else if(strcmp(argv[2],"divide") == 0)
        	operation = 3;
        else
        	operation = -1;

				if(strcmp(argv[4], "C") == 0)
					cl_ser = 0;
				else if (strcmp(argv[4], "S") == 0)
					cl_ser = 1;
				else
					cl_ser = -1;

        int num_ops = atoi(argv[3]);

		struct timeval start, end;

  		gettimeofday(&start, NULL);

		double ret_value = 0.0;

   		switch (method)
     	{
        	case 0:
					printf(MSG, argv[1], argv[2], argv[3], argv[4]);
   				switch (operation)
     			{
        			case 0:
           				for (int i=0;i<num_ops;i++)
           					ret_value = add((double)rand()/RAND_MAX,(double)rand()/RAND_MAX);
           				break;
        			case 1:
           				for (int i=0;i<num_ops;i++)
           					ret_value = subtract((double)rand()/RAND_MAX,(double)rand()/RAND_MAX);
           				break;
        			case 2:
           				for (int i=0;i<num_ops;i++)
           					ret_value = multiply((double)rand()/RAND_MAX,(double)rand()/RAND_MAX);
           				break;
        			case 3:
           				for (int i=0;i<num_ops;i++)
           					ret_value = divide((double)rand()/RAND_MAX,(double)rand()/RAND_MAX);
           				break;
        			default:
           				printf("operation not supported, exit...\n");
           				return -1;
     			}
     			break;
        	case 1:	//pipes

					//Open both pipes
					if ((rfd = open(fn, O_RDWR)) < 0){
						printf("Pipe error for read\n");
						perror("mkfifo() failed");
						exit(2);
					}


					if ((wfd = open(fn2, O_RDWR)) < 0){
						printf("Pipe error for write\n");
						exit(2);
					}

					//Switch to client or server
					switch (cl_ser)
					{
						case 0:
							printf(MSG, argv[1], argv[2], argv[3], argv[4]);
							//CLIENT

							//Perform requested number of operations
							for(int i=0; i < num_ops; i++){

								//Get random numbers
								num1 = (double)rand()/RAND_MAX;
								num2 = (double)rand()/RAND_MAX;

								//Put random numbers in write buffer
								buff[0] = num1;
								buff[1] = num2;

								//write the args
								temp = write(rfd, buff, (sizeof(double)*2));
								if (temp < 0){
									printf("Write error");
									exit(3);
								}

								//read the result into readBuf
								temp = read(wfd, readBuf, sizeof(double));
								if (temp < 0){
									printf("Read error");
									exit(3);
								}

							}	//end of for loop
							break;	//end of client

						case 1:
							//SERVER

							//Perform requested number of operations
							for(int i=0; i < num_ops; i++){

								//read the args
								temp = read(rfd, nums, (sizeof(double)*2));
								if (temp < 0){
									printf("Read error");
									exit(3);
								}

								//perform the function and get return value
								switch (operation)
			     			{
			        			case 0:
											ret_value = add(nums[0], nums[1]);
			         				break;
			        			case 1:
			         				ret_value = subtract(nums[0], nums[1]);
			         				break;
										case 2:
											ret_value = multiply(nums[0], nums[1]);
											break;
										case 3:
											ret_value = divide(nums[0], nums[1]);
											break;
										default:
					           				printf("operation not supported, exit...\n");
					           				return -1;
			     			}

								//write the return value to client
								temp = write(wfd, &ret_value, sizeof(double));
								if (temp < 0){
									printf("Write error");
									exit(3);
								}
							}//end for loop
							break;	//end of server
					}

					//Close the pipes
					close(rfd);
					close(wfd);
					unlink(fn);
					unlink(fn2);

					if(strcmp(argv[4],"S")==0){
						exit(0);
					}

					break; 	//end of pipes

        	case 2: //sockets

					switch (cl_ser) //client/server
					{
						case 0:
							//ClIENT

							//Create socket
							if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
								perror("Error: Socket failed!\n");
								exit(1);
							}

							//Attach socket to port 8080
							if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
								perror("Error: setsockopt\n");
								exit(1);
							}

							address.sin_family = AF_INET;
							address.sin_addr.s_addr = INADDR_ANY;
							address.sin_port = htons(PORT);

							if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
								perror("Error: bind failed!\n");
								exit(1);
							}

							if (listen(server_fd, 3) < 0){
								perror("Error: listen\n");
								exit(1);
							}

							if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
								perror("Error: accept\n");
								exit(1);
							}

							for(int i=0; i<num_ops; i++){

								//Get random numbers
								num1 = (double)rand()/RAND_MAX;
								num2 = (double)rand()/RAND_MAX;

								//Put random numbers in write buffer
								buff[0] = num1;
								buff[1] = num2;

								send(new_socket, buff, (sizeof(double)*2), 0);

								valread = recv(new_socket, readBuf, sizeof(double), 0);

							}
							close(new_socket);
							exit(0);

							break;
						case 1:
							printf(MSG, argv[1], argv[2], argv[3], argv[4]);
							//SERVER

							if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
								printf("Socket creation error\n");
								return -1;
							}

							serv_addr.sin_family = AF_INET;
							serv_addr.sin_port = htons(PORT);

							if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
								printf("Error: Invalid address/Not supported\n");
								return -1;
							}

							if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
								printf("Error: Connection failed\n");
								return -1;
							}

							for(int i=0; i<num_ops; i++){

								valread = recv(sock, nums, (sizeof(double)*2), 0);

								//perform the function and get return value
								switch (operation)
								{
										case 0:
											ret_value = add(nums[0], nums[1]);
											break;
										case 1:
											ret_value = subtract(nums[0], nums[1]);
											break;
										case 2:
											ret_value = multiply(nums[0], nums[1]);
											break;
										case 3:
											ret_value = divide(nums[0], nums[1]);
											break;
										default:
														printf("operation not supported, exit...\n");
														return -1;
								}

								send(sock, &ret_value, sizeof(double), 0);
							}//end of for loop

							break;
					}	//end of client/server
     			break;	//end of sockets

        case 3: //rpcgen
					printf(MSG, argv[1], argv[2], argv[3], argv[4]);

					if (operation==0){
						temp = system("gnome-terminal -- sudo ./add_server");
					}
					else if (operation==1){
						temp = system("gnome-terminal -- sudo ./sub_server");
					}
					else if (operation==2){
						temp = system("gnome-terminal -- sudo ./mult_server");
					}
					else{
						temp = system("gnome-terminal -- sudo ./div_server");
					}

					for(int i=0; i<num_ops; i++){

						//Get random numbers
						num1 = (double)rand()/RAND_MAX;
						num2 = (double)rand()/RAND_MAX;

						//Put random numbers in write buffer
						buff[0] = num1;
						buff[1] = num2;


	   				switch (operation)
						{
							case 0:
								snprintf(temp_buffer, sizeof(temp_buffer), "sudo ./add_client localhost %f %f", buff[0], buff[1]);
								temp = system(temp_buffer);
								break;
							case 1:
								snprintf(temp_buffer, sizeof(temp_buffer), "sudo ./sub_client localhost %f %f", buff[0], buff[1]);
								temp = system(temp_buffer);
								break;
							case 2:
								snprintf(temp_buffer, sizeof(temp_buffer), "sudo ./mult_client localhost %f %f", buff[0], buff[1]);
								temp = system(temp_buffer);
								break;
							case 3:
								snprintf(temp_buffer, sizeof(temp_buffer), "sudo ./div_client localhost %f %f", buff[0], buff[1]);
								temp = system(temp_buffer);
								break;
							default:
											printf("operation not supported, exit...\n");
											return -1;
	     			}//end operation switch
					}//end for loop

     			break;	//end of rpc
        default:
        	printf("method not supported, exit...\n");
           	return -1;
     	}

     	gettimeofday(&end, NULL);

		double elapsed_time_us = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
		printf("==> %f ops/sec\n",(num_ops/elapsed_time_us)*1000000);

    }

    return 0;
}


/*
These will all require a front end and a back end (client and server)
-Use netio.c as client for all
	--Build servers for each method
		---Need to copy the function implementations (add, sub, mult, divide) into server
PIPES (1 file):
-Client and server CAN be separate
-Can also be done with fork() **Not recommended

-Server starts, waits for communication
	--Interprets commuication to determine operation (add, sub, mult, div, or report op not found)
	--Message struct: **Consider fixed sized buffer or binary data structure or custom struct
		---Use command line arg to pass info to function/Server
		---add 1stNum 2ndNum --> 3 argments put into string
		---Send string to Server
		---Server Interprets the string
		---strcmp("add, argv[0]")...etc
		---double(atoi(argv[1]))....etc
		---call function, get return value
		---send return value back to client (result num)
			----error check return message

--instead of performing add, etc, will be doing send() and receive() **Done 1 million times

SOCKETS (1 file):
-Client and server CAN be separate
-Can also be done with fork() **Not recommended

RPC(1+ files):
-Clear sep between client and server
-"Remote Procedure Calls"
	--Help programmer not deal with small details involved in pipes and sockets
-rpcgen tool
	--How to define different functions and types of data to pass in and out
	--Will look similar to calling a local function

*/
