#include "cmovetest.h"
#include "../cmove.h"
#include "../cstate.h"
#include "../../utils/random.c"

#include <iostream>

extern uint32_t Rand32();

CPPUNIT_TEST_SUITE_REGISTRATION(CMoveTest);

CMoveTest::CMoveTest()
  : CppUnit::TestFixture()
{
}

CMoveTest::~CMoveTest()
{
}

void CMoveTest::create()
{
  for (int i = 0; i < 100; i++)
  {
    U32 to = Rand32() % 64;
    U32 from = Rand32() % 64;

    CMove m(from, to, CMove::QUIET_MOVE);

    CPPUNIT_ASSERT_EQUAL(from, m.GetFrom());
    CPPUNIT_ASSERT_EQUAL(to, m.GetTo());
  }

  U32 state_pieces[6] = {CState::PAWN, CState::KNIGHT, CState::BISHOP, CState::ROOK, CState::QUEEN, CState::KING};
  U32 from_pieces[6] = {CMove::FROM_PAWN, CMove::FROM_KNIGHT, CMove::FROM_BISHOP, CMove::FROM_ROOK, CMove::FROM_QUEEN, CMove::FROM_KING};
  U32 to_pieces[6] = {CMove::TO_PAWN, CMove::TO_KNIGHT, CMove::TO_BISHOP, CMove::TO_ROOK, CMove::TO_QUEEN, CMove::TO_KING};
  for (int i = 0; i < 100; i++)
  {
    U32 to = Rand32() % 64;
    U32 from = Rand32() % 64;

    int i_f = Rand32() % 6;
    int i_t = Rand32() % 6;
    U32 from_piece = from_pieces[i_f];
    U32 to_piece = to_pieces[i_t];

    CMove m(from, to, CMove::CAPTURES|from_piece|to_piece);

    CPPUNIT_ASSERT_EQUAL(state_pieces[i_f], m.GetFromPiece());
    CPPUNIT_ASSERT_EQUAL(state_pieces[i_t], m.GetToPiece());
  }
}

void CMoveTest::setUp()
{
}

void CMoveTest::tearDown()
{
}
