#include <bitset>
#include <iostream>

using namespace std;

typedef struct RInsField {
  bitset<5> rs;
  bitset<5> rt;
  bitset<5> rd;

  bitset<32> rsData;
  bitset<32> rtData;
  bitset<32> rdData;
} RInsField;

typedef struct IInsField {
  bitset<5> rs;
  bitset<5> rt;
  bitset<16> imm;

  bitset<32> rsData;
  bitset<32> rtData;
  bitset<32> immSignExt;
} IInsField;

typedef struct JInsField {
  bitset<26> address;

  bitset<32> jAddress;
} JInsField;

class Ins {
 public:
  enum type { kR, kI, kJ };
  union {
    RInsField rInsField;
    IInsField iInsField;
    JInsField jInsField;
  };

  Ins(string insStr);
};

Ins::Ins(string insStr) {
  this->type = kR;
  this->rInsField->rsStr = bitset<5>(insStr.substr(0, 2));
  this->rInsField->rtStr = bitset<5>(insStr.substr(2, 2));
  this->rInsField->rdStr = bitset<5>(insStr.substr(4, 2));
}

int main() {
  Ins ins(string("123456"));
  cout << ins.type << endl;
  cout << ins.rInsField.rs.to_string() << endl;
}
