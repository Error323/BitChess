#ifndef CSTATE_H
#define CSTATE_H

#include "../asearch/state.h"
#include "../bitboard/bitboard.h"
#include "magics.h"

#include <vector>

using namespace asearch;
using namespace bboard;

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
  enum Side {
    WHITE,
    BLACK
  };

  enum Piece {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
  };

  enum Castling {
    WHITE_KING_CASTLE  = (1 << 0),
    WHITE_QUEEN_CASTLE = (1 << 1),
    BLACK_KING_CASTLE  = (1 << 2),
    BLACK_QUEEN_CASTLE = (1 << 3)
  };

  enum Square {
    A1, B1, C1, D11, E1, F1, G1, H1,
    A2, B2, C2, D21, E2, F2, G2, H2,
    A3, B3, C3, D31, E3, F3, G3, H3,
    A4, B4, C4, D41, E4, F4, G4, H4,
    A5, B5, C5, D51, E5, F5, G5, H5,
    A6, B6, C6, D61, E6, F6, G6, H6,
    A7, B7, C7, D71, E7, F7, G7, H7,
    A8, B8, C8, D81, E8, F8, G8, H8
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

  /// Initialize all moves
  static void InitializeChessState();

private:
  U64 mPieces[2][7]; ///< Piece positions (white, black) (pawn,...,king)
  U64 mFriends[2]; ///< Friendly positions (white, black)
  U64 mOccupied; ///< All occupied squares
  Int8 mEP; ///< Location of en passant square
  Uint8 mFlags; ///< Castling flags
  bool mIsCastled[2]; ///< True if castling has occured
  Side mSide; ///< Side that has to move (white, black)

  static U64 sBMask[64]; ///< Bishop mask for all squares
  static U64 sRMask[64]; ///< Rook mask for all squares
  static U64 sBMoves[64][4096]; ///< All possible bishop moves
  static U64 sRMoves[64][4096]; ///< All possible rook moves
//  static U64 sNMoves[64]; ///< All knight moves per square
//  static U64 sKMoves[64]; ///< All king moves per square
};

#endif // CSTATE_H
