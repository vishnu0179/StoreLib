#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include "interface/memory.h"

#define MALLOC_ERROR_MESSAGE(location, reason) ("in " location ":\n" \
                                                reason ", returning NULL\n")



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
    printf(MALLOC_ERROR_MESSAGE("createMatrix",
                                "unable to allocate space to data matrix"));
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
    printf(MALLOC_ERROR_MESSAGE("createMatrix",
                                "unable to allocate space to data matrix"));
    return NULL;
  }

  return matrix;
}



/* memory *initializeMemory(uint64_t wordSize, uint64_t totalLocations):
 * Takes wordSize and total no. of locations of memory and returns a
 * memory pointer pointing towards the data. Returns NULL if unable
 * to allocate space for the pointer.
 * WARNING: giving large values can make program run like a snail, and take
 * for integer to unsigned integer conversions.
 */
memory *initializeMemory(uint64_t wordSize, uint64_t totalLocations){
  memory *memoryPointer = malloc(sizeof(memory));
  if (memoryPointer == NULL){
    printf(MALLOC_ERROR_MESSAGE("initializeMemory",
                                "Unable to allocate memory to object"));
    return NULL;
  }

  (*memoryPointer).matrix = createMatrix(wordSize, totalLocations);
  if ((*memoryPointer).matrix == NULL){
    printf(MALLOC_ERROR_MESSAGE("initializeMemory",
                                "unable to allocate space to data matrix"));
    return NULL;
  }

  (*memoryPointer).wordSize = wordSize;
  (*memoryPointer).totalLocations = totalLocations;
  (*memoryPointer).set = true;

  return memoryPointer;
}
