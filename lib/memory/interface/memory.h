#ifndef LIB_MEMORY_INTERFACE_MEMORY
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

memory *initializeMemory(uint64_t wordSize, uint64_t totalLocations);
#endif
