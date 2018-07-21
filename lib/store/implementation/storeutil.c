#include "store/storeutil.h"

#include <stdbool.h>
#include <stdint.h>

#define ERROR_MESSAGE(location,reason,reaction) ("\nin " location ":\n"\
                                                 reason ", " reaction "\n")
#define WRITE_ERROR(reason,reaction) ERROR_MESSAGE("writeMultiBitstoStore",\
                                                   reason,reaction)

int writeMultiBitstoStore (store STORE, const uint64_t location,
                           const uint64_t wordStartBit, const bool bitArray[],
                           size_t length)
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
  }

  if((STORE.wordSize - wordStartBit) < length){
    printf(WRITE_ERROR("WARNING: bitArray length is too big to fit in word",\
                       "truncating the bitArray"));
    length = STORE.wordSize - wordStartBit;
  }

  for (uint64_t index = 0; index < length; index++){
    writeBittoStore(STORE, location, wordStartBit, bitArray[index]);
  }

  return 0;
}
