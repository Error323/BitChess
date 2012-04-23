#ifndef ASEARCH_H
#define ASEARCH_H

#include "ttable.h"
#include "state.h"

#define INF Score(30000)

namespace asearch
{

/**
 * @class ASearch
 *
 * @brief
 * Adversarial Search functions for zero sum games:
 *  - Minimax
 *  - AlphaBeta
 *  - Principal Variation Search or Negascout
 */
class ASearch
{
public:
  ASearch();
  ~ASearch();

  /// Iterative deepening using Negascout, time in seconds
  Move Iterate(State *inState, int inMaxDepth, int inTime = 5);

  /// Minimax algorithm, searches entire game tree
  Move Minimax(State *inState, int inMaxDepth);

  /// Minimax algorithm, with alpha beta pruning
  Move AlphaBeta(State *inState, int inMaxDepth);

  /// Minimax algorithm, with alpha beta pruning and search window
  Move Negascout(State *inState, int inMaxDepth);

private:
  int mStatesVisited; ///< Number of states traversed

  /// Recursive subfunction
  Score MinimaxValue(State *inState, int inPly, int inDepth);

  /// Recursive subfunction
  Score AlphaBetaValue(State *inState, int inPly, int inDepth, Score inAlpha, Score inBeta);

  /// Recursive subfunction
  Score NegascoutValue(State *inState, int inPly, int inDepth, Score inAlpha, Score inBeta);
};

} // namespace asearch

#endif // ASEARCH_H
