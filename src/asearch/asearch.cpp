#include "asearch.h"
#include "state.h"

#include "../utils/verbose.h"

#include <vector>

namespace asearch {

#define INF 100000

ASearch::ASearch() {}
ASearch::~ASearch() {}

//------------------------------------------------------------------------------
// MINIMAX
//------------------------------------------------------------------------------
Pair ASearch::Minimax(State *inState, int inMaxPly)
{
  mStatesVisited = 0;
  Pair best = std::make_pair<Value, Move>(-INF, -1);

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value val = -MinimaxValue(inState, inMaxPly - 1);
    inState->UndoMove(moves[i]);

    if (val > best.first)
    {
      best.first = val;
      best.second = moves[i];
    }
  }
  DebugLine("Minimax states visited:   " << mStatesVisited);

  if (best.second == -1)
    FatalLine("Invalid move returned.");
  return best;
}

Value ASearch::MinimaxValue(State *inState, int inMaxPly)
{
  if (inMaxPly == 0 || inState->IsTerminal())
    return inState->GetScore();

  mStatesVisited++;

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
  mStatesVisited = 0;
  Pair best = std::make_pair<Value, Move>(-INF, -1);

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value val = -AlphaBetaValue(inState, inMaxPly - 1, -INF, -best.first);
    inState->UndoMove(moves[i]);

    if (val > best.first)
    {
      best.first = val;
      best.second = moves[i];
    }
  }
  DebugLine("AlphaBeta states visited: " << mStatesVisited);

  if (best.second == -1)
    FatalLine("Invalid move returned.");
  return best;
}

Value ASearch::AlphaBetaValue(State *inState, int inMaxPly, int inAlpha,
                              int inBeta)
{
  if (inMaxPly == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta);

  mStatesVisited++;

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

//------------------------------------------------------------------------------
// NEGASCOUT
//------------------------------------------------------------------------------
Pair ASearch::Negascout(State *inState, int inMaxPly)
{
  mStatesVisited = 0;
  Pair best = std::make_pair<Value, Move>(-INF, -1);

  int beta = INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value val = -NegascoutValue(inState, inMaxPly - 1, -beta, -best.first);
    if (i > 0 && best.first < val && val < INF)
      val = -NegascoutValue(inState, inMaxPly - 1, -INF, -best.first);
    inState->UndoMove(moves[i]);

    if (val > best.first)
    {
      best.first = val;
      best.second = moves[i];
    }
    beta = best.first + 1;
  }
  DebugLine("Negascout states visited: " << mStatesVisited);

  if (best.second == -1)
    FatalLine("Invalid move returned.");
  return best;
}

Value ASearch::NegascoutValue(State *inState, int inMaxPly, int inAlpha,
                              int inBeta)
{
  if (inMaxPly == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta);

  mStatesVisited++;

  int beta = inBeta;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value val = -NegascoutValue(inState, inMaxPly - 1, -beta, -inAlpha);
    if (i > 0 && inAlpha < val && val < inBeta)
      val = -NegascoutValue(inState, inMaxPly - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);

    inAlpha = std::max<int>(inAlpha, val);
    if (inAlpha >= inBeta)
      return inAlpha;

    beta = inAlpha + 1;
  }

  return inAlpha;
}

} // namespace asearch
