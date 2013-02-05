#include "cstate.h"

#include <sstream>

U64 CState::sBMask[64];
U64 CState::sRMask[64];
U64 CState::sBMoves[64][4096];
U64 CState::sRMoves[64][4096];
U64 CState::sNMoves[64];
U64 CState::sKMoves[64];

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
  for (int s = WHITE; s <= BLACK; s++)
  {
    mFriends[s] = C64(0);
    for (int p = PAWN; p <= KING; p++)
      mFriends[s] |= mPieces[s][p];
  }
  mOccupied = mFriends[WHITE] | mFriends[BLACK];
  CreateHash();
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
  if      (mFlags & WHITE_KING_CASTLE)
  {
    ss << "K";
    spaces--;
  }
  else if (mFlags & WHITE_QUEEN_CASTLE)
  {
    ss << "Q";
    spaces--;
  }
  if      (mFlags & BLACK_KING_CASTLE)
  {
    ss << "k";
    spaces--;
  }
  else if (mFlags & BLACK_QUEEN_CASTLE)
  {
    ss << "q";
    spaces--;
  }
  for (int i = 0; i < spaces; i++)
    ss << " ";
  ss << "  EP:";
  if (mEP > -1)
    ss << algebraic_notation[mEP];
  ss << "\n\n";
  for (r = 56; r >= 0; r -= 8)
  {
    ss << (r / 8 + 1) << "  ";
    for (c = 0; c < 8; c++)
    {
      sq = r + c;
      if      (mPieces[WHITE][PAWN]   & bit_pos_array[sq]) ss << "P ";
      else if (mPieces[WHITE][KNIGHT] & bit_pos_array[sq]) ss << "N ";
      else if (mPieces[WHITE][BISHOP] & bit_pos_array[sq]) ss << "B ";
      else if (mPieces[WHITE][ROOK]   & bit_pos_array[sq]) ss << "R ";
      else if (mPieces[WHITE][QUEEN]  & bit_pos_array[sq]) ss << "Q ";
      else if (mPieces[WHITE][KING]   & bit_pos_array[sq]) ss << "K ";
      else if (mPieces[BLACK][PAWN]   & bit_pos_array[sq]) ss << "p ";
      else if (mPieces[BLACK][KNIGHT] & bit_pos_array[sq]) ss << "n ";
      else if (mPieces[BLACK][BISHOP] & bit_pos_array[sq]) ss << "b ";
      else if (mPieces[BLACK][ROOK]   & bit_pos_array[sq]) ss << "r ";
      else if (mPieces[BLACK][QUEEN]  & bit_pos_array[sq]) ss << "q ";
      else if (mPieces[BLACK][KING]   & bit_pos_array[sq]) ss << "k ";
      else                                                 ss << ". ";
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

void CState::InitializeChessState()
{
  const int table_size = 8000 * 2; // 8000 entries per player
  const int hash_codes = 2 * 7 + 4 + 2 + 1; // pieces, castletypes, ep, side
  CState::Initialize(table_size, hash_codes);
  int n, j;
  for (int sq = 0; sq < 64; sq++)
  {
    // Initialize rook move table
    sRMask[sq] = RookMask(sq);
    n = CountBits(sRMask[sq]);
    for (int i = 0; i < (1 << n); i++)
    {
      U64 block = Index2U64(i, n, sRMask[sq]);
      j = Transform(block, r_magic[sq], n);
      sRMoves[sq][j] = RookAttack(sq, block);
    }

    // Initialize bishop move table
    sBMask[sq] = BishopMask(sq);
    n = CountBits(sBMask[sq]);
    for (int i = 0; i < (1 << n); i++)
    {
      U64 block = Index2U64(i, n, sBMask[sq]);
      j = Transform(block, b_magic[sq], n);
      sBMoves[sq][i] = BishopAttack(sq, block);
    }

    // Initialize knight move table
    U64 knight = C64(1) << sq;
    sKMoves[sq] = KnightAttack(knight);
  }
}
