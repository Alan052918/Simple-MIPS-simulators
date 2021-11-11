#ifndef DATA_MEM_HPP_
#define DATA_MEM_HPP_

#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

// memory size, in reality, the memory size should be 2^32, but for this
// lab, for the space resaon, we keep it as this large number, but the
// memory is still 32-bit addressable.
#define MemSize 1000

class DataMem {
 public:
  std::bitset<32> ReadData;
  DataMem() {
    DMem.resize(MemSize);
    std::ifstream dmem;
    std::string line;
    int i = 0;
    dmem.open("dmem.txt");
    if (dmem.is_open()) {
      while (getline(dmem, line)) {
        DMem[i] = std::bitset<8>(line);
        i++;
      }
    } else
      std::cout << "Unable to open file";
    dmem.close();
  }

  std::bitset<32> readDataMem(std::bitset<32> Address) {
    std::string datamem;
    datamem.append(DMem[Address.to_ulong()].to_string());
    datamem.append(DMem[Address.to_ulong() + 1].to_string());
    datamem.append(DMem[Address.to_ulong() + 2].to_string());
    datamem.append(DMem[Address.to_ulong() + 3].to_string());
    ReadData = std::bitset<32>(datamem);  // read data memory
    return ReadData;
  }

  void writeDataMem(std::bitset<32> Address, std::bitset<32> WriteData) {
    DMem[Address.to_ulong()] =
        std::bitset<8>(WriteData.to_string().substr(0, 8));
    DMem[Address.to_ulong() + 1] =
        std::bitset<8>(WriteData.to_string().substr(8, 8));
    DMem[Address.to_ulong() + 2] =
        std::bitset<8>(WriteData.to_string().substr(16, 8));
    DMem[Address.to_ulong() + 3] =
        std::bitset<8>(WriteData.to_string().substr(24, 8));
  }

  void outputDataMem() {
    std::ofstream dmemout;
    dmemout.open("dmemresult.txt");
    if (dmemout.is_open()) {
      for (int j = 0; j < 1000; j++) {
        dmemout << DMem[j] << std::endl;
      }
    } else
      std::cout << "Unable to open file";
    dmemout.close();
  }

 private:
  std::vector<std::bitset<8>> DMem;
};

#endif  // DATA_MEM_HPP_
