#ifndef STATE_HPP_
#define STATE_HPP_

#include <bitset>

struct IFStruct
{
  std::bitset<32> PC;
  bool nop;
};

struct IDStruct
{
  std::bitset<32> Instr;
  bool nop;
};

struct EXStruct
{
  std::bitset<32> Read_data1;
  std::bitset<32> Read_data2;
  std::bitset<16> Imm;
  std::bitset<5> Rs;
  std::bitset<5> Rt;
  std::bitset<5> Wrt_reg_addr;
  bool is_I_type;
  bool rd_mem;
  bool wrt_mem;
  bool alu_op; // 1 for addu, lw, sw, 0 for subu
  bool wrt_enable;
  bool nop;
};

struct MEMStruct
{
  std::bitset<32> ALUresult;
  std::bitset<32> Store_data;
  std::bitset<5> Rs;
  std::bitset<5> Rt;
  std::bitset<5> Wrt_reg_addr;
  bool rd_mem;
  bool wrt_mem;
  bool wrt_enable;
  bool nop;
};

struct WBStruct
{
  std::bitset<32> Wrt_data;
  std::bitset<5> Rs;
  std::bitset<5> Rt;
  std::bitset<5> Wrt_reg_addr;
  bool wrt_enable;
  bool nop;
};

struct stateStruct
{
  IFStruct IF;
  IDStruct ID;
  EXStruct EX;
  MEMStruct MEM;
  WBStruct WB;
};

#endif // STATE_HPP_
