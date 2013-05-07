#include "cstate.h"

#include "../bitboard/bitboard.h"
#include "magics.h"
#include "../utils/debugger.h"

#include <sstream>

CState::CState()
{
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
  U32 table_size = 128; // 128 megabyte
  U32 hash_codes = 2 * 6 + 4 + 17 + 2; // pieces, castletypes, ep, side
  CState::Initialize(table_size, hash_codes);
  magics::Initialize();
}
