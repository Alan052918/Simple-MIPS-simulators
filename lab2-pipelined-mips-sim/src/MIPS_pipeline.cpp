#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include "../include/data_mem.hpp"
#include "../include/ins_mem.hpp"
#include "../include/rf.hpp"
#include "../include/state.hpp"

void printState(stateStruct state, int cycle)
{
    std::ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    if (printstate.is_open())
    {
        printstate << "State after executing cycle:\t" << cycle << std::endl;

        printstate << "IF.PC:\t" << state.IF.PC.to_ulong() << std::endl;
        printstate << "IF.nop:\t" << state.IF.nop << std::endl;

        printstate << "ID.Instr:\t" << state.ID.Instr << std::endl;
        printstate << "ID.nop:\t" << state.ID.nop << std::endl;

        printstate << "EX.Read_data1:\t" << state.EX.Read_data1 << std::endl;
        printstate << "EX.Read_data2:\t" << state.EX.Read_data2 << std::endl;
        printstate << "EX.Imm:\t" << state.EX.Imm << std::endl;
        printstate << "EX.Rs:\t" << state.EX.Rs << std::endl;
        printstate << "EX.Rt:\t" << state.EX.Rt << std::endl;
        printstate << "EX.Wrt_reg_addr:\t" << state.EX.Wrt_reg_addr << std::endl;
        printstate << "EX.is_I_type:\t" << state.EX.is_I_type << std::endl;
        printstate << "EX.rd_mem:\t" << state.EX.rd_mem << std::endl;
        printstate << "EX.wrt_mem:\t" << state.EX.wrt_mem << std::endl;
        printstate << "EX.alu_op:\t" << state.EX.alu_op << std::endl;
        printstate << "EX.wrt_enable:\t" << state.EX.wrt_enable << std::endl;
        printstate << "EX.nop:\t" << state.EX.nop << std::endl;

        printstate << "MEM.ALUresult:\t" << state.MEM.ALUresult << std::endl;
        printstate << "MEM.Store_data:\t" << state.MEM.Store_data << std::endl;
        printstate << "MEM.Rs:\t" << state.MEM.Rs << std::endl;
        printstate << "MEM.Rt:\t" << state.MEM.Rt << std::endl;
        printstate << "MEM.Wrt_reg_addr:\t" << state.MEM.Wrt_reg_addr << std::endl;
        printstate << "MEM.rd_mem:\t" << state.MEM.rd_mem << std::endl;
        printstate << "MEM.wrt_mem:\t" << state.MEM.wrt_mem << std::endl;
        printstate << "MEM.wrt_enable:\t" << state.MEM.wrt_enable << std::endl;
        printstate << "MEM.nop:\t" << state.MEM.nop << std::endl;

        printstate << "WB.Wrt_data:\t" << state.WB.Wrt_data << std::endl;
        printstate << "WB.Rs:\t" << state.WB.Rs << std::endl;
        printstate << "WB.Rt:\t" << state.WB.Rt << std::endl;
        printstate << "WB.Wrt_reg_addr:\t" << state.WB.Wrt_reg_addr << std::endl;
        printstate << "WB.wrt_enable:\t" << state.WB.wrt_enable << std::endl;
        printstate << "WB.nop:\t" << state.WB.nop << std::endl;
    }
    else
        std::cout << "Unable to open file";
    printstate.close();
}

int main()
{

    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;

    stateStruct state;
    stateStruct newState;

    unsigned int cycle = 0;

    while (1)
    {

        /* --------------------- WB stage --------------------- */

        /* --------------------- MEM stage --------------------- */

        /* --------------------- EX stage --------------------- */

        /* --------------------- ID stage --------------------- */

        /* --------------------- IF stage --------------------- */
        myInsMem.readInstr(state.IF.PC);

        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;

        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ...

        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */
    }

    myRF.outputRF();           // dump RF;
    myDataMem.outputDataMem(); // dump data mem

    return 0;
}
