#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <wait.h>
#include <unistd.h>


#define MSG "* running fileio %s workload with %s blocks and %s process(es)\n"

#define USAGE "usage: ./fileio <workload> <block_size> <num_procs> <flag>\n" \
"     - workload: WS / WR / RS / RR \n" \
"     - block_size: 4KB, 64KB, 1MB, 64MB \n" \
"     - num_procs: 1 / 2 / 4 / 8 \n" \
"     - WS = write-sequential \n" \
"     - WR = write-random \n" \
"     - RS = read-sequential \n" \
"     - RR = read-random \n" \
"     - flag (optional): -O = return in Ops/sec \n"

int main(int argc, char **argv)
{
    //Error checking - Num of Args
    if (argc < 4) {
        printf(USAGE);
        exit(1);
    } else {
        printf(MSG, argv[1], argv[2], argv[3]);
    }

    //Setting up variables
    int fd, i, upper, block_size, file_size, num_procs, num_ops, status;
    pid_t pid;
    ssize_t bytes_read = 0;
    double time, opsPerSec, mbPs;
    const char *files[8];
    struct timeval start_time, end_time;
    num_ops = 0;

    //Setting block_size variable
    if(strcmp(argv[2],"4KB")==0){
      block_size = 4096;
    }
    else if(strcmp(argv[2],"64KB")==0){
      block_size = 65536;
    }
    else if(strcmp(argv[2],"1MB")==0){
      block_size = 1048576;
    }
    else if(strcmp(argv[2],"16MB")==0){
      block_size = 16777216;
    }
    //Error checking - valid block size
    else{
      printf(USAGE);
      exit(1);
    }


    //if desired output is Ops/sec
    if (argc == 5 && strcmp(argv[4],"-O")==0){
      if(strcmp(argv[3],"1")==0){
        num_procs = 1;

        files[0] = "file4mb";

        file_size = 4194304;
      }
      else if(strcmp(argv[3],"2")==0){
        num_procs = 2;

        files[0] = "file2mb-0";
        files[1] = "file2mb-1";

        file_size = 2097152;
      }
      else if (strcmp(argv[3],"4")==0){
        num_procs = 4;

        files[0] = "file1mb-0";
        files[1] = "file1mb-1";
        files[2] = "file1mb-2";
        files[3] = "file1mb-3";

        file_size = 1048576;
      }
      else if (strcmp(argv[3],"8")==0){
        num_procs = 8;

        files[0] = "file500kb-0";
        files[1] = "file500kb-1";
        files[2] = "file500kb-2";
        files[3] = "file500kb-3";
        files[4] = "file500kb-4";
        files[5] = "file500kb-5";
        files[6] = "file500kb-6";
        files[7] = "file500kb-7";

        file_size = 512000;
      }

    }//end of Ops/sec files
    else{
      //Assign num_procs and setup file arrays
      if(strcmp(argv[3],"1")==0){
        num_procs = 1;

        files[0] = "file1gb";

        file_size = 1073741824;
      }
      else if(strcmp(argv[3],"2")==0){
        num_procs = 2;

        files[0] = "file500-0";
        files[1] = "file500-1";

        file_size = 524288000;
      }
      else if (strcmp(argv[3],"4")==0){
        num_procs = 4;

        files[0] = "file250-0";
        files[1] = "file250-1";
        files[2] = "file250-2";
        files[3] = "file250-3";

        file_size = 262144000;
      }
      else if (strcmp(argv[3],"8")==0){
        num_procs = 8;

        files[0] = "file125-0";
        files[1] = "file125-1";
        files[2] = "file125-2";
        files[3] = "file125-3";
        files[4] = "file125-4";
        files[5] = "file125-5";
        files[6] = "file125-6";
        files[7] = "file125-7";

        file_size = 131072000;
      }
    }//end of MB/s files

    num_ops = file_size/block_size;


    //Setting up buffer
    char *buf = aligned_alloc(512, sizeof(char)*block_size);

    //Get start time
    gettimeofday(&start_time, NULL);

    //timer_s = clock();
    //printf("Start timestamp: %ld\n",(timer_s/CLOCKS_PER_SEC));

    //Multiple processes
    //if(num_procs >= 1){

    //Create num_procs number of children
    for(i=0; i<num_procs; i++){
      pid = fork();

      //Child process
      if(pid == 0){

        //open the files
        fd = open(files[i], O_RDWR, O_DIRECT);

        //Run corresponding function
        if(strcmp(argv[1],"WS")==0){

          while(bytes_read < file_size){

            //Write sequential
            bytes_read += write(fd, buf, block_size);
            //printf("Bytes_read: %ld\n", bytes_read);

          } //end of while loop
        }//end of WS

        else if(strcmp(argv[1],"WR")==0){
          upper = file_size - block_size;
          i = 0;

          while(i < num_ops){

            //Generate random number
            int num = (rand() % (upper + 1));

            //Seek to random location
            lseek(fd,num,SEEK_SET);

            //Write random
            bytes_read += write(fd, buf, block_size);

            i++;
          } //end of while loop
        }//end of WR

        else if(strcmp(argv[1],"RS")==0){
          while(bytes_read < file_size){

            //read sequential
            bytes_read += read(fd, buf, block_size);
          } //end of while loop
        }//end of RR

        else if(strcmp(argv[1],"RR")==0){
          upper = file_size - block_size;

          i = 0;

          while(i < num_ops){

            //Generate random number
            int num = (rand() % (upper + 1));

            //Seek to random location
            lseek(fd,num,SEEK_SET);

            //Write random
            bytes_read += read(fd, buf, block_size);

            i++;
          } //end of while loop
        }//end of RR

        //Close each file
        close(fd);
        exit(0);
      } // end of if

    } //end of for loop

    //Parents waiting for children
    for(i=0; i<num_procs;i++){
        waitpid(pid, &status, WUNTRACED | WCONTINUED);
    }

  //}

    //Get ending timestamp
    gettimeofday(&end_time, NULL);

    //Free the allocated memory
    free(buf);

    //Get time in seconds
    time = (double)(end_time.tv_sec - start_time.tv_sec) + (double) (end_time.tv_usec - start_time.tv_usec)/1000000;

    //output is Ops/sec
    if (argc == 5 && strcmp(argv[4],"-O")==0){

      //Compute Ops/sec
      opsPerSec = (num_ops*num_procs)/time;

      //Output result
      printf("%f Ops/sec\n",opsPerSec);
    }
    //output MB/s
    else{
      //Compute MB/s
      mbPs = ((file_size*num_procs)/1024/1204)/time;

      //Output result
      printf("%f MB/sec\n",mbPs);
    }

    printf("\n");
    return 0;
}//end of main
