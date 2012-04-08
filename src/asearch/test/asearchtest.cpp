#include "asearchtest.h"
#include "../asearch.h"

extern uint64_t Rand64();

#include <iostream>
#include <sstream>

using namespace asearch;

CPPUNIT_TEST_SUITE_REGISTRATION(ASearchTest);

ASearchTest::ASearchTest()
  : CppUnit::TestFixture()
{
}

ASearchTest::~ASearchTest()
{
}

const static unsigned short win_positions[8] =
{
  0x1C0, // row3
  0x38,  // row2
  0x7,   // row1
  0x124, // col3
  0x92,  // col2
  0x49,  // col1
  0x111, // diagonal 1
  0x54   // diagonal 2
};

const static unsigned short moves[9] =
{
  1<<0, 1<<1, 1<<2,
  1<<3, 1<<4, 1<<5,
  1<<6, 1<<7, 1<<8
};

void ASearchTest::test_create()
{
  std::cout << std::endl;
  int hash_table_size = 1024*64;
  int hash_codes = 18; // 9 for the board * 2 for the pieces
  TTTState::Initialize(hash_table_size, hash_codes);

  for (int i = 0; i < 5; i++)
  {
    TTTState::Reset();
    std::cout << "-----------GAME " << i << "----------" << std::endl;
    TTTState game_state;
    int turn = static_cast<int>(Rand64() % 7);
    for (int t = 0; t < turn; t++)
    {
      Move move = static_cast<Uint8>(Rand64() % 9);
      while (!game_state.IsLegalMove(move))
        move = static_cast<Uint8>(Rand64() % 9);

      game_state.MakeMove(move);
      if (game_state.IsTerminal())
      {
        game_state.UndoMove(move);
        t--;
      }
    }
    std::cout << "Turn: " << turn << std::endl;
    std::cout << game_state.ToString() << std::endl << std::endl;

    while (!game_state.IsTerminal())
    {
      Pair A, B, C;
      int max_depth = 9 - turn;
      if (turn % 2 == 0)
      {
        A = mAI->Minimax(&game_state, max_depth);
        B = mAI->AlphaBeta(&game_state, max_depth);
        C = mAI->Negascout(&game_state, max_depth);

        CPPUNIT_ASSERT_EQUAL(A.first, B.first);
        CPPUNIT_ASSERT_EQUAL(int(A.second), int(B.second));
        CPPUNIT_ASSERT_EQUAL(A.first, C.first);
        CPPUNIT_ASSERT_EQUAL(A.second, C.second);
      }
      else
      {
        // NOTE: We cannot use AlphaBeta here for depths < max_depth as it will
        //       find superior values from AlphaBeta player 1's ttable
        A = mAI->Minimax(&game_state, 2);
        C = mAI->Negascout(&game_state, 2);

        CPPUNIT_ASSERT_EQUAL(A.first, C.first);
        CPPUNIT_ASSERT_EQUAL(int(A.second), int(C.second));
      }
      game_state.MakeMove(A.second);

      turn++;
      std::cout << "Turn: " << turn << std::endl;
      std::cout << game_state.ToString() << std::endl << std::endl;
    }
  }
}

void ASearchTest::setUp()
{
  mAI = new ASearch();
}

void ASearchTest::tearDown()
{
  delete mAI;
}

TTTState::TTTState()
{
  mBoard[CROSS] = 0x0;
  mBoard[CIRCLE] = 0x0;
  mSide = CROSS;
  CreateHash();
}

bool TTTState::IsLegalMove(Move inMove)
{
  if (inMove < 0 || inMove > 8)
    return false;

  unsigned short empty = ~(mBoard[CROSS] | mBoard[CIRCLE]);
  if ((empty & moves[inMove]) == moves[inMove])
    return true;

  return false;
}

bool TTTState::IsMateScore(int inScore)
{
  return (inScore > INF-255) || (inScore < -INF+255);
}

void TTTState::CreateHash()
{
  mHashKey = (HashType) 0;
  for (int i = 0; i < 9; i++)
  {
    if ((mBoard[CROSS] & moves[i]) == moves[i])
    {
      mHashKey ^= sHashCodes[9*CROSS + i];
    }
    else if ((mBoard[CIRCLE] & moves[i]) == moves[i])
    {
      mHashKey ^= sHashCodes[9*CIRCLE + i];
    }
  }
}

bool TTTState::IsTerminal()
{
  if ((mBoard[CROSS] | mBoard[CIRCLE]) == 0x1FF)
    return true;

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 8; j++)
      if ((mBoard[i]&win_positions[j]) == win_positions[j])
        return true;

  return false;
}

void TTTState::MakeMove(Move inMove)
{
  mHashKey ^= sHashCodes[9*mSide + inMove];
  mBoard[mSide] |= moves[inMove];
  mSide = Type(1^mSide);
}

void TTTState::UndoMove(Move inMove)
{
  mSide = Type(1^mSide);
  mHashKey ^= sHashCodes[9*mSide + inMove];
  mBoard[mSide] &= ~(moves[inMove]);
}

std::vector<Move> TTTState::GetLegalMoves()
{
  std::vector<Move> moves;
  for (Move i = 0; i < 9; i++)
    if (IsLegalMove(i))
      moves.push_back(i);
  return moves;
}


int BitCount(unsigned short inVal)
{
  int count = 0;
  while (inVal)
  {
    count++;
    inVal &= inVal - 1;
  }
  return count;
}
int TTTState::GetScore()
{
  static const unsigned short CENTER = 0x10;
  static const unsigned short CORNERS = 0x145;
  static const unsigned short REST = ~(CENTER|CORNERS);

  Type opp = Type(1^mSide);

  for (int j = 0; j < 8; j++)
  {
    if ((mBoard[mSide]&win_positions[j]) == win_positions[j])
      return 1000;
    if ((mBoard[opp]&win_positions[j]) == win_positions[j])
      return -1000;
  }

  if ((mBoard[CROSS] | mBoard[CIRCLE]) == 0x1FF)
    return 0;

  return (BitCount(mBoard[mSide]&CENTER)  * 40 +
          BitCount(mBoard[mSide]&CORNERS) * 30 +
          BitCount(mBoard[mSide]&REST)    * 20)
      -
         (BitCount(mBoard[opp]&CENTER)    * 40 +
          BitCount(mBoard[opp]&CORNERS)   * 30 +
          BitCount(mBoard[opp]&REST)      * 20);
}

int TTTState::Quiescence(int inAlpha, int inBeta)
{
  (void) inAlpha;
  (void) inBeta;
  return GetScore();
}

std::string TTTState::ToString()
{
  std::stringstream s;

  for (int i = 0; i < 3; i++)
  {
    unsigned char circles = mBoard[CIRCLE] >> ((i*3) & 0xf);
    unsigned char crosses = mBoard[CROSS] >> ((i*3) & 0xf);
    for (int j = 0; j < 3; j++)
    {
      if (((circles >> j) & 1) == 1)
        s << " o";
      else if (((crosses >> j) & 1) == 1)
        s << " x";
      else
        s << " .";// << i*3+j;
    }
    s << "  ";
    for (int j = 0; j < 3; j++)
      s << " " << i*3+j;
    s << "\n";
  }

  return s.str();
}
