#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int del(char *argv[]){
  int size = strlen(argv[2]);

  while (1){
    char lett = fgetc(stdin);

    if (lett == EOF){
      break;
    }

    bool isIn=false;
    for (int i = 0; i < size; i++){
      if (argv[2][i] == lett){
        isIn=true;
      }
    }

    if (isIn==false){
      fputc(lett,stdout);
    }

  }
  return 0;
}


int rep(char *argv[]){

  //if provided string arguments are not the same length,
  //exit with error
  if (strlen(argv[1]) != strlen(argv[2])){
    printf("STRING1 and STRING2 must have the same length\n");
    exit(1);
  }

  int size = strlen(argv[1]);

  while (1){
    char c = fgetc(stdin);

    if (c == EOF){
      break;
    }

    for (int i = 0; i < size; i++){
      if (i==(size-1) && c!=argv[1][i]){
        fputc(c,stdout);
      }
      if (c == argv[1][i]){
        fputc(argv[2][i],stdout);
        break;
      }
    }
  }
  return 0;
}


int main(int argc, char *argv[]) {

  //if too many arguments were provided,
  //exit with error
  if (argc < 3) {
    printf("Usage: %s STRING1 STRING2\n", argv[0]);
    printf("       %s -d STRING\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[1], "-d") == 0){
    del(argv);
  }
  else {
    rep(argv);
  }

  return 0;
}
