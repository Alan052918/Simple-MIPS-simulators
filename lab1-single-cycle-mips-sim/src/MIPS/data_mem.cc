#include "../../include/common.h"
#include "../../include/MIPS/data_mem.h"

DataMem::DataMem() {
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
  } else {
    std::cerr << "Unable to open dmem file" << std::endl;
    exit(EXIT_FAILURE);
  }
  dmem.close();
}

void DataMem::OutputDataMem() {
  std::ofstream dmemout;
  dmemout.open("dmemresult.txt");
  if (dmemout.is_open()) {
    for (int j = 0; j < 1000; j++) {
      dmemout << DMem[j] << std::endl;
    }
  } else {
    std::cout << "Unable to open dmemresult file" << std::endl;
    exit(EXIT_FAILURE);
  }
  dmemout.close();
}

std::bitset<32> DataMem::MemoryAccess(std::bitset<32> Address,
                                      std::bitset<32> WriteData,
                                      std::bitset<1> readmem,
                                      std::bitset<1> writemem) {
  // implement!
  if (readmem.test(0)) {
    int readByteAddr = (int)Address.to_ulong();
    std::string readDataStr;
    for (int i = 0; i < 4; i++) {
      std::string byteStr = DMem.at(readByteAddr + i).to_string();
      readDataStr += byteStr;
    }
    readdata = std::bitset<32>(readDataStr);
  }
  if (writemem.test(0)) {
    int writeAddr = (int)Address.to_ulong();
    std::string writeDataStr = WriteData.to_string();
    for (int i = 0; i < 32; i += 8) {
      std::string byteStr = writeDataStr.substr(i, i + 8);
      DMem.at(writeAddr + i) = std::bitset<8>(byteStr);
    }
  }
  return readdata;
}
