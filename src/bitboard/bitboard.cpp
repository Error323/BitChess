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
  std::cout << ToString(inBoard) << std::endl;
}

int Transform(U64 inBoard, U64 inMagic, int inBits)
{
  return int((inBoard * inMagic) >> (64 - inBits));
}

Uint8 PopLSB(U64 &inBoard)
{
  Uint8 index = LSBIndex(inBoard);
  inBoard &= inBoard-1;
  return index;
}

const U64 magic = 0x022fdd63cc95386d; ///< The DeBruin magic number
const Uint8 magic_table[64] = ///< Indices for finding the LSB index of a board
{
   0,  1,  2, 53,  3,  7, 54, 27,
   4, 38, 41,  8, 34, 55, 48, 28,
  62,  5, 39, 46, 44, 42, 22,  9,
  24, 35, 59, 56, 49, 18, 29, 11,
  63, 52,  6, 26, 37, 40, 33, 47,
  61, 45, 43, 21, 23, 58, 17, 10,
  51, 25, 36, 32, 60, 20, 57, 16,
  50, 31, 19, 15, 30, 14, 13, 12,
};
Uint8 LSBIndex(U64 inBoard)
{
  return magic_table[((inBoard&-inBoard)*magic) >> 58];
}

int CountBits(U64 inBoard)
{
  int i;
  for(i = 0; inBoard; i++, inBoard &= inBoard - 1);
  return i;
}

U64 Index2U64(int inIndex, int inBits, U64 inMask)
{
  int i, j;
  U64 result = 0ULL;
  for(i = 0; i < inBits; i++)
  {
    j = PopLSB(inMask);
    if(inIndex & (1 << i)) result |= (1ULL << j);
  }
  return result;
}

}
