#ifndef COMMON_H_
#define COMMON_H_

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ADDU 1
#define SUBU 3
#define AND 4
#define OR 5
#define NOR 7

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space
// reasons, we keep it as this large number, but the memory is still 32-bit
// addressable.
#define MemSize 65536

#endif  // COMMON_H_
