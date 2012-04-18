#include "cstate.h"
#include "magics.h"

#include <sstream>

U64 CState::sBMoves[64][4096];
U64 CState::sRMoves[64][4096];

CState::CState()
{
  mPieces[WHITE][EMPTY]  = C64(0xFFFFFFFFFFFF0000);
  mPieces[WHITE][PAWN]   = C64(0x000000000000FF00);
  mPieces[WHITE][KNIGHT] = C64(0x0000000000000042);
  mPieces[WHITE][BISHOP] = C64(0x0000000000000024);
  mPieces[WHITE][ROOK]   = C64(0x0000000000000081);
  mPieces[WHITE][QUEEN]  = C64(0x0000000000000008);
  mPieces[WHITE][KING]   = C64(0x0000000000000010);

  mPieces[BLACK][EMPTY]  = C64(0x0000FFFFFFFFFFFF);
  mPieces[BLACK][PAWN]   = C64(0x00FF000000000000);
  mPieces[BLACK][KNIGHT] = C64(0x4200000000000000);
  mPieces[BLACK][BISHOP] = C64(0x2400000000000000);
  mPieces[BLACK][ROOK]   = C64(0x8100000000000000);
  mPieces[BLACK][QUEEN]  = C64(0x0800000000000000);
  mPieces[BLACK][KING]   = C64(0x1000000000000000);

  mEP = -1;
  mFlags = 0;
  mSide = WHITE;
  mIsCastled[WHITE] = mIsCastled[BLACK] = false;
}

std::vector<Move> CState::GetLegalMoves()
{
  std::vector<Move> moves;
  return moves;
}

void CState::MakeMove(Move inMove)
{
  (void) inMove;
}

void CState::UndoMove(Move inMove)
{
  (void) inMove;
}

void CState::CreateHash()
{

}

bool CState::IsLegalMove(Move inMove)
{
  (void) inMove;
  return false;
}

bool CState::IsMateScore(Score inScore)
{
  (void) inScore;
  return false;
}

State::TerminalType CState::IsTerminal()
{
  return NONE;
}

std::string CState::ToString()
{
  static std::vector<U64> bit_pos_array;
  if (bit_pos_array.empty())
  {
    bit_pos_array.resize(64, C64(0));
    for (int i = 0; i < 64; i++)
      bit_pos_array[i] |= (C64(1) << i);
  }

  static std::vector<std::string> algebraic_notation;
  if (algebraic_notation.empty())
  {
    for (char file = '1'; file <= '8'; file++)
    {
      for (char rank = 'a'; rank <= 'h'; rank++)
      {
        std::string s = "";
        s += rank;
        s += file;
        algebraic_notation.push_back(s);
      }
    }
  }

  std::stringstream ss;
  int r, c, sq, spaces = 2;

  ss << "\n";
  if (mSide == WHITE)
    ss << "White  C:";
  else
    ss << "Black  C:";

  if      (mFlags & WHITE_KING_CASTLE)  {ss << "K"; spaces--;}
  else if (mFlags & WHITE_QUEEN_CASTLE) {ss << "Q"; spaces--;}

  if      (mFlags & BLACK_KING_CASTLE)  {ss << "k"; spaces--;}
  else if (mFlags & BLACK_QUEEN_CASTLE) {ss << "q"; spaces--;}

  for (int i = 0; i < spaces; i++)
    ss << " ";
  ss << "  EP:";
  if (mEP > -1)
    ss << algebraic_notation[mEP];

  ss << "\n\n";
  for (r = 56; r >= 0; r -= 8)
  {
    ss << (r/8+1) << "  ";
    for (c = 0; c < 8; c++)
    {
      sq = r + c;
      if (     mPieces[WHITE][PAWN]   & bit_pos_array[sq])
        ss << "P ";
      else if (mPieces[WHITE][KNIGHT] & bit_pos_array[sq])
        ss << "N ";
      else if (mPieces[WHITE][BISHOP] & bit_pos_array[sq])
        ss << "B ";
      else if (mPieces[WHITE][ROOK]   & bit_pos_array[sq])
        ss << "R ";
      else if (mPieces[WHITE][QUEEN]  & bit_pos_array[sq])
        ss << "Q ";
      else if (mPieces[WHITE][KING]   & bit_pos_array[sq])
        ss << "K ";
      else if (mPieces[BLACK][PAWN]   & bit_pos_array[sq])
        ss << "p ";
      else if (mPieces[BLACK][KNIGHT] & bit_pos_array[sq])
        ss << "n ";
      else if (mPieces[BLACK][BISHOP] & bit_pos_array[sq])
        ss << "b ";
      else if (mPieces[BLACK][ROOK]   & bit_pos_array[sq])
        ss << "r ";
      else if (mPieces[BLACK][QUEEN]  & bit_pos_array[sq])
        ss << "q ";
      else if (mPieces[BLACK][KING]   & bit_pos_array[sq])
        ss << "k ";
      else
        ss << ". ";
    }
    ss << "\n";
  }
  ss << "\n   a b c d e f g h\n\n";
  return ss.str();
}

Score CState::GetScore(int inPly)
{
  (void) inPly;
  Score score = 0;
  return score;
}

Score CState::Quiescence(Score inAlpha, Score inBeta, int inPly)
{
  (void) inAlpha;
  (void) inBeta;
  (void) inPly;
  Score score = 0;
  return score;
}

U64 rmask(int sq)
{
  U64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
  for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
  for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
  for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
  return result;
}

U64 bmask(int sq)
{
  U64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
  for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
  for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
  for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
  return result;
}

U64 ratt(int sq, U64 block)
{
  U64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 7; r++) {
    result |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  }
  for(r = rk-1; r >= 0; r--) {
    result |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  }
  for(f = fl+1; f <= 7; f++) {
    result |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  }
  for(f = fl-1; f >= 0; f--) {
    result |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  }
  return result;
}

U64 batt(int sq, U64 block)
{
  U64 result = 0ULL;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
    result |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  }
  return result;
}


Uint8 transform(U64 b, U64 magic, int bits)
{
  return (Uint8)((b * magic) >> (64 - bits));
}

void CState::InitializeChessState()
{
  const int table_size = 8000*2; ///< 8000 entries per player
  const int hash_codes = 2*7 + 4 + 2 + 1; ///< pieces, castletypes, ep, side
  CState::Initialize(table_size, hash_codes);

  U64 mask;
  int n;
  for (Uint8 sq = 0; sq < 64; sq++)
  {
    mask = bmask(sq);
    n = CountBits(mask);
    for (int i = 0; i < (1 << n); i++)
    {
      U64 block = Index2U64(i, n, mask);
      sBMoves[sq][i] = batt(sq, block);
    }

    mask = rmask(sq);
    n = CountBits(mask);
    for (int i = 0; i < (1 << n); i++)
    {
      U64 block = Index2U64(i, n, mask);
      sRMoves[sq][i] = ratt(sq, block);
    }
  }
}
