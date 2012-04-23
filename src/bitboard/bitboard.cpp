#include "bitboard.h"

#include <iostream>
#include <sstream>

namespace bboard
{

U64 RookMask(int sq)
{
  U64 result = C64(0);
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1; r <= 6; r++) result |= (C64(1) << (fl + r * 8));
  for (r = rk - 1; r >= 1; r--) result |= (C64(1) << (fl + r * 8));
  for (f = fl + 1; f <= 6; f++) result |= (C64(1) << (f + rk * 8));
  for (f = fl - 1; f >= 1; f--) result |= (C64(1) << (f + rk * 8));
  return result;
}

U64 BishopMask(int sq)
{
  U64 result = C64(0);
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) result |= (C64(1) << (f + r * 8));
  for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) result |= (C64(1) << (f + r * 8));
  for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) result |= (C64(1) << (f + r * 8));
  for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) result |= (C64(1) << (f + r * 8));
  return result;
}

U64 RookAttack(int sq, U64 block)
{
  U64 result = C64(0);
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1; r <= 7; r++)
  {
    result |= (C64(1) << (fl + r * 8));
    if (block & (C64(1) << (fl + r * 8))) break;
  }
  for (r = rk - 1; r >= 0; r--)
  {
    result |= (C64(1) << (fl + r * 8));
    if (block & (C64(1) << (fl + r * 8))) break;
  }
  for (f = fl + 1; f <= 7; f++)
  {
    result |= (C64(1) << (f + rk * 8));
    if (block & (C64(1) << (f + rk * 8))) break;
  }
  for (f = fl - 1; f >= 0; f--)
  {
    result |= (C64(1) << (f + rk * 8));
    if (block & (C64(1) << (f + rk * 8))) break;
  }
  return result;
}

U64 BishopAttack(int sq, U64 block)
{
  U64 result = C64(0);
  int rk = sq / 8, fl = sq % 8, r, f;
  for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++)
  {
    result |= (C64(1) << (f + r * 8));
    if (block & (C64(1) << (f + r * 8))) break;
  }
  for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--)
  {
    result |= (C64(1) << (f + r * 8));
    if (block & (C64(1) << (f + r * 8))) break;
  }
  for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++)
  {
    result |= (C64(1) << (f + r * 8));
    if (block & (C64(1) << (f + r * 8))) break;
  }
  for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--)
  {
    result |= (C64(1) << (f + r * 8));
    if (block & (C64(1) << (f + r * 8))) break;
  }
  return result;
}

U64 KnightAttack(U64 knights) {
   U64 l1 = (knights >> 1) & C64(0x7f7f7f7f7f7f7f7f);
   U64 l2 = (knights >> 2) & C64(0x3f3f3f3f3f3f3f3f);
   U64 r1 = (knights << 1) & C64(0xfefefefefefefefe);
   U64 r2 = (knights << 2) & C64(0xfcfcfcfcfcfcfcfc);
   U64 h1 = l1 | r1;
   U64 h2 = l2 | r2;
   return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
}

std::string ToString(const U64 &inBoard)
{
  std::stringstream ss;
  for (int i = 7; i >= 0; i--)
  {
    ss << i + 1;
    Uint8 line = (inBoard >> (i * 8)) & 0xff;
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

int PopLSB(U64 &inBoard)
{
  int index = LSBIndex(inBoard);
  inBoard &= inBoard - 1;
  return index;
}

int LSBIndex(U64 inBoard)
{
  return magic_table[((inBoard & -inBoard) * magic) >> 58];
}

int CountBits(U64 inBoard)
{
  int i;
  for (i = 0; inBoard; i++, inBoard &= inBoard - 1);
  return i;
}

U64 Index2U64(int inIndex, int inBits, U64 inMask)
{
  int i, j;
  U64 result = C64(0);
  for (i = 0; i < inBits; i++)
  {
    j = PopLSB(inMask);
    if (inIndex & (1 << i)) result |= (C64(1) << j);
  }
  return result;
}

}
