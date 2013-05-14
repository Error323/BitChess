#include "perftest.h"
#include "../cmove.h"
#include "../magics.h"

#include <iostream>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(PerfTest);

PerfTest::PerfTest()
  : CppUnit::TestFixture()
{
}

PerfTest::~PerfTest()
{
}

void PerfTest::start()
{
  CState s;
  perft(1, s);
  perft(2, s);
  perft(3, s);
  perft(4, s);
  perft(5, s);
  perft(6, s);
}

U64 PerfTest::perft(int depth, CState &state)
{
  if (depth == 0)
    return 1;

  std::vector<Move> moves = state.GetLegalMoves();
  U64 nodes = 0;
  int n, i;

  for (i = 0, n = moves.size(); i < n; i++)
  {
    state.MakeMove(moves[i]);
    nodes += perft(depth - 1, state);
    state.UndoMove(moves[i]);
  }

  return nodes;
}

void PerfTest::setUp()
{
  CState::InitializeChessState();
}

void PerfTest::tearDown()
{
}
