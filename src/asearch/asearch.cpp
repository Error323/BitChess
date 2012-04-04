#include "asearch.h"
#include "state.h"

#include <vector>
#include <cassert>

namespace asearch {

#define INF 100000

ASearch::ASearch() {}
ASearch::~ASearch() {}

//------------------------------------------------------------------------------
// MINIMAX
//------------------------------------------------------------------------------
Pair ASearch::Minimax(State *inState, int inMaxPly)
{
  Pair best = std::make_pair<Value, Move>(-INF, -1);

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value val = -MinimaxValue(inState, inMaxPly - 1);
    inState->UndoMove(moves[i]);

    if (val > best.first)
    {
      best.first = val;
      best.second = moves[i];
    }
  }

  assert(best.second != -1);
  return best;
}

Value ASearch::MinimaxValue(State *inState, int inMaxPly)
{
  if (inMaxPly == 0 || inState->IsTerminal())
    return inState->GetScore();

  Value best = -INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value val = -MinimaxValue(inState, inMaxPly - 1);
    inState->UndoMove(moves[i]);

    if (val > best)
      best = val;
  }

  return best;
}

//------------------------------------------------------------------------------
// ALPHABETA
//------------------------------------------------------------------------------
Pair ASearch::AlphaBeta(State *inState, int inMaxPly)
{
  Pair best = std::make_pair<Value, Move>(-INF, -1);

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value val = -AlphaBetaValue(inState, inMaxPly - 1, -INF, -best.first);
    inState->UndoMove(moves[i]);

    if (val > best.first)
    {
      best.first = val;
      best.second = moves[i];
    }
  }

  assert(best.second != -1);
  return best;
}

Value ASearch::AlphaBetaValue(State *inState, int inMaxPly, int inAlpha,
                              int inBeta)
{
  if (inMaxPly == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta);

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value val = -AlphaBetaValue(inState, inMaxPly - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);

    if (val >= inBeta)
      return val;

    if (val > inAlpha)
      inAlpha = val;
  }

  return inAlpha;
}

} // namespace asearch
