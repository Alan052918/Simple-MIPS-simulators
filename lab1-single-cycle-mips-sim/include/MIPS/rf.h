#ifndef RF_H_
#define RF_H_

#include "../common.h"

class RF {
 public:
  bitset<32> ReadData1, ReadData2;

  RF();

  void OutputRF();
  void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg,
                 bitset<32> WrtData, bitset<1> WrtEnable);

 private:
  vector<bitset<32> > Registers;
};

#endif  // RF_H_
