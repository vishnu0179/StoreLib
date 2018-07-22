#include "store/storeutil.h"

#include <stdbool.h>
#include <stdint.h>

#define ERROR_MESSAGE(location,reason,reaction) ("\nin " location ":\n"\
                                                 reason ", " reaction "\n")
#define WRITE_ERROR(reason,reaction) ERROR_MESSAGE("writeMultiBitstoStore",\
                                                   reason,reaction)

/* writeMultiBitstoStore (store STORE, const uint64_t location,
                          const uint64_t wordStartBit, const boll bitArray[],
                          size_t length):
 * Takes array of bool integers, size of bool arrray, location in store
 * object, and the starting bit in the word to write bool integer array
 * as 'wordStartBit'.
 * Truncates the bool Array if the size provided and wordStartBit provided
 * exceeds the size of the word of STORE object.
 * Returns -1 if STORE object is not initialized, location is greater than
 * equal to totalLocations of STORE object, wordStartBit is greater than
 * equal to STORE's wordSize.
 * Returns 0 if write operation successful , even after truncating the array.
 * WARNING: Take care for keeping bitArray's actual length greater than the
 * 'uint64_t length' provided as parameter in the function.
 */
int writeMultiBitstoStore (store STORE, const uint64_t location,
                           const uint64_t wordStartBit, const bool bitArray[],
                           uint64_t length){
  if (!STORE.set){
    printf(WRITE_ERROR("STORE is not formally initialized yet",\
                       "returning -1"));
    return -1;
  }

  if(location >= STORE.totalLocations){
    printf(WRITE_ERROR("requested location %d out of bound",\
                       "returning -1"), location);
    return -1;
  }

  if(wordStartBit >= STORE.wordSize){
    printf(WRITE_ERROR("requesting start bit in word %d out of bound",\
                       "returning -1"), wordStartBit);
    return -1;
  }

  uint64_t appliedLength = length;
  /* STORE.wordSize - wordStartBit can't overflow as
   * STORE.wordSize > wordStartBit, thanks to the checking of the same
   * previously.*/
  if((STORE.wordSize - wordStartBit) < length){
    printf(WRITE_ERROR("WARNING: bitArray length is too big to fit in word",\
                       "truncating the bitArray"));
    appliedlength = STORE.wordSize - wordStartBit;
  }

  for (uint64_t index = 0; index < appliedlength; index++){
    writeBittoStore(STORE, location, wordStartBit + index, bitArray[index]);
  }

  return 0;
}
