#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <cmath>
#include "../utils/types.h"
#include <x86intrin.h>


/**
 * @namespace bitboard
 *
 * @brief
 * 64 bit board and various helper functions
 */
namespace bboard
{

#define C64(x) x##ull

#define KING_START_POS C64(0x1000000000000010)
#define NOT_A_FILE     C64(0xfefefefefefefefe)
#define NOT_H_FILE     C64(0x7f7f7f7f7f7f7f7f)
#define A_FILE         C64(0x0101010101010101)
#define H_FILE         C64(0x8080808080808080)
#define E_FILE         C64(0x1010101010101010)
#define NOT_RANK_8     C64(0x00ffffffffffffff)
#define NOT_RANK_1     C64(0xffffffffffffff00)
#define RANK8          C64(0xff00000000000000)
#define RANK1          C64(0x00000000000000ff)
#define BLACK_SQUARES  C64(0xAA55AA55AA55AA55)
#define WHITE_SQUARES  (~BLACK)

#define Rank(x) ((x)>>3)
#define File(x) ((x)&7)
#define Flip(x) ((x)^1)
#define FileDistance(a, b) abs(File(a) - File(b))
#define RankDistance(a, b) abs(Rank(a) - Rank(b))
#define Distance(a, b) std::max(FileDistance(a,b), RankDistance(a,b))
#define Set(sq) (C64(1) << (sq))
#define Unset(sq) (~Set(sq))
#define TurnOff(b, s) (b) &= Unset(s)
#define TurnOn(b, s) (b) |= Set(s)

#define SoutOne(x) ((x) >> 8)
#define NortOne(x) ((x) << 8)
#define EastOne(x) (((x) << 1) & NOT_A_FILE)
#define WestOne(x) (((x) >> 1) & NOT_H_FILE)
#define NoEaOne(x) (((x) << 9) & NOT_A_FILE)
#define NoWeOne(x) (((x) << 7) & NOT_H_FILE)
#define SoEaOne(x) (((x) >> 7) & NOT_A_FILE)
#define SoWeOne(x) (((x) >> 9) & NOT_H_FILE)

/// Pops the least significant bit of the board and returns index
inline int PopLSB(register U64 &b)
{
  int index = __bsfq(b);
  b &= b - 1;
  return index;
}

/// Find the index to the least significant bit
inline int LSBIndex(register U64 b)
{
  return __bsfq(b);
}

/// Count the number of set bits on a bitboard
inline int CountBits(register U64 b)
{
  return __popcntq(b);
}

/// Print a bitboard to stdout
void Print1(const U64 board);

/// Print 2 bitboards to stdout beside eachother
void Print2(const U64 a, const U64 b);

/// Print 3 bitboards to stdout beside eachother
void Print3(const U64 a, const U64 b, const U64 c);

} // namespace bboard

#endif // BITBOARD_H
