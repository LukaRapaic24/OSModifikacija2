//
// Created by os on 9/17/24.
//

// ideja za alociranje memorije sa prvog kolokvijuma iz os1 marta 2024.

#include "syscall_c.h"
#include "../lib/hw.h"

class Memory {

public:

  static void* allocate(uint32 blocks);

  static int free(void* startAddress);

  static void clear();

  static void printArr();

private:

  static uint32 *START_ADDRESS;
  static uint32 *END_ADDRESS;

  static uint32 BLOCK_SIZE;

  static uint32 BLOCKS;

  static uint32 *ARR_START;
  static uint32 *ARR_END;

  static uint32 *BLOCKS_START;
  static uint32 *BLOCKS_END;

  static inline void set31(uint32 *n) {*n |= (1 << 31);}
  static inline void clear31(uint32 *n) {*n &= ~(1 << 31);}
  static inline bool isFree(uint32 n) {return n < (uint32)(1 << 31);}

};