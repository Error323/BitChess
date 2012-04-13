#ifndef ASEARCH_H
#define ASEARCH_H

#include "ttable.h"
#include "state.h"

#define INF Value(10000)

namespace asearch {

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

  /// Minimax algorithm, searches entire game tree
  Move Minimax(State *inState, int inMaxDepth);

  /// Minimax algorithm, with alpha beta pruning
  Move AlphaBeta(State *inState, int inMaxDepth);

  /// Minimax algorithm, with alpha beta pruning and search window
  Move Negascout(State *inState, int inMaxDepth);

private:
  int mStatesVisited; ///< Number of states traversed

  /// Recursive subfunction
  Value MinimaxValue(State *inState, int inDepth);

  /// Recursive subfunction
  Value AlphaBetaValue(State *inState, int inPly, int inDepth, Value inAlpha, Value inBeta);

  /// Recursive subfunction
  Value NegascoutValue(State *inState, int inPly, int inDepth, Value inAlpha, Value inBeta);
};

} // namespace asearch

#endif // ASEARCH_H
