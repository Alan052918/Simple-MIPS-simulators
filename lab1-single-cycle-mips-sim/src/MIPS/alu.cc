#include "../../include/common.h"
#include "../../include/MIPS/alu.h"
#include "../../include/MIPS/decoder.h"

bitset<32> ALU::ALUOperation(bitset<3> ALUOP, bitset<32> oprand1,
                             bitset<32> oprand2) {
  // implement!
  switch (ALUOP.to_ulong()) {
    case ADDU: {
      unsigned long oprand1Val = oprand1.to_ulong();
      unsigned long oprand2Val = oprand2.to_ulong();
      ALUresult = bitset<32>(oprand1Val + oprand2Val);
      break;
    }
    case SUBU: {
      unsigned long oprand1Val = oprand1.to_ulong();
      unsigned long oprand2Val = oprand2.to_ulong();
      ALUresult = bitset<32>(oprand1Val - oprand2Val);
      break;
    }
    case AND: {
      ALUresult = oprand1 & oprand2;
      break;
    }
    case OR: {
      ALUresult = oprand1 | oprand2;
      break;
    }
    case NOR: {
      ALUresult = ~(oprand1 | oprand2);
      break;
    }
    default: {
      cout << "unsupported ALU operation" << endl;
      break;
    }
  }
  return ALUresult;
}
