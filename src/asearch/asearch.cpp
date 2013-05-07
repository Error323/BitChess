#include "asearch.h"
#include "state.h"

#include "../utils/debugger.h"
#include "../utils/verbose.h"

#include <ctime>
#include <vector>
#include <algorithm>

namespace asearch
{

ASearch::ASearch() {}
ASearch::~ASearch() {}

//------------------------------------------------------------------------------
// ITERATIVE DEEPENING
//------------------------------------------------------------------------------
Move ASearch::Iterate(State *inState, int inMaxDepth, int inTime)
{
  mStatesVisited = 0;
  int max_depth = 0;
  clock_t start_time = clock();
  clock_t elapsed_time = 0;
  Move best_move = -1;
  std::vector<Move> moves = inState->GetLegalMoves();
  mRootMoves.clear();
  for (int i = 0, n = moves.size(); i < n; i++)
    mRootMoves.push_back(std::make_pair(-INF, moves[i]));

  while (max_depth <= inMaxDepth && elapsed_time < inTime)
  {
    max_depth++;
    best_move = Negascout(inState, max_depth);
    elapsed_time = difftime(start_time, clock()) * CLOCKS_PER_SEC;
  }
  DebugLine("Negascout states visited: " << mStatesVisited);
  return best_move;
}

//------------------------------------------------------------------------------
// MINIMAX
//------------------------------------------------------------------------------
Move ASearch::Minimax(State *inState, int inMaxDepth)
{
  mStatesVisited = 0;
  Move best_move = -1;
  Score best_score = -INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Score score = -MinimaxValue(inState, inMaxDepth - 1, 2);
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

Score ASearch::MinimaxValue(State *inState, int inDepth, int inPly)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->GetScore(inPly);
  mStatesVisited++;
  Score best_score = -INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Score score = -MinimaxValue(inState, inDepth - 1, inPly + 1);
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
  Score best_score = -INF;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(moves[i]);
    Score score = -AlphaBetaValue(inState, 2, inMaxDepth - 1, -INF, -best_score);
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

Score ASearch::AlphaBetaValue(State *inState, int inPly, int inDepth, Score inAlpha,
                              Score inBeta)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta, inPly);
  mStatesVisited++;
  std::vector<Move> moves = inState->GetLegalMoves();
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    Score score = -AlphaBetaValue(inState, inPly + 1, inDepth - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);
    inAlpha = std::max<Score>(score, inAlpha);
    if (inAlpha >= inBeta)
      break;
  }
  return inAlpha;
}

inline bool MoveSorter(const std::pair<Score, Move> &a, const std::pair<Score, Move> &b)
{
  return a.first > b.first;
}

//------------------------------------------------------------------------------
// NEGASCOUT
//------------------------------------------------------------------------------
Move ASearch::Negascout(State *inState, int inMaxDepth)
{
  int beta = INF;
  Score best_score = -INF;
  for (int i = 0, n = mRootMoves.size(); i < n; i++)
  {
    mStatesVisited++;
    inState->MakeMove(mRootMoves[i].second);
    mRootMoves[i].first = -NegascoutValue(inState, 2, inMaxDepth - 1, -beta, -best_score);
    if (i > 0 && best_score < mRootMoves[i].first)
      mRootMoves[i].first = -NegascoutValue(inState, 2, inMaxDepth - 1, -INF, -best_score);
    inState->UndoMove(mRootMoves[i].second);
    best_score = std::max<Score>(mRootMoves[i].first, best_score);
    beta = best_score + 1;
  }
  std::sort(mRootMoves.begin(), mRootMoves.end(), MoveSorter);
  ASSERT(mRootMoves.front().first == best_score);
  return mRootMoves.front().second;
}

/// As defined at: http://en.wikipedia.org/wiki/Negascout
Score ASearch::NegascoutValue(State *inState, int inPly, int inDepth,
                              Score inAlpha, Score inBeta)
{
  if (inDepth == 0 || inState->IsTerminal())
    return inState->Quiescence(inAlpha, inBeta, inPly);
  mStatesVisited++;
  Move move;
  Score score;
  TTable::Flag flag = inState->Get(inDepth, inPly, move, score);
  Score beta = inBeta; // store initial search window
  switch (flag)
  {
  case TTable::EXACT:
    return score;
  case TTable::UPPERBOUND:
    beta = std::min<Score>(beta, score);
    break;
  case TTable::LOWERBOUND:
    inAlpha = std::max<Score>(inAlpha, score);
    break;
  default:
    break;
  }
  if (inAlpha >= inBeta)
    return score;
  std::vector<Move> moves = inState->GetLegalMoves();
  // If hash found, switch first move
  if (flag)
  {
    std::vector<Move>::iterator i = std::find(moves.begin(), moves.end(), move);
    std::swap<Move>(*i, moves[0]);
  }
  // Apply moves
  for (int i = 0, n = moves.size(); i < n; i++)
  {
    inState->MakeMove(moves[i]);
    move = moves[i];
    score = -NegascoutValue(inState, inPly + 1, inDepth - 1, -beta, -inAlpha);
    if (i > 0 && inAlpha < score && score < inBeta)
      score = -NegascoutValue(inState, inPly + 1, inDepth - 1, -inBeta, -inAlpha);
    inState->UndoMove(moves[i]);
    inAlpha = std::max<Score>(inAlpha, score);
    if (inAlpha >= inBeta)
      break;
    beta = inAlpha + 1;
  }
  inState->Put(inDepth, inPly, inAlpha, beta, move, score);
  return inAlpha;
}

} // namespace asearch
