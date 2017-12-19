/*
Sheldon Dong
004784870 

tr2u.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define true 1
#define false 0

void checkError() {
  if (ferror(stdin) != false) {
    fprintf(stderr,"Error: Unable to process stdin\n");
    exit(1);
  }
}

int main(int argc, char* argv[]){


  const char* from = argv[1];
  const char* to = argv[2];

  if (argc != 3) {
    fprintf(stderr,"Error: incorrect number of operands\n");
    exit(1);
  }

  int lenFrom = strlen(from);
  int lenTo = strlen(to);
  size_t i;
  size_t j;

  for (i = 0; i < lenFrom; i++){
    for (j = i+1; j < lenFrom; j++){
      if (from[i] == from[j]) {
	fprintf(stderr, "Found duplicate bytes in From\n");
	exit(1);
      }
    }
  }

  if (lenFrom != lenTo){
    fprintf(stderr, "Operands must be the same size\n");
    exit(1);
  }
  
  char current[1];

  int bool = false;

  ssize_t status;

  status = read(STDIN_FILENO,current,1);

  while (status > 0) {
    size_t k = 0;
    while(k < lenFrom) {
      if (from[k] == current[0]) {
	current[0] = to[k];
	write(STDOUT_FILENO,current,1);
	bool = true;
	break;
      }
      else
	bool = false;

      k++;
    }     

    if (!bool)
      write(STDOUT_FILENO,current,1);

    status = read(STDIN_FILENO,current,1);
  }

  return 0;
}
