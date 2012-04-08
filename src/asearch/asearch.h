#ifndef ASEARCH_H
#define ASEARCH_H

#include "ttable.h"
#include "state.h"

#define INF 100000

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
  Pair Minimax(State *inState, int inMaxPly);

  /// Minimax algorithm, with alpha beta pruning
  Pair AlphaBeta(State *inState, int inMaxPly);

  /// Minimax algorithm, with alpha beta pruning and search window
  Pair Negascout(State *inState, int inMaxPly);

private:
  int mStatesVisited; ///< Number of states traversed

  /// Recursive subfunction
  Value MinimaxValue(State *inState, int inMaxPly);

  /// Recursive subfunction
  Value AlphaBetaValue(State *inState, int inMaxPly, int inAlpha, int inBeta);

  /// Recursive subfunction
  Value NegascoutValue(State *inState, int inMaxPly, int inAlpha, int inBeta);
};

} // namespace asearch

#endif // ASEARCH_H
