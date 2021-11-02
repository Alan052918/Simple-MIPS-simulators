#ifndef INS_MEM_HPP_
#define INS_MEM_HPP_

#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

class INSMem
{
public:
    std::bitset<32> Instruction;
    INSMem()
    {
        IMem.resize(MemSize);
        std::ifstream imem;
        std::string line;
        int i = 0;
        imem.open("imem.txt");
        if (imem.is_open())
        {
            while (getline(imem, line))
            {
                IMem[i] = std::bitset<8>(line);
                i++;
            }
        }
        else
            std::cout << "Unable to open file";
        imem.close();
    }

    std::bitset<32> readInstr(std::bitset<32> ReadAddress)
    {
        std::string insmem;
        insmem.append(IMem[ReadAddress.to_ulong()].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 1].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 2].to_string());
        insmem.append(IMem[ReadAddress.to_ulong() + 3].to_string());
        Instruction = std::bitset<32>(insmem); // read instruction memory
        return Instruction;
    }

private:
    std::vector<std::bitset<8>> IMem;
};

#endif // INS_MEM_HPP_
