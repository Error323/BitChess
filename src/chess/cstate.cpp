#include "cstate.h"

#include "magics.h"
#include "../bitboard/bitboard.h"
#include "../utils/debugger.h"
#include "../utils/verbose.h"

#include <sstream>

CState::CState()
{
  FromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
  std::stringstream ss;
  int r, f, sq, spaces = 5;
  ss << "\n";
  if (mSide == WHITE)
    ss << Verbose::Colorize("W", Verbose::WHITE, Verbose::BOLD) << " C:";
  else
    ss << Verbose::Colorize("B", Verbose::RED, Verbose::BOLD) << " C:";

  if (mFlags & WHITE_KING_CASTLE)
  {
    ss << Verbose::Colorize("K", Verbose::WHITE);
    spaces--;
  }
  if (mFlags & WHITE_QUEEN_CASTLE)
  {
    ss << Verbose::Colorize("Q", Verbose::WHITE);
    spaces--;
  }
  if (mFlags & BLACK_KING_CASTLE)
  {
    ss << Verbose::Colorize("K", Verbose::RED);
    spaces--;
  }
  if (mFlags & BLACK_QUEEN_CASTLE)
  {
    ss << Verbose::Colorize("Q", Verbose::RED);
    spaces--;
  }
  for (int i = 0; i < spaces; i++)
    ss << " ";
  ss << "EP:";
  if (mEP > 0)
    ss << char(mEP%8+65) << int(mEP/8+1);
  else
    ss << "  ";
  ss << " M:" << mPly;
  ss << "\n\n";
  for (r = 56; r >= 0; r -= 8)
  {
    ss << (r / 8 + 1) << "  ";
    for (f = 0; f < 8; f++)
    {
      sq = r + f;
      if      (mPieces[WHITE][PAWN]   & Set(sq)) ss << Verbose::Colorize("P ", Verbose::WHITE);
      else if (mPieces[WHITE][KNIGHT] & Set(sq)) ss << Verbose::Colorize("N ", Verbose::WHITE);
      else if (mPieces[WHITE][BISHOP] & Set(sq)) ss << Verbose::Colorize("B ", Verbose::WHITE);
      else if (mPieces[WHITE][ROOK]   & Set(sq)) ss << Verbose::Colorize("R ", Verbose::WHITE);
      else if (mPieces[WHITE][QUEEN]  & Set(sq)) ss << Verbose::Colorize("Q ", Verbose::WHITE);
      else if (mPieces[WHITE][KING]   & Set(sq)) ss << Verbose::Colorize("K ", Verbose::WHITE);
      else if (mPieces[BLACK][PAWN]   & Set(sq)) ss << Verbose::Colorize("P ", Verbose::RED);
      else if (mPieces[BLACK][KNIGHT] & Set(sq)) ss << Verbose::Colorize("N ", Verbose::RED);
      else if (mPieces[BLACK][BISHOP] & Set(sq)) ss << Verbose::Colorize("B ", Verbose::RED);
      else if (mPieces[BLACK][ROOK]   & Set(sq)) ss << Verbose::Colorize("R ", Verbose::RED);
      else if (mPieces[BLACK][QUEEN]  & Set(sq)) ss << Verbose::Colorize("Q ", Verbose::RED);
      else if (mPieces[BLACK][KING]   & Set(sq)) ss << Verbose::Colorize("K ", Verbose::RED);
      else if (BLACK_SQUARES & Set(sq))          ss << Verbose::Colorize(". ", Verbose::RED);
      else                                       ss << Verbose::Colorize(". ", Verbose::WHITE);
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

void CState::FromFen(const std::string &fen)
{

}

void CState::InitializeChessState()
{
  U32 table_size = 128; // 128 megabyte
  U32 hash_codes = 2 * 6 + 4 + 17 + 2; // pieces, castletypes, ep, side
  CState::Initialize(table_size, hash_codes);
  magics::Initialize();
}
