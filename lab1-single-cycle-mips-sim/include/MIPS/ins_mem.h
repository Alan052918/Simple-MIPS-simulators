#ifndef INS_MEM_H_
#define INS_MEM_H_

#include "../common.h"

class INSMem {
 public:
  std::bitset<32> Instruction;

  INSMem();

  std::bitset<32> ReadMemory(std::bitset<32> ReadAddress);

 private:
  std::vector<std::bitset<8> > IMem;
};

#endif  // INS_MEM_H
