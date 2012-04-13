#ifndef STATE_H
#define STATE_H

#include "ttable.h"

#include <vector>

namespace asearch {

/**
 * @abstract State
 *
 * @brief
 * State represents the game state
 */
class State : public TTable
{
public:
  State() {}
  virtual ~State() {}

  /// Return all legal moves (indices to a move table defined elsewhere)
  virtual std::vector<Move> GetLegalMoves() = 0;

  /// Apply a move on the current game state
  virtual void MakeMove(Move inMove) = 0;

  /// Undo a move on the current game state
  virtual void UndoMove(Move inMove) = 0;

  /// Determine whether a state is terminal (i.e. game over)
  virtual bool IsTerminal() = 0;

  /// Return the current heuristic value of the state for a player
  virtual Value GetScore() = 0;

  /// Return the Quiescence heuristic value of the state given alpha and beta
  virtual Value Quiescence(Value inAlpha, Value inBeta) = 0;
};

} // namespace asearch

#endif // STATE_H
