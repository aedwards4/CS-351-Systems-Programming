#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    exit(1);
  }

  //open file for reading
  FILE *fp;

  if(fp = fopen(argv[1], "r")){
    int c;
    char lett;
    int rc;

    while(1){

      rc = fread(&c, sizeof(int), 1, fp);

      if (rc <= 0){
        break;
      }

      rc = fread(&lett, sizeof(char), 1, fp);

      for(int i = 0; i<c; i++){
        fputc(lett, stdout);
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
