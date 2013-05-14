#ifndef CMOVE_H
#define CMOVE_H

#include "../utils/types.h"
#include "cstate.h"

class CMove {
public:
  enum Type {
    QUIET_MOVE           = 0  << 12,
    DOUBLE_PAWN_PUSH     = 1  << 12,
    CAPTURES             = 2  << 12,
    EP_CAPTURE           = 3  << 12,
    KNIGHT_PROMOTION     = 4  << 12,
    BISHOP_PROMOTION     = 5  << 12,
    ROOK_PROMOTION       = 6  << 12,
    QUEEN_PROMOTION      = 7  << 12,
    KNIGHT_PROMO_CAPTURE = 8  << 12,
    BISHOP_PROMO_CAPTURE = 9  << 12,
    ROOK_PROMO_CAPTURE   = 10 << 12,
    QUEEN_PROMO_CAPTURE  = 11 << 12,
    KING_CASTLE          = 12 << 12,
    QUEEN_CASTLE         = 13 << 12
  };

  enum From {
    FROM_PAWN            = CState::PAWN << 16,
    FROM_KNIGHT          = CState::KNIGHT << 16,
    FROM_BISHOP          = CState::BISHOP << 16,
    FROM_ROOK            = CState::ROOK << 16,
    FROM_QUEEN           = CState::QUEEN << 16,
    FROM_KING            = CState::KING << 16
  };

  enum To {
    TO_PAWN              = CState::PAWN << 19,
    TO_KNIGHT            = CState::KNIGHT << 19,
    TO_BISHOP            = CState::BISHOP << 19,
    TO_ROOK              = CState::ROOK << 19,
    TO_QUEEN             = CState::QUEEN << 19,
    TO_KING              = CState::KING << 19
  };

  CMove(): move(0ul) {}
  CMove(U32 from, U32 to, U32 flags): move(flags|(from<<6)|to) {}

  inline U32 GetTo() const {return move & 0x3f;}
  inline U32 GetFrom() const {return (move >> 6) & 0x3f;}
  inline U32 GetType() const {return (move >> 12) & 0xf;}
  inline U32 GetFromPiece() const {return (move >> 16) & 0x7;}
  inline U32 GetToPiece() const {return (move >> 19) & 0x7;}

  inline bool IsCapture() const {return GetType() == 2;}
  inline bool IsCastle() const {return GetType() == 12 || GetType() == 13;}
  inline bool IsEnPassant() const {return GetType() == 3;}
  inline bool IsDoublePawnPush() const {return GetType() == 1;}
  inline bool IsPromotion() const {return GetType() >= 4 && GetType() <= 7;}
  inline bool IsPromotionAndCapture() const {return GetType() >= 8 && GetType() <= 11;}

protected:
  U32 move; // Using first 22 bits
};

#endif // CMOVE_H
