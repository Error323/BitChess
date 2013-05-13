#include "cstatetest.h"
#include "../cstate.h"
#include "../magics.h"
#include "../../bitboard/bitboard.h"

#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(CStateTest);

CStateTest::CStateTest()
  : CppUnit::TestFixture()
{
}

CStateTest::~CStateTest()
{
}

void CStateTest::test_create()
{
  CState::InitializeChessState();
  CState state1;
  std::cout << state1.ToString() << std::endl;
  CState state2("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  std::cout << state2.ToString() << std::endl;
}

void CStateTest::test_magics()
{
  // Bishops
  for (int i = 0; i < 64; i++)
  {
    U64 m = magics::bishop_magics[i];
    U64 mask = magics::BishopMask(i);
    int used_bits = 64 - (m >> 58);
    int max_bits = bboard::CountBits(mask);
    int size = 1 << used_bits;
    std::vector<U64> used(size, C64(0));

    for (int j = 0; j < (1<<max_bits); j++)
    {
      U64 block = magics::Index2U64(j, max_bits, mask);
      U64 attack = magics::BishopAttack(i, block);
      CPPUNIT_ASSERT(attack == magics::BishopAttacks(block, i));
      int index = magics::Transform(block, m);
      if (used[index] == C64(0))
        used[index] = attack;
      CPPUNIT_ASSERT(used[index] == attack);
    }
  }

  // Rooks
  for (int i = 0; i < 64; i++)
  {
    U64 m = magics::rook_magics[i];
    U64 mask = magics::RookMask(i);
    int used_bits = 64 - (m >> 58);
    int max_bits = bboard::CountBits(mask);
    int size = 1 << used_bits;
    std::vector<U64> used(size, C64(0));

    for (int j = 0; j < (1<<max_bits); j++)
    {
      U64 block = magics::Index2U64(j, max_bits, mask);
      U64 attack = magics::RookAttack(i, block);
      CPPUNIT_ASSERT(attack == magics::RookAttacks(block, i));
      int index = magics::Transform(block, m);
      if (used[index] == C64(0))
        used[index] = attack;
      CPPUNIT_ASSERT(used[index] == attack);
    }
  }

}

void CStateTest::setUp()
{
  magics::Initialize();
}

void CStateTest::tearDown()
{
}
