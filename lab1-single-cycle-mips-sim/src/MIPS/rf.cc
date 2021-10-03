#include "../../include/common.h"
#include "../../include/MIPS/rf.h"

RF::RF() {
  Registers.resize(32);
  Registers[0] = std::bitset<32>(0);
}

void RF::OutputRF() {
  std::ofstream rfout;
  rfout.open("RFresult.txt", std::ios_base::app);
  if (rfout.is_open()) {
    rfout << "A state of RF:" << std::endl;
    for (int j = 0; j < 32; j++) {
      rfout << Registers[j] << std::endl;
    }
  } else {
    std::cout << "Unable to open file";
  }
  rfout.close();
}

void RF::ReadWrite(std::bitset<5> RdReg1, std::bitset<5> RdReg2,
                   std::bitset<5> WrtReg, std::bitset<32> WrtData,
                   std::bitset<1> WrtEnable) {
  // implement!
  unsigned rdReg1Addr = RdReg1.to_ulong();
  ReadData1 = Registers.at(rdReg1Addr);
  unsigned rdReg2Addr = RdReg2.to_ulong();
  ReadData2 = Registers.at(rdReg2Addr);

  if (WrtEnable.test(0)) {
    unsigned wrtRegAddr = WrtReg.to_ulong();
    Registers.at(wrtRegAddr) = WrtData;
  }
}
