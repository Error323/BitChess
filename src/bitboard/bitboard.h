#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include "../utils/types.h"

/**
 * @namespace bitboard
 *
 * @brief
 * 64 bit board and various helper functions
 */
namespace bboard {

typedef Uint64 U64;
#define C64(BitBoard) BitBoard##ULL

/// Returns index to a table, it's where the magic happens
/// See http://www.rivalchess.com/magic-bitboards/ for details
int Transform(U64 inBoard, U64 inMagic, int inBits);

/// ...
U64 Index2U64(int inIndex, int inBits, U64 inMask);

/// Pops off the least significant bit of the board and returns index
Uint8 PopLSB(U64 &inBoard);

/// Find the index to the least significant bit using a debruin magic number
Uint8 LSBIndex(U64 inBoard);

/// Count the number of set bits on a bitboard
int CountBits(U64 inBoard);

/// Representation of a bitboard
std::string ToString(const U64 &inBoard);

/// Print a bitboard to stdout
void Print(const U64 &inBoard);

} // namespace bboard

#endif // BITBOARD_H
