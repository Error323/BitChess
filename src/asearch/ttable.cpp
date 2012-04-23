#include "ttable.h"

#include <cstring>
#include <cmath>

#include "../utils/random.c"
#include "../utils/verbose.h"

extern uint64_t Rand64();

namespace asearch
{

Uint32 TTable::sHits = 0;
Uint32 TTable::sSuccessfullHits = 0;
Uint32 TTable::sCollisions = 0;
Uint32 TTable::sHashSize = 0;
HashType TTable::sHashMask = (HashType) 0;

std::vector<HashType> TTable::sHashCodes;
std::vector<TTable::Entry> TTable::sHashTable;

TTable::~TTable()
{
  WarningLine("Hits:             " << sHits);
  WarningLine("Successfull hits: " << sSuccessfullHits);
  WarningLine("Collisions:       " << sCollisions);
}

void TTable::Initialize(int inTableSize, int inNumHashCodes)
{
  sHashTable.resize(inTableSize);
  sHashCodes.resize(inNumHashCodes);
  Reset();
  for (int i = 0; i < inNumHashCodes; i++)
    sHashCodes[i] = Rand64();
  sHashMask = HashType(0);
  while ((inTableSize >>= 1) > 0)
  {
    sHashMask <<= 1;
    sHashMask |= 1;
  }
  sHashSize = sHashMask + 1;
  DebugLine("Allocated hashtable of " << round(sHashSize * sizeof(Entry) / 1024.0) << "K.");
}

void TTable::Reset()
{
  sHits = 0;
  sSuccessfullHits = 0;
  sCollisions = 0;
  memset(&sHashTable[0], 0, sizeof(Entry)*sHashTable.size());
}

std::string TTable::PrintHashType(HashType inHashType)
{
  std::string s;
  s.resize(64);
  for (int i = 0; i < 64; i++)
    s[63 - i] = ((HashType(1) << HashType(i)) & inHashType) ? '1' : '0';
  return s;
}

void TTable::Put(Uint8 inDepth, Uint8 inPly, Score inAlpha, Score inBeta, Move inMove, Score inScore)
{
  Entry &entry = sHashTable[(mHashKey & sHashMask)];
  if (entry.mFlag && entry.mKey != mHashKey)
    sCollisions++;
  entry.mMove = inMove;
  entry.mValue = inScore;
  entry.mKey = mHashKey;
  entry.mDepth = inDepth;
  bool is_mate_score = IsMateScore(inScore);
  if (inDepth == 0 && !is_mate_score)
    entry.mFlag = QUIESCENT;
  else if (inScore >= inBeta)
    entry.mFlag = LOWERBOUND;
  else if (inScore <= inAlpha)
    entry.mFlag = UPPERBOUND;
  else
    entry.mFlag = EXACT;
  if (is_mate_score)
    entry.mValue += (inScore > 0) ? inPly : -inPly;
}

TTable::Flag TTable::Get(Uint8 inDepth, Uint8 inPly, Move &outMove, Score &outScore)
{
  sHits++;
  Entry &entry = sHashTable[(mHashKey & sHashMask)];
  if (mHashKey != entry.mKey)
    return INVALID;
  sSuccessfullHits++;
  outScore = entry.mValue;
  outMove = entry.mMove;
  bool is_mate_score = IsMateScore(outScore);
  if (entry.mDepth == 0)
    return QUIESCENT;
  if (entry.mDepth < inDepth && !is_mate_score)
    return OUTDATED;
  if (is_mate_score)
    outScore -= (outScore > 0 ? inPly : -inPly);
  return Flag(entry.mFlag);
}

} // namespace asearch
