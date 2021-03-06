#ifndef DECODER_H_
#define DECODER_H_

#include "../common.h"

class Decoder {
 public:
  enum { kIType, kJType, kRType } insType;
  std::bitset<3> aluOp;
  bool isLoad;
  bool isStore;
  bool isBranch;
  bool wrtEnable;

  void DecodeInstruction(std::string InstructionStr);
};

#endif  // DECODER_H_
