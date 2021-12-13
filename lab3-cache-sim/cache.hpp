#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <bitset>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// access state:
typedef int AccessState;
#define NA 0  // no action
#define RH 1  // read hit
#define RM 2  // read miss
#define WH 3  // Write hit
#define WM 4  // write miss

struct config {
  size_t L1blocksize;
  size_t L1setsize;
  size_t L1size;
  size_t L2blocksize;
  size_t L2setsize;
  size_t L2size;
};

/* you can define the cache class here, or design your own data structure for L1
   and L2 cache
*/
class LowLevelCache {
 public:
  LowLevelCache() {
    this->tagPos = std::make_pair(0, 0);
    this->setIndexPos = std::make_pair(0, 0);
    this->blockOffsetPos = std::make_pair(0, 0);
    std::cout << "new LowLevelCache object created, uninitialized."
              << std::endl;
  }

  LowLevelCache(int blockSize, int associativity, int cacheSize) {
    int tagLength, setIndexLength, blockOffsetLength;

    if (associativity == 0) {
      setIndexLength = log2(cacheSize * pow(2, 10) / blockSize);
    } else {
      setIndexLength =
          log2(cacheSize * pow(2, 10) / (blockSize * associativity));
    }
    blockOffsetLength = log2(blockSize);
    tagLength = 32 - setIndexLength - blockOffsetLength;

    this->tagPos = std::make_pair(0, tagLength);
    this->setIndexPos = std::make_pair(tagLength, setIndexLength);
    this->blockOffsetPos =
        std::make_pair(tagLength + setIndexLength, blockOffsetLength);
    this->wayCount = associativity;

    for (int i = 0; i < std::pow(2, setIndexLength); i++) {
      this->tagBitsBuffer.push_back(
          std::vector<unsigned long>(this->wayCount, 0));
      this->validBitsBuffer.push_back(std::vector<bool>(this->wayCount, false));
    }

    if (associativity == 0) {
      std::cout << "Fully associative" << std::endl;
    } else if (associativity == 1) {
      std::cout << "Directly mapped" << std::endl;
    } else {
      std::cout << associativity << "-way associative" << std::endl;
    }
    std::cout << "\ttag bits: " << tagPos.first << "-" << tagPos.second
              << std::endl;
    std::cout << "\tset index bits: " << setIndexPos.first << "-"
              << setIndexPos.second << std::endl;
    std::cout << "\tblock Offset bits: " << blockOffsetPos.first << "-"
              << blockOffsetPos.second << std::endl;
  }

  /**
   * @brief Read cache for memory at addr, update cache access state
   *
   * @param addr read memory address
   */
  void read(std::bitset<32> addr) {
    this->decodeAddress(addr);
    this->accessState = RM;
    for (int i = 0; i < this->wayCount; i++) {
      if ((this->tagBitsBuffer.at(this->setIndexVal).at(i) == this->tagVal) &&
          (this->validBitsBuffer.at(this->setIndexVal).at(i) == true)) {
        this->accessState = RH;
        break;
      }
    }
  }

  /**
   * @brief
   *
   * @param addr
   */
  void write(std::bitset<32> addr) {
    this->decodeAddress(addr);
    this->accessState = WM;
    for (int i = 0; i < this->wayCount; i++) {
      if ((this->tagBitsBuffer.at(this->setIndexVal).at(i) == this->tagVal) &&
          (this->validBitsBuffer.at(this->setIndexVal).at(i) == true)) {
        this->accessState = WH;
        break;
      }
    }
  }

  /**
   * @brief Insert tag bits of missed address, evict old tag values if necessary
   *
   */
  void update() {
    for (int way = 0; way < this->wayCount; way++) {
      if (this->validBitsBuffer.at(this->setIndexVal).at(way) == false) {
        this->validBitsBuffer.at(this->setIndexVal).at(way) = true;
        this->tagBitsBuffer.at(this->setIndexVal).at(way) = this->tagVal;
        std::cout << "Way " << way << " invalid, cache updated" << std::endl;
        return;
      }
    }

    // all ways are valid, set full
    this->tagBitsBuffer.at(this->setIndexVal).at(wayEvict) = this->tagVal;
    std::cout << "Set full, way " << wayEvict << " evicted, cache updated"
              << std::endl;

    // increment eviction counter
    wayEvict++;
    if (wayEvict == wayCount) {
      wayEvict = 0;
    }
  }

  AccessState getAccessState() { return this->accessState; }

  void resetStates() {
    this->tagVal = 0;
    this->setIndexVal = 0;
    this->blockOffsetVal = 0;
    this->accessState = NA;
  }

 private:
  // first parameter is starting index in the address
  // second parameter is offset
  std::pair<int, int> tagPos;
  std::pair<int, int> setIndexPos;
  std::pair<int, int> blockOffsetPos;

  // equals to associativity, for read traversal
  int wayCount;
  // for eviction when all ways are full
  int wayEvict;

  // temporary states for update access
  unsigned long tagVal;
  unsigned long setIndexVal;
  unsigned long blockOffsetVal;

  AccessState accessState;

  // first layer is to select set
  // second layer is to select way inside a set
  std::vector<std::vector<unsigned long> > tagBitsBuffer;
  std::vector<std::vector<bool> > validBitsBuffer;

  void decodeAddress(std::bitset<32> addr) {
    std::string addrStr = addr.to_string();
    this->tagVal =
        std::bitset<32>(addrStr.substr(tagPos.first, tagPos.second)).to_ulong();
    this->setIndexVal =
        std::bitset<32>(addrStr.substr(setIndexPos.first, setIndexPos.second))
            .to_ulong();
    this->blockOffsetVal =
        std::bitset<32>(
            addrStr.substr(blockOffsetPos.first, blockOffsetPos.second))
            .to_ulong();
  }
};

class Cache {
 public:
  Cache(config cacheConfig) {
    this->l1Cache = LowLevelCache(cacheConfig.L1blocksize,
                                  cacheConfig.L1setsize, cacheConfig.L1size);
    this->l2Cache = LowLevelCache(cacheConfig.L2blocksize,
                                  cacheConfig.L2setsize, cacheConfig.L2size);
  }

  void read(std::bitset<32> addr) {
    this->l1Cache.read(addr);
    if (this->l1Cache.getAccessState() == RH) {
      std::cout << "L1 Cache Read Hit" << std::endl;
      return;
    }
    std::cout << "L1 Cache Read Miss" << std::endl;
    this->l2Cache.read(addr);
    if (this->l2Cache.getAccessState() == RH) {
      std::cout << "L2 Cache Read Hit" << std::endl;

      // TODO: update L1
      this->l1Cache.update();
      return;
    }
    std::cout << "L2 Cache Read Miss" << std::endl;
    // TODO: update L1 and L2
    this->l1Cache.update();
    this->l2Cache.update();
  }

  void write(std::bitset<32> addr) {
    this->l1Cache.write(addr);
    if (this->l1Cache.getAccessState() == WH) {
      std::cout << "L1 Cache Write Hit" << std::endl;
      return;
    }
    std::cout << "L1 Cache Write Miss" << std::endl;
    this->l2Cache.write(addr);
    if (this->l2Cache.getAccessState() == WH) {
      std::cout << "L2 Cache Write Hit" << std::endl;
      return;
    }
    std::cout << "L2 Cache Write Miss" << std::endl;
  }

  void resetAccessStates() {
    this->l1Cache.resetStates();
    this->l2Cache.resetStates();
  }

 private:
  LowLevelCache l1Cache;
  LowLevelCache l2Cache;
};

#endif  // CACHE_HPP_
