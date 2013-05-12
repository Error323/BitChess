#include "magics.h"
#include "../utils/debugger.h"
#include "../utils/verbose.h"

namespace magics {

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

U64 Index2U64(const int index, const int bits, U64 mask)
{
  int i, j;
  U64 result = C64(0);
  for (i = 0; i < bits; i++)
  {
    j = PopLSB(mask);
    if (index & (1 << i)) result |= (C64(1) << j);
  }
  return result;
}

int Transform(U64 board, const U64 magic)
{
  board *= magic;
  board >>= (magic >> 58);
  return static_cast<int>(board);
}

/// Returns attacks of the rook given occupancy board and square
U64 RookAttacks(U64 board, const int square)
{
  board  &= sRookMagics[square].premask;
  board  *= sRookMagics[square].magic;
  board >>= sRookMagics[square].magic >> 58;

  return sRookMagics[square].postmask &
         sRookMagics[square].location[board];
}

/// Returns attacks of the bishop given occupancy board and square
U64 BishopAttacks(U64 board, const int square)
{
  board  &= sBishopMagics[square].premask;
  board  *= sBishopMagics[square].magic;
  board >>= sBishopMagics[square].magic >> 58;

  return sBishopMagics[square].postmask &
         sBishopMagics[square].location[board];
}

void Initialize()
{
  if (!sAttackTable.empty())
    return;

  // Compute pre and postmasks of both bishop and rook
  for (int i = 0, bits; i < 64; i++)
  {
    sBishopMagics[i].premask = BishopMask(i);
    sBishopMagics[i].magic = bishop_magics[i];
    sBishopMagics[i].postmask = BishopAttack(i, C64(0));
    bits = 64 - (sBishopMagics[i].magic >> 58);
    ASSERT(bits <= CountBits(sBishopMagics[i].premask));

    sRookMagics[i].premask = RookMask(i);
    sRookMagics[i].magic = rook_magics[i];
    sRookMagics[i].postmask = RookAttack(i, C64(0));
    bits = 64 - (sRookMagics[i].magic >> 58);
    ASSERT(bits <= CountBits(sRookMagics[i].premask));
  }

  // Allocate attack table for bishop
  for (int i = 0, size; i < 16; i++)
  {
    size = 0;
    for (int j = 0; j < 64; j++)
      if (i == sBishopSharing[j])
        size = std::max(size, int(64-(sBishopMagics[j].magic >> 58)));
    sAttackTable.resize(sAttackTable.size() + (1<<size));
  }

  // Allocate attack table for rook
  for (int i = 0, size; i < 32; i++)
  {
    size = 0;
    for (int j = 0; j < 64; j++)
      if (i == sRookSharing[j])
        size = std::max(size, int(64-(sRookMagics[j].magic >> 58)));
    sAttackTable.resize(sAttackTable.size() + (1<<size));
  }

  sAttackTable.assign(sAttackTable.size(), C64(0));

  // Add bishops to the attack table
  size_t attack_index = 0;
  for (int i = 0, size; i < 16; i++)
  {
    std::vector<int> squares;
    size = 0;
    for (int j = 0; j < 64; j++)
      if (i == sBishopSharing[j])
      {
        size = std::max(size, int(64-(sBishopMagics[j].magic >> 58)));
        squares.push_back(j);
      }


    for (size_t k = 0, bits, n, index, j, square; k < squares.size(); k++)
    {
      square = squares[k];
      sBishopMagics[square].location = &sAttackTable[attack_index];
      bits = CountBits(sBishopMagics[square].premask);
      n = 1 << bits;
      U64 attack, block;

      for (j = 0; j < n; j++)
      {
        block = Index2U64(j, bits, sBishopMagics[square].premask);
        index = Transform(block, sBishopMagics[square].magic);
        attack = BishopAttack(square, block);
        sAttackTable[attack_index + index] |= attack;
      }
    }

    attack_index += (1 << size);
  }

  // Add rooks to the attack table
  for (int i = 0, size; i < 32; i++)
  {
    std::vector<int> squares;
    size = 0;
    for (int j = 0; j < 64; j++)
      if (i == sRookSharing[j])
      {
        size = std::max(size, int(64-(sRookMagics[j].magic >> 58)));
        squares.push_back(j);
      }


    for (size_t k = 0, bits, n, index, j, square; k < squares.size(); k++)
    {
      square = squares[k];
      sRookMagics[square].location = &sAttackTable[attack_index];
      bits = CountBits(sRookMagics[square].premask);
      n = 1 << bits;
      U64 attack, block;

      for (j = 0; j < n; j++)
      {
        block = Index2U64(j, bits, sRookMagics[square].premask);
        index = Transform(block, sRookMagics[square].magic);
        attack = RookAttack(square, block);
        sAttackTable[attack_index + index] |= attack;
      }
    }

    attack_index += (1 << size);
  }

  ASSERT(attack_index == sAttackTable.size());
  DebugLine("Allocated attack table of " << (sAttackTable.size()*sizeof(U64)/1024.0f) << "KB (" << sAttackTable.size() << " entries)");
}

}
