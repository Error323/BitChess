#ifndef STATE_H
#define STATE_H

#include <utility>
#include <vector>

namespace asearch {

typedef int Move; ///< Actual move (index to a move table)
typedef int Value; ///< Minimax value
typedef std::pair<Value, Move> Pair; ///< Minimax Value and corresponding Move

/**
 * @interface State
 *
 * @brief
 * State represents the game state.
 *
 * @details
 * Defines helper functions for ASearch
 */
class State
{
public:
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
  virtual Value Quiescence(int inAlpha, int inBeta) = 0;
};

}

#endif // STATE_H
