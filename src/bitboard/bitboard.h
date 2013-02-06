#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <cmath>
#include "../utils/types.h"

/**
 * @namespace bitboard
 *
 * @brief
 * 64 bit board and various helper functions
 */
namespace bboard
{

typedef Uint64 U64;

#define C64(BitBoard) BitBoard##ULL
#define Rank(x) ((x)>>3)
#define File(x) ((x)&7)
#define Flip(x) ((x)^1)
#define FileDistance(a, b) std::abs(File(a) - File(b))
#define RankDistance(a, b) std::abs(Rank(a) - Rank(b))
#define Distance(a, b) std::max(FileDistance(a,b), RankDistance(a,b))
#define SetMask(sq) (C64(1) << sq)

const int rook_bits[64] =
{
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

const int bishop_bits[64] =
{
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

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

/// Returns the rook's mask across the bitboard given the square
U64 RookMask(int sq);

/// Returns the bishop's mask across the bitboard given the square
U64 BishopMask(int sq);

/// Returns the rook's attack field given the blocking bitboard
U64 RookAttack(int sq, U64 block);

/// Returns the bishop's attack field given the blocking bitboard
U64 BishopAttack(int sq, U64 block);

/// Returns the knight move locations for all bits that are set to 1
U64 KnightAttack(U64 inKnights);

/// Returns index to a move table, it's where the magic happens
/// See http://www.rivalchess.com/magic-bitboards/ for details
int Transform(U64 inBoard, U64 inMagic, int inBits);

/// ...
U64 Index2U64(int inIndex, int inBits, U64 inMask);

/// Pops off the least significant bit of the board and returns index
int PopLSB(U64 &inBoard);

/// Find the index to the least significant bit using a debruin magic number
int LSBIndex(U64 inBoard);

/// Count the number of set bits on a bitboard
int CountBits(U64 inBoard);

/// Representation of a bitboard
std::string ToString(const U64 &inBoard);

/// Print a bitboard to stdout
void Print(const U64 &inBoard);

} // namespace bboard

#endif // BITBOARD_H
