#ifndef RF_HPP_
#define RF_HPP_

#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

class RF
{
public:
    std::bitset<32> Reg_data;
    RF()
    {
        Registers.resize(32);
        Registers[0] = std::bitset<32>(0);
    }

    std::bitset<32> readRF(std::bitset<5> Reg_addr)
    {
        Reg_data = Registers[Reg_addr.to_ulong()];
        return Reg_data;
    }

    void writeRF(std::bitset<5> Reg_addr, std::bitset<32> Wrt_reg_data)
    {
        Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
    }

    void outputRF()
    {
        std::ofstream rfout;
        rfout.open("RFresult.txt", std::ios_base::app);
        if (rfout.is_open())
        {
            rfout << "State of RF:\t" << std::endl;
            for (int j = 0; j < 32; j++)
            {
                rfout << Registers[j] << std::endl;
            }
        }
        else
            std::cout << "Unable to open file";
        rfout.close();
    }

private:
    std::vector<std::bitset<32>> Registers;
};

#endif // RF_HPP_
