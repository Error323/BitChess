#ifndef CSTATE_H
#define CSTATE_H

#include "../asearch/state.h"

#include <vector>

using namespace asearch;

/**
 * @class CState
 *
 * @brief
 * Chess state represents the game state of chess
 */
class CState : public State
{
  friend class CStateTest;

public:
  enum Side
  {
    WHITE,
    BLACK
  };

  enum Piece
  {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
  };

  enum Castling
  {
    WHITE_KING_CASTLE  = (1 << 0),
    WHITE_QUEEN_CASTLE = (1 << 1),
    BLACK_KING_CASTLE  = (1 << 2),
    BLACK_QUEEN_CASTLE = (1 << 3)
  };

  CState();
  ~CState() {}

  /// Return all legal moves (indices to a move table defined elsewhere)
  std::vector<Move> GetLegalMoves();

  /// Return wether a move is legal for this state or not
  bool IsLegalMove(Move inMove);

  /// Apply a move on the current game state
  void MakeMove(Move inMove);

  /// Undo a move on the current game state
  void UndoMove(Move inMove);

  /// Represent state as string
  std::string ToString();

  /// Create hashkey from current state
  void CreateHash();

  /// Determine wether state is winning or losing
  bool IsMateScore(Score inScore);

  /// Determine whether a state is terminal (i.e. game over)
  TerminalType IsTerminal();

  /// Return the current heuristic value of the state for a player
  Score GetScore(int inPly);

  /// Return the Quiescence heuristic value of the state given alpha and beta
  Score Quiescence(Score inAlpha, Score inBeta, int inPly);

  /// Initialize chessboard from fen string
  void FromFen(const std::string &fen);

  /// Initialize all moves
  static void InitializeChessState();

private:
  U64 mPieces[2][7]; ///< Piece positions (white, black) (pawn,...,king)
  U64 mFriends[2]; ///< Friendly positions (white, black)
  U64 mOccupied; ///< All occupied squares
  I8 mEP; ///< Location of en passant square
  U8 mFlags; ///< Castling flags
  Side mSide; ///< Side that has to move (white, black)
  U32 mPly; ///< Current half move
} __attribute__((aligned));

#endif // CSTATE_H
