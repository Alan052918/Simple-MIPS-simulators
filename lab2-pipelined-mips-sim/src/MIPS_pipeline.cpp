#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include "../include/data_mem.hpp"
#include "../include/ins_mem.hpp"
#include "../include/rf.hpp"
#include "../include/state.hpp"

void printState(stateStruct state, int cycle) {
  std::ofstream printstate;
  printstate.open("stateresult.txt", std::ios_base::app);
  if (printstate.is_open()) {
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
  } else
    std::cout << "Unable to open file";
  printstate.close();
}

int main() {
  RF myRF;
  INSMem myInsMem;
  DataMem myDataMem;

  stateStruct state;
  state.IF.nop = false;
  state.ID.nop = true;
  state.EX.nop = true;
  state.MEM.nop = true;
  state.WB.nop = true;
  stateStruct newState;

  unsigned int cycle = 0;

  while (1) {
    /* --------------------- WB stage --------------------- */
    if (!state.WB.nop && state.WB.wrt_enable) {
      myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
    }

    /* --------------------- MEM stage --------------------- */
    if (!state.MEM.nop) {
      if (state.MEM.wrt_mem) {  // Sw
        myDataMem.writeDataMem(state.MEM.ALUresult, state.MEM.Store_data);
      }

      newState.WB.Wrt_data =  // data to be written back to RF
          state.MEM.rd_mem ? myDataMem.readDataMem(state.MEM.ALUresult)  // Lw
                           : state.MEM.ALUresult;  // Addu, Subu
      newState.WB.Rs = state.MEM.Rs;
      newState.WB.Rt = state.MEM.Rt;
      newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
      newState.WB.wrt_enable = state.MEM.wrt_enable;
    }
    newState.WB.nop = state.MEM.nop;

    /* --------------------- EX stage --------------------- */
    if (!state.EX.nop) {
      unsigned long term1;
      unsigned long term2;

      if (state.EX.is_I_type) {
        // I-type: Lw, Sw
        std::bitset<32> imm_sign_ext =
            state.EX.Imm.test(15) ? std::bitset<32>(std::string(16, '1') +
                                                    state.EX.Imm.to_string())
                                  : std::bitset<32>(std::string(16, '0') +
                                                    state.EX.Imm.to_string());
        term1 = state.EX.Read_data1.to_ulong();
        term2 = imm_sign_ext.to_ulong();
      } else {
        // R-type: Addu, Subu
        term1 = state.EX.Read_data1.to_ulong();  // rs
        term2 = state.EX.Read_data2.to_ulong();  // rt
      }

      // Addu, Subu, Lw, Sw (Beq resolved at ID stage)
      newState.MEM.ALUresult =
          std::bitset<32>(state.EX.alu_op ? term1 + term2 : term1 - term2);
      newState.MEM.Store_data = state.EX.Read_data2;  // R[rt] data for Sw
      newState.MEM.Rs = state.EX.Rs;
      newState.MEM.Rt = state.EX.Rt;
      newState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr;
      newState.MEM.rd_mem = state.EX.rd_mem;
      newState.MEM.wrt_mem = state.EX.wrt_mem;
      newState.MEM.wrt_enable = state.EX.wrt_enable;
    }
    newState.MEM.nop = state.EX.nop;

    /* --------------------- ID stage --------------------- */
    if (!state.ID.nop) {
      std::string instruction_str = state.ID.Instr.to_string();
      std::string opcode_str = instruction_str.substr(0, 6);
      std::string funct_str = instruction_str.substr(26);

      newState.EX.is_I_type = (opcode_str == "000000") ? false : true;
      newState.EX.rd_mem = (opcode_str == "100011") ? true : false;   // Lw
      newState.EX.wrt_mem = (opcode_str == "101011") ? true : false;  // Sw

      newState.EX.Rs = std::bitset<5>(instruction_str.substr(6, 5));   // rs
      newState.EX.Rt = std::bitset<5>(instruction_str.substr(11, 5));  // rt
      newState.EX.Read_data1 = myRF.readRF(newState.EX.Rs);  // R[rs] data
      newState.EX.Read_data2 = myRF.readRF(newState.EX.Rt);  // R[rt] data

      newState.EX.Imm =
          std::bitset<16>(instruction_str.substr(16));  // I-type immediate
      newState.EX.Wrt_reg_addr =
          newState.EX.is_I_type
              ? newState.EX.Rt                                  // I-type rt
              : std::bitset<5>(instruction_str.substr(16, 5));  // R-type rd

      newState.EX.alu_op =  // addition: Addu, Lw, Sw
          (funct_str == "100001" || newState.EX.rd_mem || newState.EX.wrt_mem)
              ? true
              : false;
      newState.EX.wrt_enable =  // update RF: Addu, Subu, Lw
          (!newState.EX.is_I_type || newState.EX.rd_mem) ? true : false;

      // branch/PC resolution
      // assumed not taken by default
      newState.IF.PC = std::bitset<32>(state.IF.PC.to_ulong() + 4);
      if ((opcode_str == "000100") &&  // Beq
          (newState.EX.Read_data1 != newState.EX.Read_data2)) {
        // branch taken (bne rs != rt)
        std::bitset<32> branch_addr =
            newState.EX.Imm.test(15)
                ? std::bitset<32>(std::string(14, '1') +
                                  newState.EX.Imm.to_string() + "00")
                : std::bitset<32>(std::string(14, '0') +
                                  newState.EX.Imm.to_string() + "00");
        newState.IF.PC =  // PC = PC + 4 + BranchAddr
            std::bitset<32>(newState.IF.PC.to_ulong() + branch_addr.to_ulong());
      }
    }
    newState.EX.nop = state.ID.nop;

    /* --------------------- IF stage --------------------- */
    if (!state.IF.nop) {
      myInsMem.readInstr(state.IF.PC);
      if (myInsMem.Instruction.all()) {
        // Halt
        newState.IF.nop = true;
        newState.IF.PC = state.IF.PC;
      } else {
        newState.IF.nop = state.IF.nop;
        newState.ID.Instr = myInsMem.Instruction;
      }
    }
    newState.ID.nop = state.IF.nop;

    /* --------------------- Cycle End --------------------- */
    if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop &&
        state.WB.nop)
      break;

    // print states after executing cycle 0, cycle 1, cycle 2 ...
    printState(newState, cycle);

    // The end of the cycle and updates the current state with the values
    // calculated in this cycle
    state = newState;
  }

  myRF.outputRF();            // dump RF;
  myDataMem.outputDataMem();  // dump data mem

  return 0;
}
