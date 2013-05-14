#include "cstate.h"

#include "magics.h"
#include "../bitboard/bitboard.h"
#include "../utils/debugger.h"
#include "../utils/verbose.h"

#include <sstream>
#include <cstring>

#define FenError(fen,pos)              \
  do {                                 \
    ErrorLine("\nError: Invalid FEN"); \
    ErrorLine("  " << fen);            \
    for (U32 i = 0; i < pos; i++)      \
      Error(" ");                      \
    WarningLine("  ^");                \
    exit(1);                           \
  } while (0);

// 12*64 + 16 + 8 + 1; // pieces, castletypes, ep, side
#define HASH_PIECE(piece, sq) ((piece)*12+(sq))
#define HASH_CASTLE(flag) (64*12+(flag))
#define HASH_EP(sq) (64*12+16+File(sq))
#define HASH_SIDE() (64*12+16+8+1)

CState::CState()
{
  FromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CreateHash();
}

CState::CState(const std::string &fen)
{
  FromFen(fen);
  CreateHash();
}

std::vector<Move> CState::GetLegalMoves()
{
  std::vector<Move> moves;
  moves.reserve(256);

  return moves;
}

void CState::MakeMove(Move inMove)
{
  (void) inMove;
}

void CState::UndoMove(Move inMove)
{
  (void) inMove;
}

bool CState::IsLegalMove(Move inMove)
{
  (void) inMove;
  return true;
}

bool CState::IsMateScore(Score inScore)
{
  (void) inScore;
  return false;
}

State::TerminalType CState::IsTerminal()
{
  return NONE;
}


Score CState::GetScore(int inPly)
{
  (void) inPly;
  Score score = 0;
  return score;
}

Score CState::Quiescence(Score inAlpha, Score inBeta, int inPly)
{
  (void) inAlpha;
  (void) inBeta;
  (void) inPly;
  Score score = 0;
  return score;
}

void CState::CreateHash()
{
  mHashKey = HashType(0);
  U64 pieces;
  int square;

  // Hash the pieces (64*12 possibilities)
  for (int side = WHITE; side <= BLACK; side++)
  {
    for (int piece = PAWN; piece <= KING; piece++)
    {
      pieces = mPieces[side][piece];
      while (pieces)
      {
        square = bboard::PopLSB(pieces);
        mHashKey ^= sHashCodes[HASH_PIECE(piece, square)];
      }
    }
  }

  // Hash the castling types (16 possibilities)
  mHashKey ^= sHashCodes[HASH_CASTLE(mFlags)];

  // Hash the en passant types (8 possibilities)
  if (mEP > 0)
    mHashKey ^= sHashCodes[HASH_EP(mEP)];

  // Hash the side types (1 possibilities)
  if (mSide)
    mHashKey ^= sHashCodes[HASH_SIDE()];
}

std::string CState::ToString()
{
  std::stringstream ss;
  int r, f, sq, spaces = 5;
  ss << "\n";
  if (mSide == WHITE)
    ss << Verbose::Colorize("W", Verbose::WHITE, Verbose::BOLD) << " C:";
  else
    ss << Verbose::Colorize("B", Verbose::YELLOW, Verbose::BOLD) << " C:";

  if (mFlags & WHITE_KING_CASTLE)
  {
    ss << Verbose::Colorize("K", Verbose::WHITE);
    spaces--;
  }
  if (mFlags & WHITE_QUEEN_CASTLE)
  {
    ss << Verbose::Colorize("Q", Verbose::WHITE);
    spaces--;
  }
  if (mFlags & BLACK_KING_CASTLE)
  {
    ss << Verbose::Colorize("k", Verbose::YELLOW);
    spaces--;
  }
  if (mFlags & BLACK_QUEEN_CASTLE)
  {
    ss << Verbose::Colorize("q", Verbose::YELLOW);
    spaces--;
  }
  for (int i = 0; i < spaces; i++)
    ss << " ";
  ss << "EP:";
  if (mEP > 0)
    ss << char(mEP%8+'a') << int(mEP/8+1);
  else
    ss << "  ";
  ss << " M:" << mPly;
  ss << "\n\n";
  for (r = 56; r >= 0; r -= 8)
  {
    ss << (r / 8 + 1) << "  ";
    for (f = 0; f < 8; f++)
    {
      sq = r + f;
      if      (mPieces[WHITE][PAWN]   & Set(sq)) ss << Verbose::Colorize("P ", Verbose::WHITE);
      else if (mPieces[WHITE][KNIGHT] & Set(sq)) ss << Verbose::Colorize("N ", Verbose::WHITE);
      else if (mPieces[WHITE][BISHOP] & Set(sq)) ss << Verbose::Colorize("B ", Verbose::WHITE);
      else if (mPieces[WHITE][ROOK]   & Set(sq)) ss << Verbose::Colorize("R ", Verbose::WHITE);
      else if (mPieces[WHITE][QUEEN]  & Set(sq)) ss << Verbose::Colorize("Q ", Verbose::WHITE);
      else if (mPieces[WHITE][KING]   & Set(sq)) ss << Verbose::Colorize("K ", Verbose::WHITE);
      else if (mPieces[BLACK][PAWN]   & Set(sq)) ss << Verbose::Colorize("p ", Verbose::YELLOW);
      else if (mPieces[BLACK][KNIGHT] & Set(sq)) ss << Verbose::Colorize("n ", Verbose::YELLOW);
      else if (mPieces[BLACK][BISHOP] & Set(sq)) ss << Verbose::Colorize("b ", Verbose::YELLOW);
      else if (mPieces[BLACK][ROOK]   & Set(sq)) ss << Verbose::Colorize("r ", Verbose::YELLOW);
      else if (mPieces[BLACK][QUEEN]  & Set(sq)) ss << Verbose::Colorize("q ", Verbose::YELLOW);
      else if (mPieces[BLACK][KING]   & Set(sq)) ss << Verbose::Colorize("k ", Verbose::YELLOW);
      else if (BLACK_SQUARES & Set(sq))          ss << Verbose::Colorize(". ", Verbose::YELLOW);
      else                                       ss << Verbose::Colorize(". ", Verbose::WHITE);
    }
    ss << "\n";
  }
  ss << "\n   a b c d e f g h\n\n";
#ifndef NDEBUG
  ss << "0x" << std::hex << mHashKey << std::endl;
#endif
  return ss.str();
}

void CState::FromFen(const std::string &fen)
{
  memset(this, 0, sizeof(CState));

  U32 pos;
  int file, rank, sq;
  int c;

  pos = 0;
  c = fen[pos];

  // Determine piece placement
  for (rank = 7; rank >= 0; rank--)
  {
    for (file = 0; file < 8;)
    {
      if (c >= '1' && c <= '8')
        file += c - '0';
      else
      {
        sq = rank * 8 + file;
        switch (c)
        {
        case 'p': mPieces[BLACK][PAWN]   |= Set(sq); break;
        case 'n': mPieces[BLACK][KNIGHT] |= Set(sq); break;
        case 'b': mPieces[BLACK][BISHOP] |= Set(sq); break;
        case 'r': mPieces[BLACK][ROOK]   |= Set(sq); break;
        case 'q': mPieces[BLACK][QUEEN]  |= Set(sq); break;
        case 'k': mPieces[BLACK][KING]   |= Set(sq); break;
        case 'P': mPieces[WHITE][PAWN]   |= Set(sq); break;
        case 'N': mPieces[WHITE][KNIGHT] |= Set(sq); break;
        case 'B': mPieces[WHITE][BISHOP] |= Set(sq); break;
        case 'R': mPieces[WHITE][ROOK]   |= Set(sq); break;
        case 'Q': mPieces[WHITE][QUEEN]  |= Set(sq); break;
        case 'K': mPieces[WHITE][KING]   |= Set(sq); break;
        default: FenError(fen, pos);
        }
        file++;
      }
      c = fen[++pos];
    }
    if (rank > 0)
    {
      if (c != '/') FenError(fen, pos);
      c = fen[++pos];
    }
  }

  for (int s = WHITE; s <= BLACK; s++)
  {
    for (int p = PAWN; p <= KING; p++)
    {
      mOccupied |= mPieces[s][p];
      mFriends[s] |= mPieces[s][p];
    }
  }

  if (c != ' ') FenError(fen, pos);

  // Determine side to move
  c = fen[++pos];
  switch (c)
  {
  case 'w': mSide = WHITE; break;
  case 'b': mSide = BLACK; break;
  default: FenError(fen, pos);
  }

  c = fen[++pos];
  if (c != ' ') FenError(fen, pos);

  // Determine castling rights
  c = fen[++pos];
  if (c == '-')
    c = fen[++pos];
  else
  {
    if (c == 'K')
    {
      mFlags |= WHITE_KING_CASTLE;
      c = fen[++pos];
    }
    if (c == 'Q')
    {
      mFlags |= WHITE_QUEEN_CASTLE;
      c = fen[++pos];
    }
    if (c == 'k')
    {
      mFlags |= BLACK_KING_CASTLE;
      c = fen[++pos];
    }
    if (c == 'q')
    {
      mFlags |= BLACK_QUEEN_CASTLE;
      c = fen[++pos];
    }
  }

  if (c != ' ') FenError(fen, pos);

  // Determine en passant pos
  c = fen[++pos];
  if (c == '-')
    c = fen[++pos];
  else
  {
    if (c < 'a' || c > 'h') FenError(fen, pos);
    file = c - 'a';
    c = fen[++pos];
    if (c != (mSide ? '3' : '6')) FenError(fen, pos);
    rank = c - '1';
    c = fen[++pos];
    mEP = rank * 8 + file;
  }

  // Determine if we are done
  if (pos >= fen.size())
    return;

  if (c != ' ') FenError(fen, pos);
  c = fen[++pos];

  // Determine half move
  if (!isdigit(c)) FenError(fen, pos);
  mPly = atoi(&fen[pos]);
}

void CState::InitializeChessState()
{
  const U32 table_size = 128; // 128 megabyte
  const U32 hash_codes = 12*64 + 16 + 8 + 1; // pieces, castletypes, ep, side
  CState::Initialize(table_size, hash_codes);
  magics::Initialize();
}
