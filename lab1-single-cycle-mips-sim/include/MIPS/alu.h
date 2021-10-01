#ifndef ALU_H_
#define ALU_H_

#include "../common.h"

class ALU {
 public:
  bitset<32> ALUresult;

  bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1,
                          bitset<32> oprand2);
};

#endif  // ALU_H_
