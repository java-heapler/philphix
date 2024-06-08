/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */
void readDictionary(char *dictName) {
  // -- TODO --
  // fprintf(stderr, "You need to implement readDictionary\n");
  FILE *filePtr = fopen(dictName, "r+"); // opens file dictName in read (r) mode, assigns it address to a ptr
  if (filePtr == NULL) {
    exit(61); // exits program if dict doesn't exist
  }
  char *word = (char *) malloc(61 * (size_t)sizeof(char)); // allocated initialized data using calloc;
  char *prevWord = (char *) malloc(61 * (size_t)sizeof(char));
  int wordIndex = 0; // predicts 60 letter words at first
  int wordLength = 60;
  char p = fgetc(filePtr);
  while (p != EOF) { // checks to see if we've reached end of file
      if (p == ' ' || p == '\t') { // checking whether the line has a space or tab
          if (strlen(word) != 0) {
              for (int i = 0; i<wordIndex; i++) {
                  if (!isalnum(word[i])) {
                      exit(61);
                  }
              }
              word[wordIndex] = '\0'; // null character indicates the end of a word
              strcpy(prevWord, word); // copies the key to correct pos for input into insertdata
              wordIndex = 0;
              word = (char *) malloc((size_t) wordLength+1 * (size_t)sizeof(char));
          }
      }
      else if (p == '\n') { // goes to next line if we've reached the end
          for (int i = 0; i<wordIndex; i++) {
              if (word[i] == ' ' || word[i] == '\t') {
                  exit(61);
              }
          }
          word[wordIndex] = '\0'; // null character indicates the end of a word
          insertData(dictionary, prevWord, word);
          wordIndex = 0;
          word = (char *) malloc((size_t) wordLength+1 * (size_t)sizeof(char));
          prevWord = (char *) malloc((size_t) wordLength+1 * (size_t)sizeof(char));
      }
      else {
          if (wordIndex + 5 >= wordLength - 1) { // checks to see if our length exceeds wordlength
              word = (char *) realloc(word, (size_t) ((wordLength + 1) * 2 * sizeof(char)));
              prevWord = (char *) realloc(prevWord, (size_t) ((wordLength + 1) * 2 * sizeof(char)));
              wordLength = wordLength * 2; // doubles word length allocation
          }
          word[wordIndex++] = p; // converts our letter to char (we needed int for EOF)
          }
      p = fgetc(filePtr);
  }
  if (p = EOF) {
      if (strlen(word) != 0 && wordIndex !=0) {
          for (int i = 0; i < wordIndex; i++) {
              if (word[i] == ' ' || word[i] == '\t') {
                  exit(61);
              }
          }
          word[wordIndex] = '\0';
          insertData(dictionary, prevWord, word);
      }
  }
}

/* Task 4 */
void processInput() {
    // -- TODO --
    // fprintf(stderr, "You need to implement processInput\n");
    char p = fgetc(stdin);
    char *word = (char *) malloc(61 * (size_t) sizeof(char));  // allocated initialized data using calloc
    char *wordCopy = (char *) malloc(61 * (size_t) sizeof(char));
    int wordIndex = 0;
    int wordLength = 60;
    while (p != EOF) { // checks to see if we've reached end of file
        if (!isalnum(p)) { // checks if p is not alphanumeric
            if (strlen(word)==0) { // we print the nonalphanumeric character if we have no word in the buffer to evaluate
                printf("%c", p);
            } else {
                word[wordIndex] = '\0';
                strcpy(wordCopy, word);
                int try = 2;
                while (!findData(dictionary,word) && try<4) {
                    if (try == 2) {
                        for (int i = 1; i < strlen(word); i++) { // makes all letters except first lowercase in word
                            word[i] = tolower(word[i]);
                        }
                    }
                    else if (try == 3) {
                        for (int i = 0; i < strlen(word); i++) { // makes all letters lowercase
                            word[i] = tolower(word[i]);
                        }
                    }
                    try++;
                }
                char * foundWord;
                if (findData(dictionary,word)) {
                    foundWord = findData(dictionary,word);
                    fprintf(stdout, (foundWord));
                } else { // adds unedited word to output is if its edited or unedited forms aren't found in dict
                    fprintf(stdout, wordCopy);
                }
                wordIndex = 0;
                printf("%c", p);
            }
        } else {
            if (wordIndex+5 >= wordLength-1) { // checks to see if our length exceeds wordlength
                word = (char *) realloc(word, (size_t) ((wordLength+1)*2 * sizeof(char))); // the +1 accounts for '\0' at end
                wordCopy = (char *) realloc(wordCopy, (size_t) ((wordLength+1)*2 * sizeof(char)));
                wordLength = wordLength * 2; // doubles word length allocation
            }
            word[wordIndex++] = p;
        }
     //   if (p == '\n') { // goes to next line if we've reached the end
    //        p = fgetc(stdin);
   //         printf("%c", '\n');
        p = fgetc(stdin);
    }
    if (p = EOF) { // written for special case of no newline at end of file
        if (strlen(word) != 0 && wordIndex !=0) {
            word[wordIndex] = '\0';
            strcpy(wordCopy, word);
            int try = 2;
            while (!findData(dictionary,word) && try<4) {
                if (try == 2) {
                    for (int i = 1; i < strlen(word); i++) { // makes all letters except first lowercase in word
                        word[i] = tolower(word[i]);
                    }
                }
                else if (try == 3) {
                    for (int i = 0; i < strlen(word); i++) { // makes all letters lowercase
                        word[i] = tolower(word[i]);
                    }
                }
                try++;
            }
            char * foundWord;
            if (findData(dictionary,word)) {
                foundWord = findData(dictionary,word);
                fprintf(stdout, (foundWord));
            } else { // adds unedited word to output is if its edited or unedited forms aren't found in dict
                fprintf(stdout, wordCopy);
            }
        }
    }
    free(word);
    free(wordCopy);
   /* FILE *readOut = fopen("Output", "r");
    char c = fgetc(readOut); // prints ouptput file into stdout
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(outputFile);
    } */
}