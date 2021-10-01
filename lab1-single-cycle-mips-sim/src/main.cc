#include "../include/common.h"
#include "../include/MIPS/alu.h"
#include "../include/MIPS/data_mem.h"
#include "../include/MIPS/decoder.h"
#include "../include/MIPS/ins_mem.h"
#include "../include/MIPS/rf.h"

int main() {
  RF myRF;
  ALU myALU;
  INSMem myInsMem;
  DataMem myDataMem;
  Decoder myDecoder;
  bitset<32> myPC;

  // for ID
  string instructionStr;

  // for EXE
  bitset<5> rs;
  bitset<5> rt;
  bitset<5> rd;
  bitset<16> imm;
  string addrStr;

  bitset<32> rsRegData;
  bitset<32> rtRegData;
  bitset<32> rdRegData;

  // for MEM & WB
  bitset<32> wrtData;
  bitset<32> RWMemAddr;
  bitset<5> WBRegAddr;

  while (1) {
    // Fetch
    myInsMem.ReadMemory(myPC);

    myPC = bitset<32>(myPC.to_ulong() + 4);

    // If current insturciton is "11111111111111111111111111111111", then
    // break;
    if (myInsMem.Instruction.all()) {
      break;
    }

    // decode (Read RF)
    instructionStr = myInsMem.Instruction.to_string();
    myDecoder.DecodeInstruction(instructionStr);
    switch (myDecoder.insType) {
      case Decoder::kIType: {
        rs = bitset<5>(instructionStr.substr(6, 5));   // 25-21
        rt = bitset<5>(instructionStr.substr(11, 5));  // 20-16
        imm = bitset<16>(instructionStr.substr(16));   // 15-0

        if (myDecoder.isLoad || myDecoder.isStore ||
            (myDecoder.aluOp.to_string() == "001")) {
          myRF.ReadWrite(rs, bitset<5>(), bitset<5>(), bitset<32>(),
                         bitset<1>());
          rsRegData = myRF.ReadData1;
        }
        if (myDecoder.isBranch) {
          myRF.ReadWrite(rs, rt, bitset<5>(), bitset<32>(), bitset<1>());
          rsRegData = myRF.ReadData1;
          rtRegData = myRF.ReadData2;
        }
        break;
      }
      case Decoder::kJType: {
        addrStr = instructionStr.substr(6);  // 25-0
        break;
      }
      case Decoder::kRType: {
        rs = bitset<5>(instructionStr.substr(6, 5));   // 25-21
        rt = bitset<5>(instructionStr.substr(11, 5));  // 20-16
        rd = bitset<5>(instructionStr.substr(16, 5));  // 15-11

        myRF.ReadWrite(rs, rt, bitset<5>(), bitset<32>(), bitset<1>());
        rsRegData = myRF.ReadData1;
        rtRegData = myRF.ReadData2;
        break;
      }
      default: {
        break;
      }
    }

    // Execute
    switch (myDecoder.insType) {
      case Decoder::kIType: {
        if (myDecoder.isLoad || myDecoder.isStore ||
            (myDecoder.aluOp.to_string() == "001")) {
          bitset<32> immSignExt =
              imm.test(0) ? bitset<32>(string(16, '1') + imm.to_string())
                          : bitset<32>(string(16, '0') + imm.to_string());
          myALU.ALUOperation(myDecoder.aluOp, rsRegData, immSignExt);
          if (myDecoder.isLoad || myDecoder.isStore) {
            RWMemAddr = myALU.ALUresult;  // lw
            if (myDecoder.isStore) {      // sw
              wrtData = rtRegData;
            }
          } else {  // addiu
            wrtData = myALU.ALUresult;
            WBRegAddr = rt;
          }
        } else if (myDecoder.isBranch && (rsRegData == rtRegData)) {  // beq
          bitset<32> branchAddr =
              imm.test(0)
                  ? bitset<32>(string(14, '1') + imm.to_string() + "00")
                  : bitset<32>(string(14, '0') + imm.to_string() + "00");
          myPC = bitset<32>(myPC.to_ulong() + branchAddr.to_ulong());
        }
        break;
      }
      case Decoder::kJType: {
        myPC = bitset<32>(myPC.to_string().substr(0, 4) + addrStr + "00");
        break;
      }
      case Decoder::kRType: {
        myALU.ALUOperation(myDecoder.aluOp, rsRegData, rtRegData);
        wrtData = myALU.ALUresult;
        WBRegAddr = rd;
        break;
      }
      default: {
        cout << "unsupported instruction type" << endl;
        break;
      }
    }

    // Read/Write Mem
    if (myDecoder.isStore) {
      myDataMem.MemoryAccess(RWMemAddr, wrtData, bitset<1>(), bitset<1>(1));
    }
    if (myDecoder.isLoad) {
      myDataMem.MemoryAccess(RWMemAddr, bitset<32>(), bitset<1>(1),
                             bitset<1>());
      wrtData = myDataMem.readdata;
    }

    // Write back to RF
    myRF.ReadWrite(bitset<5>(), bitset<5>(), WBRegAddr, wrtData,
                   bitset<1>(myDecoder.wrtEnable));

    myRF.OutputRF();  // dump RF;
  }
  myDataMem.OutputDataMem();  // dump data mem

  return 0;
}
