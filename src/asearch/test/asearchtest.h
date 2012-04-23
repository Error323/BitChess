#ifndef ASEARCH_TEST_H
#define ASEARCH_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../state.h"
#include <vector>
#include <string>

namespace asearch
{

class ASearch;

// tictactoe state
class TTTState : public State
{
public:
  enum Type
  {
    CROSS  = 0,
    CIRCLE = 1
  };

  TTTState();
  ~TTTState() {}

  std::vector<Move> GetLegalMoves();
  void MakeMove(Move inMove);
  void UndoMove(Move inMove);
  void CreateHash();
  bool IsMateScore(Score inScore);
  TerminalType IsTerminal();
  Score GetScore(int inPly);
  Score Quiescence(Score inAlpha, Score inBeta, int inPly);

  bool IsLegalMove(Move inMove);
  std::string ToString();

private:
  unsigned short mBoard[2];
  Type mSide;
};

class ASearchTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(ASearchTest);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST_SUITE_END();

public:
  ASearchTest();
  ~ASearchTest();

  void setUp();
  void tearDown();

  // Test Methods
  void test_create();

private:
  ASearch *mAI;
};

} // namespace asearch

#endif // ASEARCH_TEST_H
