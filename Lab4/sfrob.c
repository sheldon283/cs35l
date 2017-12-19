/*Sheldon Dong
  004784870

  sfrob.c
*/

#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

int frobcmp (char const* a, char const* b) {
  //  printf("%s","hello\n");
  //  printf("%c",*a,"\n");
  //  printf("%c",*b,"\n");
  while(true) {
    if (*a == ' ' && *b == ' ')
      return 0;
    else if (*a == ' ' || ((*a^42) < (*b^42)))
      return -1;
    //    else if ((*a^42) < (*b^42))
    //      return -1;
    else if (*b == ' ' || ((*a^42) > (*b^42)))
      return 1;
    //    else if ((*a^42) > (*b^42))
    //      return 1;

    a++;
    b++;
    }
}

void checkError() {
  if (ferror(stdin) != false) {
    fprintf(stderr, "Error: Unable to process stdin\n");
    exit(1);
  }
}

int container(const void* item1, const void* item2){
  return frobcmp(*(const char**)item1, *(const char**)item2);
}

void memError() {
  fprintf(stderr, "Error: Memory Allocation");
  exit(1);
}

int main(void) {
  char* currentWord = (char*)malloc(sizeof(char));
  char** allWords = (char**)malloc(sizeof(char*));

  char current, next;
  current = getchar();
  checkError();
  next = getchar();
  checkError();
  int letterIndex = 0;
  int wordIndex = 0;

  while (current != EOF && !ferror(stdin)) {
    currentWord[letterIndex] = current;

    char* temporary = realloc(currentWord, (letterIndex+2)*sizeof(char));
    if (temporary == NULL) {
      free(currentWord);
      /*      fprintf(stderr, "Error: Memory Allocation");*/
      memError();
      exit(1);
    }
    else
      currentWord = temporary;




    if (current == ' ') {
      allWords[wordIndex] = currentWord;
      char** temporary = realloc(allWords, (wordIndex+2)*sizeof(char*));
      if (temporary == NULL) {
	free(allWords);
	/*	fprintf(stderr,"Error: Memory Allocation");*/
	memError();
	exit(1);
      }
      else {
	allWords = temporary;
	wordIndex++;
	currentWord = NULL;
	currentWord = (char*)malloc(sizeof(char));
	letterIndex = -1;
      }
      //    }
    
      // if (current == ' '){
      if (next == EOF) 
	break;
      else if (next == ' '){
	while (current == ' '){
	  current = getchar();
	  checkError();
	}
	next = getchar();
	checkError();
	letterIndex++;
	continue;
      }
    }
    else if (next == EOF){
      current = ' ';
      letterIndex++;
      continue;
    }


    current = next;
    next = getchar();
    checkError();
    letterIndex++;


  }


  qsort(allWords, wordIndex, sizeof(char*), container);

  size_t i = 0;
  size_t j = 0;
  for (i = 0; i < wordIndex; i++){
    for (j = 0; ; j++){
      if(putchar(allWords[i][j]) == EOF) {
	fprintf(stderr, "Error while writing character!");
	exit(1);
      }
      if (allWords[i][j] == ' ')
	break;
    }
  }
  
  size_t k = 0;
  for (k = 0; k < wordIndex; k++)
    free(allWords[k]);

  free(allWords);
  free(currentWord);
  exit(0);

}
