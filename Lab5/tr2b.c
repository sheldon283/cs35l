/*
Sheldon Dong
004784870 

tr2b.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

void checkError() {
  if (ferror(stdin) != false) {
    fprintf(stderr,"Error: Unable to process stdin\n");
    exit(1);
  }
}

int main(int argc, char* argv[]){
  if (argc != 3) {
    printf("Error: incorrect number of operands\n");
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];

  int lenFrom = strlen(from);
  int lenTo = strlen(to);

  if (lenFrom != lenTo){
    fprintf(stderr, "Operands must be the same size\n");
    exit(1);
  }

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
  
  char current = getchar();
  checkError();

  int bool = false;

  while (current != EOF) {
    size_t k = 0;
    while(k < lenFrom) {
      if (from[k] == current) {
	putchar(to[k]);
	bool = true;
	break;
      }
      else
	bool = false;

      k++;
    }     

    if (!bool)
      putchar(current);
    
    current = getchar();
  }

  return 0;
}
