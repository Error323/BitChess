#include "asearchtest.h"
#include "../asearch.h"

#include <iostream>
#include <sstream>

namespace asearch
{

CPPUNIT_TEST_SUITE_REGISTRATION(ASearchTest);

ASearchTest::ASearchTest()
  : CppUnit::TestFixture()
{
}

ASearchTest::~ASearchTest()
{
}


const static unsigned short win_positions[8] = {
  0x1C0, // row3
  0x38,  // row2
  0x7,   // row1
  0x124, // col3
  0x92,  // col2
  0x49,  // col1
  0x111, // diagonal 1
  0x54   // diagonal 2
};

const static int moves[9] = {
  0, 1, 2,
  3, 4, 5,
  6, 7, 8
};

void ASearchTest::test_create()
{
  std::cout << std::endl;
  TTTState game_state;
  int turn = 0;

  while (!game_state.IsTerminal())
  {
    Pair A, B;
    int max_depth = 9 - turn;
    if (turn % 2 == 0)
    {
      A = mAI->Minimax(&game_state, max_depth);
      B = mAI->AlphaBeta(&game_state, max_depth);
      CPPUNIT_ASSERT_EQUAL(A.first, B.first);
      CPPUNIT_ASSERT_EQUAL(A.second, B.second);
    }
    else
    {
      A = mAI->Minimax(&game_state, 2);
      B = mAI->AlphaBeta(&game_state, 2);
      CPPUNIT_ASSERT_EQUAL(A.first, B.first);
      CPPUNIT_ASSERT_EQUAL(A.second, B.second);
    }
    game_state.MakeMove(A.second);
    std::cout << "Turn: " << turn << std::endl;
    std::cout << game_state.ToString() << std::endl << std::endl;
    turn++;
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
}

bool TTTState::IsLegalMove(Move inMove)
{
  if (inMove < 0 || inMove > 8)
    return false;

  unsigned short empty = ~(mBoard[CROSS] | mBoard[CIRCLE]);
  if (((empty >> inMove) & 1) == 1)
    return true;

  return false;
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
  mBoard[mSide] |= static_cast<unsigned short>(1 << moves[inMove]);
  mSide = Type(1^mSide);
}

void TTTState::UndoMove(Move inMove)
{
  mSide = Type(1^mSide);
  mBoard[mSide] &= static_cast<unsigned short>(~(1 << moves[inMove]));
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

  if ((mBoard[CROSS] | mBoard[CIRCLE]) == 0x1FF)
    return 0;

  Type opp = Type(1^mSide);

  for (int j = 0; j < 8; j++)
  {
    if ((mBoard[mSide]&win_positions[j]) == win_positions[j])
      return 1000;
    if ((mBoard[opp]&win_positions[j]) == win_positions[j])
      return -1000;
  }

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
    s << "\n";
  }

  return s.str();
}

} // namespace asearch
