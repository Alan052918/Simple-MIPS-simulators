#include "../../include/common.h"
#include "../../include/MIPS/data_mem.h"

DataMem::DataMem() {
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

void DataMem::OutputDataMem() {
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

bitset<32> DataMem::MemoryAccess(bitset<32> Address, bitset<32> WriteData,
                                 bitset<1> readmem, bitset<1> writemem) {
  // implement!
  if (readmem.test(0)) {
    unsigned readByteAddr = Address.to_ulong();
    string readDataStr;
    for (int i = 0; i < 4; i++) {
      string byteStr = DMem.at(readByteAddr + i).to_string();
      readDataStr += byteStr;
    }
    readdata = bitset<32>(readDataStr);
  }
  if (writemem.test(0)) {
    unsigned writeAddr = Address.to_ulong();
    string writeDataStr = WriteData.to_string();
    for (int i = 0; i < 32; i += 8) {
      string byteStr = writeDataStr.substr(i, i + 8);
      DMem.at(writeAddr + i) = bitset<8>(byteStr);
    }
  }
  return readdata;
}
