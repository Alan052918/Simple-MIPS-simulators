#ifndef DATA_MEM_H_
#define DATA_MEM_H_

#include "../common.h"

class DataMem {
 public:
  bitset<32> readdata;

  DataMem();

  void OutputDataMem();
  bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData,
                          bitset<1> readmem, bitset<1> writemem);

 private:
  vector<bitset<8> > DMem;
};

#endif  // DATA_MEM_H_
