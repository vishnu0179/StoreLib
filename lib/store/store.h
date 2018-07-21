#ifndef LIB_STORE_STORE_H
#define LIB_STORE_STORE_H
#include <stdbool.h>
#include <stdint.h>

/* struct memory:
 * Data structure to hold data of the memory
 * and info about the status of the particular
 * memory. Data for matrix should be handled
 * separately as structure in c is non dynamic.
 */
typedef struct{
  int wordSize;
  int totalLocations;
  bool set;
  bool **matrix;
}memory;

memory initializeMemory(const uint64_t totalLocations,
                         const uint64_t wordSize);
int writeBittoMemory(memory givenMemory, const uint64_t location,
                      const uint64_t bitinWord, const bool value);
int readBitfromMemory(const memory givenMemory, const uint64_t location,
                      const uint64_t bitinWord);
size_t sizeofMemory(memory givenMemory);
#endif
