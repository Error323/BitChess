#include "bitboard.h"

#include <iostream>
#include <sstream>

namespace bboard {

std::string ToString(const U64 &inBoard)
{
  std::stringstream ss;
  for (int i = 7; i >= 0; i--)
  {
    ss << i+1;
    Uint8 line = (inBoard >> (i*8)) & 0xff;
    for (int j = 0; j < 8; j++)
      ss << (((line >> j) & 1) == 1 ? " 1" : " .");
    ss << "\n";
  }
  ss << "  a b c d e f g h\n";
  return ss.str();
}

void Print(const U64 &inBoard)
{
  std::cout << ToString(inBoard);
}

Uint8 LSBIndex(U64 inBoard)
{
  return magic_table[((inBoard&-inBoard)*magic) >> 58];
}

}
