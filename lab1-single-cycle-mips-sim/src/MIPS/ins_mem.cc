#include "../../include/common.h"
#include "../../include/MIPS/ins_mem.h"

INSMem::INSMem() {
  IMem.resize(MemSize);
  ifstream imem;
  string line;
  int i = 0;
  imem.open("imem.txt");
  if (imem.is_open()) {
    while (getline(imem, line)) {
      IMem[i] = bitset<8>(line);
      i++;
    }

  } else
    cout << "Unable to open file";
  imem.close();
}

bitset<32> INSMem::ReadMemory(bitset<32> ReadAddress) {
  // implement!
  // (Read the byte at the ReadAddress and the following three byte).
  unsigned long readByteAddr = ReadAddress.to_ulong();
  string instructionStr;
  for (int i = 0; i < 4; ++i) {
    string byteStr = IMem.at(readByteAddr + i).to_string();
    instructionStr += byteStr;
  }
  Instruction = bitset<32>(instructionStr);
  return Instruction;
}
