#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR 5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space
// reasons, we keep it as this large number, but the memory is still 32-bit
// addressable.
#define MemSize 65536

class RF {
 public:
  bitset<32> ReadData1, ReadData2;
  RF() {
    Registers.resize(32);
    Registers[0] = bitset<32>(0);
  }

  void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg,
                 bitset<32> WrtData, bitset<1> WrtEnable) {
    // implement!
    int rdReg1Addr = (int)RdReg1.to_ulong();
    ReadData1 = Registers.at(rdReg1Addr);
    int rdReg2Addr = (int)RdReg2.to_ulong();
    ReadData2 = Registers.at(rdReg2Addr);

    if (WrtEnable.test(0)) {
      int wrtRegAddr = (int)WrtReg.to_ulong();
      Registers.at(wrtRegAddr) = WrtData;
    }
  }

  void OutputRF() {
    ofstream rfout;
    rfout.open("RFresult.txt", std::ios_base::app);
    if (rfout.is_open()) {
      rfout << "A state of RF:" << endl;
      for (int j = 0; j < 32; j++) {
        rfout << Registers[j] << endl;
      }

    } else
      cout << "Unable to open file";
    rfout.close();
  }

 private:
  vector<bitset<32> > Registers;
};

class ALU {
 public:
  bitset<32> ALUresult;
  bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1,
                          bitset<32> oprand2) {
    // implement!
    switch (ALUOP.to_ulong()) {
      case 1: {  // addu / addiu
        unsigned long oprand1Val = oprand1.to_ulong();
        unsigned long oprand2Val = oprand2.to_ulong();
        ALUresult = bitset<32>(oprand1Val + oprand2Val);
      }
      case 3: {  // subu
        unsigned long oprand1Val = oprand1.to_ulong();
        unsigned long oprand2Val = oprand2.to_ulong();
        ALUresult = bitset<32>(oprand1Val - oprand2Val);
      }
      case 4: {  // and
        ALUresult = oprand1 & oprand2;
      }
      case 5: {  // or
        ALUresult = oprand1 | oprand2;
      }
      case 7: {  // nor
        ALUresult = ~(oprand1 | oprand2);
      }
    }
    return ALUresult;
  }
};

class INSMem {
 public:
  bitset<32> Instruction;
  INSMem() {
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

  bitset<32> ReadMemory(bitset<32> ReadAddress) {
    // implement!
    // (Read the byte at the ReadAddress and the following three byte).
    int readByteAddr = (int)ReadAddress.to_ulong();
    string instructionStr;
    for (int i = 0; i < 4; ++i) {
      string byteStr = IMem.at(readByteAddr + i).to_string();
      instructionStr += byteStr;
    }
    Instruction = bitset<32>(instructionStr);
    return Instruction;
  }

 private:
  vector<bitset<8> > IMem;
};

class DataMem {
 public:
  bitset<32> readdata;
  DataMem() {
    DMem.resize(MemSize);
    ifstream dmem;
    string line;
    int i = 0;
    dmem.open("dmem.txt");
    if (dmem.is_open()) {
      while (getline(dmem, line)) {
        DMem[i] = bitset<8>(line);
        i++;
      }
    } else
      cout << "Unable to open file";
    dmem.close();
  }

  bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData,
                          bitset<1> readmem, bitset<1> writemem) {
    // implement!
    if (readmem.test(0)) {
      int readByteAddr = (int)Address.to_ulong();
      string readDataStr;
      for (int i = 0; i < 4; i++) {
        string byteStr = DMem.at(readByteAddr + i).to_string();
        readDataStr += byteStr;
      }
      readdata = bitset<32>(readDataStr);
    }
    if (writemem.test(0)) {
      int writeAddr = (int)Address.to_ulong();
      string writeDataStr = WriteData.to_string();
      for (int i = 0; i < 32; i += 8) {
        string byteStr = writeDataStr.substr(i, i + 8);
        DMem.at(writeAddr + i) = bitset<8>(byteStr);
      }
    }
    return readdata;
  }

  void OutputDataMem() {
    ofstream dmemout;
    dmemout.open("dmemresult.txt");
    if (dmemout.is_open()) {
      for (int j = 0; j < 1000; j++) {
        dmemout << DMem[j] << endl;
      }

    } else
      cout << "Unable to open file";
    dmemout.close();
  }

 private:
  vector<bitset<8> > DMem;
};

int main() {
  RF myRF;
  ALU myALU;
  INSMem myInsMem;
  DataMem myDataMem;

  while (1) {
    // Fetch

    // If current insturciton is "11111111111111111111111111111111", then break;

    // decode(Read RF)

    // Execute

    // Read/Write Mem

    // Write back to RF

    myRF.OutputRF();  // dump RF;
  }
  myDataMem.OutputDataMem();  // dump data mem

  return 0;
}
