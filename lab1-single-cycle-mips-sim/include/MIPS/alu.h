#ifndef ALU_H_
#define ALU_H_

#include "../common.h"

class ALU {
 public:
  std::bitset<32> ALUresult;

  std::bitset<32> ALUOperation(std::bitset<3> ALUOP, std::bitset<32> oprand1,
                               std::bitset<32> oprand2);
};

#endif  // ALU_H_
