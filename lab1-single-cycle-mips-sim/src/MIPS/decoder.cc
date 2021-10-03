#include "../../include/common.h"
#include "../../include/MIPS/decoder.h"

void Decoder::DecodeInstruction(std::string InstructionStr) {
  std::string opcodeStr = InstructionStr.substr(0, 6);  // inst[31:26]

  if (opcodeStr == "000000") {
    this->insType = kRType;
  } else if (opcodeStr == "000010") {
    this->insType = kJType;  // j
  } else {
    this->insType = kIType;
  }

  if (opcodeStr == "100011") {
    this->isLoad = true;  // lw
  } else if (opcodeStr == "101011") {
    this->isStore = true;  // sw
  } else if (opcodeStr == "000100") {
    this->isBranch = true;  // beq
  }

  if (this->isLoad || this->isStore) {
    this->aluOp = std::bitset<3>("001");
  } else if (this->insType == kRType) {
    this->aluOp =
        std::bitset<3>(InstructionStr.substr(29)).to_ulong();  // inst[2:0]
  } else {
    this->aluOp =
        std::bitset<3>(InstructionStr.substr(3, 3)).to_ulong();  // inst[28:26]
  }

  if (this->isStore || this->isBranch || (this->insType == kJType)) {
    this->wrtEnable = false;
  } else {
    this->wrtEnable = true;
  }
}
