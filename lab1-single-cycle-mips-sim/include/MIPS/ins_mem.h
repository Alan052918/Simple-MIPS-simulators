#ifndef INS_MEM_H_
#define INS_MEM_H_

#include "../common.h"

class INSMem {
 public:
  bitset<32> Instruction;

  INSMem();

  bitset<32> ReadMemory(bitset<32> ReadAddress);

 private:
  vector<bitset<8> > IMem;
};

#endif  // INS_MEM_H
