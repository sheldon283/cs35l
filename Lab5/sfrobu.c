/*Sheldon Dong
  004784870

  sfrob.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define true 1
#define false 0
#define bool int

//int count = 0;
bool foption = false;

unsigned char deencrypt (char a) {
  char c = a;
  c = c^42;

  if (foption) 
    c = toupper((unsigned char)c);

  return c;
  
}

int frobcmp (char const* a, char const* b) {
  //  printf("%s","hello\n");
  //  printf("%c",*a,"\n");
  //  printf("%c",*b,"\n");
  int aIndex = 0;
  int bIndex = 0;
  while(true) {
    if (*a == ' ' && *b == ' ')
      return 0;
    else if (*a == ' ' || (deencrypt(a[aIndex]) < deencrypt(b[bIndex])))
      return -1;
    //    else if ((*a^42) < (*b^42))
    //      return -1;
    else if (*b == ' ' || (deencrypt(a[aIndex]) > deencrypt(b[bIndex])))
      return 1;
    //    else if ((*a^42) > (*b^42))
    //      return 1;

    a++;
    b++;
    }
}

int container(const void* item1, const void* item2){
  //  count++;
  return frobcmp(*(const char**)item1, *(const char**)item2);
}

void memError() {
  fprintf(stderr, "Error: Memory Allocation\n");
  exit(1);
}

void readError(ssize_t status) {
  if (status < 0) {
    fprintf(stderr,"Read file error");
    exit(1);
  }
}

int main(int argc, char* argv[]) {
 
  if (argc == 3 || argc == 2) {
    if (strcmp(argv[1],"-f") == 0)
      foption = true;
  }
  
  struct stat destination;
  ssize_t status;

  if (fstat(0, &destination) < 0) {
    fprintf(stderr, "fstat Error!");
    exit(1);
  }

  char** words;
  char* all = NULL;
  int wordIndex = 0;

  if (S_ISREG(destination.st_mode)) {
    all = (char*) malloc(destination.st_size * sizeof(char));
    readError(read(STDIN_FILENO, all, destination.st_size));
    //    int pCounter = 0;
    int counter = 0;

    if (read(STDIN_FILENO, all, destination.st_size) > -1) {
      size_t i;
      for (i = 0; i < destination.st_size; i++) {
	if (i == destination.st_size - 1)
	  all[i] = ' ';
	
	if (i == 0 && all[i] != ' ') 
	  counter++;

	if (all[i] == ' ') {
	  while (i < destination.st_size && all[i] == ' ')
	    ++i;
	  
	  if (i < destination.st_size)
	    ++counter;
	}
      }
    }

    bool flag = false;
    
    words = (char**) malloc(counter * sizeof(char*));

    size_t i;
    for (i = 0; i < destination.st_size; i++) {
      if (all[i] == ' ' && flag)
	flag = false;
      else if (all[i] != ' ' && !flag) {
	words[wordIndex] = &all[i];
	++wordIndex;
	flag = true;
      }
    }
      
  }
  else 
    words = (char**) malloc(sizeof(char*));


  int letterIndex = 0;

  char* oneWord = (char*) malloc(1);

  char current, next;

  ssize_t currentStatus = read(STDIN_FILENO, &current, 1);
  readError(currentStatus);

  ssize_t nextStatus = read(STDIN_FILENO, &next, 1);
  readError(nextStatus);

  while (currentStatus > 0) {
    oneWord[letterIndex] = current;

    char* temporary = realloc(oneWord, (letterIndex+2));

    if (temporary == NULL) {
      free(oneWord);
      memError();
      exit(1);
    }
    else
      oneWord = temporary; 

    if (current == ' ') {
        words[wordIndex] = oneWord;
	char** temporaryWords = (char**) realloc(words, (wordIndex+2)*sizeof(char*));

        if (temporaryWords == NULL) {
	  free(words);
	  memError();
	  exit(1);
        }
	else {
	  wordIndex++;
	  oneWord = NULL;
	  oneWord = (char*)malloc(sizeof(char));
	  letterIndex = -1;
	  words = temporaryWords;
	
	}
    }
      
    if (current == ' '){
      if (nextStatus == 0) 
	break;

      else if (next == ' '){
	while (current == ' '){
	  currentStatus = read(STDIN_FILENO, &current, 1);
	  readError(currentStatus);
	}

	nextStatus = read(STDIN_FILENO, &next, 1);
	readError(nextStatus);
	letterIndex++;
	continue;
      }
    }
    else if (nextStatus == 0){
      current = ' ';
      letterIndex++;
      continue;
    }


    current = next;
    nextStatus = read(STDIN_FILENO, &next, 1);
    readError(nextStatus);
    letterIndex++;
  }


  qsort(words, wordIndex, sizeof(char*), container);

  size_t i = 0;
  size_t j = 0;
  for (i = 0; i < wordIndex; i++){
    //    int size = 0;
    for (j = 0; ; j++){
      //size++;
      if (words[i][j] == ' '){
	status = write(STDOUT_FILENO, &words[i][j], 1);
	readError(status);
	break;
      }
      status = write(STDOUT_FILENO, &words[i][j], 1);
      readError(status);      
    }
  }

  if (S_ISREG(destination.st_mode))
    free(all);
  free(words);
  free(oneWord);

  return 0;
  
}
