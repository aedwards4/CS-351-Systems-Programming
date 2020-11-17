#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    exit(1);
  }

  FILE *fp;

  //open file for reading
  if (fp = fopen(argv[1], "r")){

    int count;
    char c = fgetc(fp);

    while(1){

      //c = fgetc(fp);
      count = 1;

      if (c == EOF){
        break;
      }

      //count ++;

      while (1){
        char temp = fgetc(fp);
        if(temp == c){
          count++;
        }
        else{
          fwrite(&count, sizeof(int),1,stdout);
          fputc(c, stdout);
          c = temp;
          break;
        }
      }
    }
  fclose(fp);
  return 0;
  }
  else{
    printf("File does not exist.\n");
    exit(1);
  }
}
