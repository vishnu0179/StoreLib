#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include "interface/memory.h"

#define ERROR_MESSAGE(location,reason,returnValue) ("\nin" location\
                                                    ":\n" reason ", returning "\
                                                    returnValue "\n")
#define INITIALIZE_ERROR_MESSAGE(reason) ERROR_MESSAGE("initializeMemory",\
                                                        reason,\
                                                       "uninitiated memory")
#define WRITE_ERROR_MESSAGE(reason) ERROR_MESSAGE("writeBittoMemory",\
                                                   reason,\
                                                   "-1")
#define READ_ERROR_MESSAGE(reason) ERROR_MESSAGE("readBitfromMemory",\
                                                  reason,\
                                                 "-1")




/* bool **createMatrix(uint64_t totalRows, uint64_t totalColumns):
 * Takes total number of rows and columns, and returns a pointer
 * pointing to two dimensional bool array. Returns NULL if unable to
 * allocate memory to any pointer.
 * WARNING: giving large values can make program run like a snail, and take
 * for integer to unsigned integer conversions.
 */
bool **createMatrix(uint64_t totalRows, uint64_t totalColumns){
  bool **matrix = (bool**)malloc(sizeof(bool*) * totalColumns);
  if(matrix == NULL){
    printf(ERROR_MESSAGE("createMatrix",
           "unable to allocate space to data matrix", "NULL"));
    return NULL;
  }

  bool NULLcheck = false;
  for (uint64_t index = 0; index < totalRows; index++){
    matrix[index] = (bool*)malloc(sizeof(bool) * totalColumns);
    if(matrix[index] == NULL){
      NULLcheck = true;
      break;
    }
  }
  if (NULLcheck == true){
    printf(ERROR_MESSAGE("createMatrix",
           "unable to allocate space to data matrix", "NULL"));
    return NULL;
  }

  return matrix;
}

/* memory initializeMemory(uint64_t wordSize, uint64_t totalLocations):
 * Takes wordSize and total no. of locations of memory.
 * Returns a memory object and set 'set' bit to 1.
 * Returns uninitiated memory object i.e. all value = 0/NULL, if unable
 * to allocate space for the matrix pointer.
 * WARNING: giving large values can make program run like a snail, and take
 * care for integer to unsigned integer conversions.
 */
memory initializeMemory(const uint64_t totalLocations,
                        const uint64_t wordSize){
  memory MEMORY;

  MEMORY.matrix = createMatrix(wordSize, totalLocations);
  if (MEMORY.matrix == NULL){
    printf(INITIALIZE_ERROR_MESSAGE("unable to allocate space to data matrix"));
    return MEMORY;
  }

  MEMORY.wordSize = wordSize;
  MEMORY.totalLocations = totalLocations;
  MEMORY.set = true;

  return MEMORY;
}

/* bool writeBittoMemory (memory givenMemory, const uint64_t location,
                          const uint64_t bitinWord, const bool value):
 * Takes a memory object, location in the matrix with row no. = location,
 * column no. = bitinWord, and the value to be written = value.
 * Returns 0 if written successfully, else returns -1.
 * WARNING: Take care of the integer typecasting while
 * calling the function.
 */
int writeBittoMemory (memory givenMemory, const uint64_t location,
                      const uint64_t bitinWord, const bool value){

  if (!givenMemory.set){
    printf(WRITE_ERROR_MESSAGE("givenMemory isn't formally initialized yet"));
    return -1;
  }

  if (location >= givenMemory.totalLocations){
    printf(WRITE_ERROR_MESSAGE("requested 'location' %d out of bound"),
           location);
    return -1;
  }

  if (bitinWord >= givenMemory.wordSize){
    printf(WRITE_ERROR_MESSAGE("requested 'bit in Word' %d out of bound"),
           bitinWord);
    return -1;
  }

  givenMemory.matrix[location][bitinWord] = value;

  return 0;
}

/* int readBitfromMemory (const memory givenMemory, const uint64_t location,
                          const uint64_t bitinWord):
 * takes memory type object, location number, and bit number in word.
 * Returns value stored at that memory location if successful,
 * else returns -1.
 */
int readBitfromMemory (const memory givenMemory, const uint64_t location,
                       const uint64_t bitinWord){

  if(!givenMemory.set){
    printf(READ_ERROR_MESSAGE("givenMemory is not formally initialized yet"));
    return -1;
  }

  if (location >= givenMemory.totalLocations){
    printf(READ_ERROR_MESSAGE("given 'location' %d is out of bound"),
              location);
    return -1;
  }

  if (bitinWord >= givenMemory.wordSize){
    printf(READ_ERROR_MESSAGE("given 'bit in word' %d is out of bound"),
           bitinWord);
    return -1;
  }

  bool value = givenMemory.matrix[location][bitinWord];

  return value;
}


/* size_t sizeofMemory(memory MEMORY):
 * Takes memory object.
 * Returns size of the matrix stored in the object, returns 0
 * if the object is not initialized.
 */
size_t sizeofMemory(memory MEMORY){
  if (!MEMORY.set){
    printf(ERROR_MESSAGE("sizeofMemory", "memory is not formally initialized",\
                         "0"));
    return 0;
  }

  size_t memorySize = MEMORY.totalLocations * MEMORY.wordSize;

  return memorySize;
}
