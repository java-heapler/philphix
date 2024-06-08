#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */

#include <string.h>

unsigned int rotateLeft(unsigned int result, int i) { // https://stackoverflow.com/questions/10134805/bitwise-rotate-left-function
    return (result << i) | (result >> (32 - i)) & ~(-1 << i); // shifts bits to left and brings others "pushed out" to the other side
}

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  unsigned int bucketLocation = table->hashFunction(key); // found hashcode by inserting key into hashFunction
  bucketLocation = bucketLocation % (table->size)+1; // ensures index will not be greater than the table size
   // 2. Allocate a new hash bucket entry struct.
  // printf("bucketLocation: %u", bucketLocation);
  // printf("%u", (int)table->buckets[bucketLocation]); table->buckets[bucketLocation]
  HashBucketEntry *newHashBucketEntry = malloc(sizeof(struct HashBucketEntry *));
  newHashBucketEntry->key = key;
  newHashBucketEntry->data = data;
  newHashBucketEntry->next = NULL;
  // 3. Append to the linked list or create it if it does not yet exist.
  if (table->buckets[bucketLocation] == NULL) {
    table->buckets[bucketLocation] = newHashBucketEntry; // adds newHashBucketEntry to start the list
  }
  else if (table->buckets[bucketLocation] != NULL) {
    HashBucketEntry *llIterate = table->buckets[bucketLocation]; // pointer to current node assigned as next node
    while (llIterate->next) { // iterates as long as next node is not null
      llIterate = llIterate->next; // changes current pointer address to next pointer in list
    }
    llIterate->next = newHashBucketEntry; // adds newHashBucketEntry to end of linkedlist
  }
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  unsigned int bucketLocation = table->hashFunction(key); // found by inserting key into hashFunction
  bucketLocation = bucketLocation % (table->size)+1; // ensures index will not be greater than the table size
  // 2. Walk the linked list and check for equality with table->equalFunction.
  HashBucketEntry *linkedListWalk = table->buckets[bucketLocation];
  while(linkedListWalk) {
    if (table->equalFunction(key, linkedListWalk->key)) { // if the given key and our list's key are equal
      return linkedListWalk->data; // return the data from the HashBucketEntry with the matching key
    }
    else {
      linkedListWalk = linkedListWalk->next; // turn the linkedListWalk's head into its tail
    }
  }
  return NULL;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
  // -- TODO --
  // fprintf(stderr, "need to implement stringHash\n");
  /* To suppress compiler warning until you implement this function, */
  /* I attempted to employ Jerry Coffin's approach which is located at:
   * https://stackoverflow.com/questions/7666509/hash-function-for-string/7666668#7666668
   * It emphasizes making the character input have the heaviest influence on the output as possible */
  char *sToCharPtr = s; // casts string to character pointer (make sure to double-check)
  unsigned int result = 0x55555555; // all even bits set as 1, all odd bits set as 0
  while (*sToCharPtr) { // iterates through the character pointer till we reach null
     result ^= *sToCharPtr++; // result will equal itself or previous value of *sToCharPtr
     result = rotateLeft(result, 5); // rotate current result by 7 bits (not even multiple of byte size)
  }
  return result; // returns the unsigned int result of the hash function
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  // fprintf(stderr, "You need to implement stringEquals");
  /* To suppress compiler warning until you implement this function */
  char *char1 = s1; // converts s1 to character pointer (conversion is implied)
  char *char2 = s2; // converts s2 to character pointer (conversion is implied)
  char c1 = *char1; // first element of char ptr 1
  char c2 = *char2; // first element of char ptr 2
  while (c1 && c2) { // compares until we've reached null terminator for char1 or char2
    if (c1 != c2) {
        return 0;
    }
    c1 =*++char1; // makes c1 the next value in the char1 linkedlist
    c2 =*++char2;
  }
  if (c1 == NULL && c2 == NULL) { // if we've reached the end of both strings, they are the same
    return 1;
  }
  return 0;
}