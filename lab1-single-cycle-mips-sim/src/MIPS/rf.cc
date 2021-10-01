#include "../../include/common.h"
#include "../../include/MIPS/rf.h"

RF::RF() {
  Registers.resize(32);
  Registers[0] = bitset<32>(0);
}

void RF::OutputRF() {
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

void RF::ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg,
                   bitset<32> WrtData, bitset<1> WrtEnable) {
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
