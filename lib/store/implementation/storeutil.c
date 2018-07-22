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


/* int checkStore(const store STORE, const uint64_t location,
               const uint64_t wordStartBit):
 * Takes store object, location in store, bit in Word in store, and
 * checks whether
 *  -store object is initialized,
 *  -location is in bound for store object,
 *  -word bit is in bound for store object.
 * if any of the above is false, RETURNS -1, else RETURNS 0
 */
int checkStore(const store STORE, const uint64_t location,
               const uint64_t wordBit){
  if (!STORE.set){
    printf(STORE_ERROR("STORE not initialized"));
    return -1;
  }
  if (location >= STORE.totalLocations){
    printf(STORE_ERROR("Requested location %"PRIu64" out of bound"), location);
    return -1;
  }
  if (wordBit >= STORE.wordSize){
    printf(STORE_ERROR("Requested 'bit in word'%"PRIu64" out of bound"),
           wordBit);
    return -1;
  }

  return 0;
}


/* writeMultiBitstoStore (store STORE, const uint64_t location,
                          const uint64_t wordStartBit, const bool bitArray[],
                          uint_64 length):
 * Takes store object, location in store object, bit location in a word of
 * store, bool array, length of the array.
 * Truncates the bool Array if (length + wordStartBit) > wordSize of store
 * object.
 * Returns -1 if STORE object is not initialized, location is greater than
 * equal to totalLocations of STORE object, wordStartBit is greater than
 * equal to STORE's wordSize.
 * Returns 0 if write operation successful , even after truncating the array.
 * WARNING: Take care for keeping bitArray's actual length greater than
 * equal to 'uint64_t length' provided as parameter in the function.
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

  /* bitArray is stored in store such that, when store word is converted
   * into decimal number, lowest address word bit stores most significant bit,
   * whereas highest address word bit stores least significant bit.
   * Loop is designed with that in mind.*/
  for (uint64_t index = 0; index < appliedLength; index++){
    writeBittoStore(STORE, location, wordStartBit + index,
                    bitArray[appliedLength - index - 1]);
  }

  return 0;
}


/* bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit,
                              uint64_t numberofBits):
 * Takes store object, location in store, bit location in word of store,
 * length - number of bits to read from store.
 * Reads multiple bits from a word of the store and returns array of bool
 * storing the bits of word from the store.
 * Retuns NULL if checkStore fails.
 */
bool *readMultiBitsfromStore (const store STORE, const uint64_t location,
                              const uint64_t wordStartBit,
                              uint64_t numberofBits){

  if(checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("readMultiBitsfromStore","checkStore returned -1",
                         "returning NULL"));
    return NULL;
  }

  uint64_t avlNumberofBits = numberofBits;
  /* STORE.wordSize - wordStartBit can't overflow as
   * STORE.wordSize > wordStartBit, thanks to the checking of the same
   * previously.*/
  if((STORE.wordSize - wordStartBit) < numberofBits){
    printf(ERROR_MESSAGE("readMultiBitsfromStore",
                         "WARNING: Requested number of bits greater than total"
                         " bits in Word after wordStartBit",
                         "storing only available bits in bitArray"));
    avlNumberofBits = STORE.wordSize - wordStartBit;
  }

  /* bitArray stores value(bits) such that, when bit array is converted
   * into decimal number, highest array index stores most significant bit,
   * whereas lowest array index stores least significant bit.
   * Loop is designed with that in mind.*/
  bool *bitArray = (bool*)malloc(sizeof(bool)*avlNumberofBits);
  for (uint64_t index = 0; index < avlNumberofBits; index++){
    bitArray[avlNumberofBits - index - 1] =
     readBitfromStore(STORE, location, wordStartBit + index);
  }

  return bitArray;
}


/* bool *numbertoBitString(uint64_t number, uint64_t length):
 * Takes unsigned number and length of bool array to store,
 * converts number into array of binary digits stored in
 * bool array. Converts such that most significant bit is the
 * highest index element, and least significant bit is the lowest
 * index element.
 * Returns this bool array.
 * Exceptions? not found yet.. help me out if you find one.
 */
bool *numbertoBitString(uint64_t number, uint64_t length){
  uint64_t actionNumber = number;
  bool *bitString = malloc(sizeof(bool)*length);
  for (uint64_t i = 0; i < length; i++){
      bitString[i] = actionNumber % 2;
      actionNumber /= 2;
  }

  return bitString;
}


/* int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length):
 * Same as writeMultiBitstoStore, but takes number as input to store
 * in the word. Returns -1 if checkStore fails, else 0.
 */
int writeNumBitstoStore (store STORE, const uint64_t location,
                         const uint64_t wordStartBit, uint64_t number,
                         uint64_t length){

  if(checkStore(STORE, location, wordStartBit)){
    printf(ERROR_MESSAGE("writeNumBitstoStore", "checkStore returned -1",
                         "returning -1"));
    return -1;
  }

  uint64_t availableBits = STORE.wordSize - wordStartBit;
  uint64_t bitstoWrite = (length <= availableBits)?length: availableBits;
  if (bitstoWrite < 64 && number >= pow(2, bitstoWrite)){
      printf(ERROR_MESSAGE("writeNumBitstoStore",
                           "WARNING: given number is bigger than"
                           " requested bit length",
                           "Truncating the number!"));
      number %= (uint64_t)pow(2, bitstoWrite);
  }

  bool *bitArray = numbertoBitString(number, bitstoWrite);
  writeMultiBitstoStore (STORE, location, wordStartBit, bitArray,
                         bitstoWrite);
  return 0;
}
