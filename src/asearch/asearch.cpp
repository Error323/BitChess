#include "asearch.h"
#include "state.h"

#include "../utils/verbose.h"

#include <vector>

namespace asearch {

ASearch::ASearch() {}
ASearch::~ASearch() {}

//------------------------------------------------------------------------------
// MINIMAX
//------------------------------------------------------------------------------
Move ASearch::Minimax(State *inState, int inMaxDepth)
{
  mStatesVisited = 0;
  Move best_move = -1;
  Value best_score = -INF;

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value score = -MinimaxValue(inState, inMaxDepth - 1);
    inState->UndoMove(moves[i]);

    if (score > best_score)
    {
      best_score = score;
      best_move = moves[i];
    }
  }
  DebugLine("Minimax states visited:   " << mStatesVisited);

  return best_move;
}

Value ASearch::MinimaxValue(State *inState, int inDepth)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->GetScore();

  mStatesVisited++;

  Value best_score = -INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value score = -MinimaxValue(inState, inDepth - 1);
    inState->UndoMove(moves[i]);

    if (score > best_score)
      best_score = score;
  }

  return best_score;
}

//------------------------------------------------------------------------------
// ALPHABETA
//------------------------------------------------------------------------------
Move ASearch::AlphaBeta(State *inState, int inMaxDepth)
{
  mStatesVisited = 0;
  Move best_move = -1;
  Value best_score = -INF;

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value score = -AlphaBetaValue(inState, 2, inMaxDepth - 1, -INF, -best_score);
    inState->UndoMove(moves[i]);

    if (score > best_score)
    {
      best_score = score;
      best_move = moves[i];
    }
  }
  DebugLine("AlphaBeta states visited: " << mStatesVisited);

  return best_move;
}

Value ASearch::AlphaBetaValue(State *inState, int inPly, int inDepth, Value inAlpha,
                              Value inBeta)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta);

  mStatesVisited++;

  /*
  TTable::Flag flag = inState->Get(inDepth, inPly, pair);
  switch (flag)
  {
    case TTable::EXACT: return pair.first;
//    case TTable::UPPERBOUND: inBeta = std::min<int>(inBeta, pair.first); break;
    case TTable::LOWERBOUND: inAlpha = std::max<int>(inAlpha, pair.first); break;
    default: break;
  }

  if (inAlpha >= inBeta)
    return inAlpha;
  */

  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Value score = -AlphaBetaValue(inState, inPly + 1, inDepth - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);

    inAlpha = std::max<Value>(score, inAlpha);

    if (inAlpha >= inBeta)
      break;
  }

//  inState->Put(inDepth, inPly, inAlpha, inBeta, pair);

  return inAlpha;
}

//------------------------------------------------------------------------------
// NEGASCOUT
//------------------------------------------------------------------------------
Move ASearch::Negascout(State *inState, int inMaxDepth)
{
  mStatesVisited = 0;
  Move best_move = -1;
  Value best_score = -INF;

  int beta = INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Value val = -NegascoutValue(inState, 2, inMaxDepth - 1, -beta, -best_score);
    if (i > 0 && best_score < val && val < INF)
      val = -NegascoutValue(inState, 2, inMaxDepth - 1, -INF, -best_score);
    inState->UndoMove(moves[i]);

    if (val > best_score)
    {
      best_score = val;
      best_move = moves[i];
    }
    beta = best_score + 1;
  }
  DebugLine("Negascout states visited: " << mStatesVisited);

  return best_move;
}

Value ASearch::NegascoutValue(State *inState, int inPly, int inDepth, Value inAlpha,
                              Value inBeta)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta);

  mStatesVisited++;

  Move move;
  Value score;
  TTable::Flag flag = inState->Get(inDepth, inPly, move, score);
  switch (flag)
  {
    case TTable::EXACT: return score;
//    case TTable::UPPERBOUND: inBeta = std::min<int>(inBeta, pair.first); break;
    case TTable::LOWERBOUND: inAlpha = std::max<Value>(inAlpha, score); break;
    default: break;
  }

  int beta = inBeta;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    move = moves[i];
    score = -NegascoutValue(inState, inPly + 1, inDepth - 1, -beta, -inAlpha);
    if (i > 0 && inAlpha < score && score < inBeta)
      score = -NegascoutValue(inState, inPly + 1, inDepth - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);

    inAlpha = std::max<int>(inAlpha, score);
    if (inAlpha >= inBeta)
      break;

    beta = inAlpha + 1;
  }

  inState->Put(inDepth, inPly, inAlpha, inBeta, move, score);

  return inAlpha;
}

} // namespace asearch
