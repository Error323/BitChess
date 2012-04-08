#ifndef ASEARCH_TEST_H
#define ASEARCH_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../state.h"

namespace asearch
{

class ASearch;

// tictactoe state
class TTTState : public State
{
public:
  enum Type {
    CROSS  = 0,
    CIRCLE = 1
  };

  TTTState();
  ~TTTState() {}

  std::vector<Move> GetLegalMoves();
  void MakeMove(Move inMove);
  void UndoMove(Move inMove);
  bool IsTerminal();
  bool IsLegalMove(Move inMove);
  Value GetScore();
  Value Quiescence(int inAlpha, int inBeta);

  std::string ToString();

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
