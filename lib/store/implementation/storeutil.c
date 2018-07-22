#include "store/storeutil.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "store/store.h"

#define ERROR_MESSAGE(location,reason,reaction) ("\nin " location ":\n"\
                                                 reason ", " reaction "\n")
#define STORE_ERROR(reason) ERROR_MESSAGE("checkStore",\
                                          reason,"returning -1")

int checkStore(const store STORE, const uint64_t location,
               const uint64_t wordStartBit){
  if (!STORE.set){
    printf(STORE_ERROR("STORE not initialized"));
    return -1;
  }
  if (location >= STORE.totalLocations){
    printf(STORE_ERROR("Requested location %"PRIu64" out of bound"), location);
    return -1;
  }
  if (wordStartBit >= STORE.wordSize){
    printf(STORE_ERROR("Requested 'bit in word'%"PRIu64" out of bound"),
           wordStartBit);
    return -1;
  }

  return 0;
}


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


  if (checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("writeMultiBitstoStore", "checkStore returned -1",
                         "returning -1"));
    return -1;
  }

  uint64_t appliedLength = length;
  /* STORE.wordSize - wordStartBit
   * can't overflow as STORE.wordSize > wordStartBit,
   * thanks to the checking of the same previously.*/
  if((STORE.wordSize - wordStartBit) < length){
    printf(ERROR_MESSAGE("writeMultiBitstoStore",
                         "WARNING: bitArray length is too big to fit in word",
                         "truncating the bitArray"));
    appliedLength = STORE.wordSize - wordStartBit;
  }

  /* bitArray is stored in store such that, when bit array is converted
   * into decimal number, highest array index stores most significant bit,
   * whereas lowest array index stores least significant bit.
   * Loop is designed with that in mind.*/
  for (uint64_t index = 0; index < appliedLength; index++){
    writeBittoStore(STORE, location, wordStartBit + index,
                    bitArray[appliedLength - index - 1]);
  }

  return 0;
}



bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit, uint64_t length){

  if(checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("readMultiBitsfromStore","checkStore returned -1",
                         "returning NULL"));
    return NULL;
  }

  uint64_t appliedLength = length;
  /* STORE.wordSize - wordStartBit can't overflow as
   * STORE.wordSize > wordStartBit, thanks to the checking of the same
   * previously.*/
  if((STORE.wordSize - wordStartBit) < length){
    printf(ERROR_MESSAGE("readMultiBitsfromStore",
                         "WARNING: Word length too small to fit in bitArray",
                         "storing only available bits in bitArray"));
    appliedLength = STORE.wordSize - wordStartBit;
  }

  /* bitArray stores value(bits) such that, when bit array is converted
   * into decimal number, highest array index stores most significant bit,
   * whereas lowest array index stores least significant bit.
   * Loop is designed with that in mind.*/
  bool *bitArray = (bool*)malloc(sizeof(bool)*appliedLength);
  for (uint64_t index = 0; index < appliedLength; index++){
    bitArray[appliedLength - index - 1] =
     readBitfromStore(STORE, location, wordStartBit + index);
  }

  return bitArray;
}


bool *numbertoBitString(uint64_t number, uint64_t length){
  uint64_t actionNumber = number;
  bool *bitString = malloc(sizeof(bool)*length);
  for (uint64_t i = 0; i < length; i++){
      bitString[i] = actionNumber % 2;
      actionNumber /= 2;
  }

  return bitString;
}

int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length){
  int checkResult = checkStore(STORE, location, wordStartBit);
  if(checkResult){
    printf(ERROR_MESSAGE("writeNumBitstoStore", "checkStore returned -1",
                         "returning -1"));
    return -1;
  }

  uint64_t availableBits = (length <= (STORE.wordSize - wordStartBit))?
                            length: (STORE.wordSize - wordStartBit);
  if (availableBits < 64 && number >= pow(2, availableBits)){
      printf(ERROR_MESSAGE("writeNumBitstoStore",
                           "WARNING: given number is bigger than"
                           "requested bit length",
                           "Truncating the number!"));
      number %= (uint64_t)pow(2, availableBits);
  }

  bool *bitArray = numbertoBitString(number, availableBits);
  writeMultiBitstoStore (STORE, location, wordStartBit, bitArray,
                         availableBits);
  return 0;
}
