#ifndef DATA_MEM_H_
#define DATA_MEM_H_

#include "../common.h"

class DataMem {
 public:
  std::bitset<32> readdata;

  DataMem();

  void OutputDataMem();
  std::bitset<32> MemoryAccess(std::bitset<32> Address,
                               std::bitset<32> WriteData,
                               std::bitset<1> readmem, std::bitset<1> writemem);

 private:
  std::vector<std::bitset<8> > DMem;
};

#endif  // DATA_MEM_H_
