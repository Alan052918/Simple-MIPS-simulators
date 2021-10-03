#ifndef RF_H_
#define RF_H_

#include "../common.h"

class RF {
 public:
  std::bitset<32> ReadData1, ReadData2;

  RF();

  void OutputRF();
  void ReadWrite(std::bitset<5> RdReg1, std::bitset<5> RdReg2,
                 std::bitset<5> WrtReg, std::bitset<32> WrtData,
                 std::bitset<1> WrtEnable);

 private:
  std::vector<std::bitset<32> > Registers;
};

#endif  // RF_H_
