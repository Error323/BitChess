#include "cstatetest.h"
#include "../cstate.h"

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
  std::cout << std::endl;
  static std::vector<std::string> algebraic_notation;
  if (algebraic_notation.empty())
  {
    for (char file = '1'; file <= '8'; file++)
    {
      for (char rank = 'a'; rank <= 'h'; rank++)
      {
        std::string s = "";
        s += rank;
        s += file;
        algebraic_notation.push_back(s);
      }
    }
  }
  CState::InitializeChessState();
  U64 black = C64(0x85E0010844200000);
  U64 white = C64(0x0000000029C40630);
  U64 pieces = black | white;
  int square = CState::C3;
  std::cout << "Black" << std::endl;
  Print(black);
  std::cout << "White" << std::endl;
  Print(white);
  std::cout << "Pieces" << std::endl;
  Print(pieces);
  std::cout << "Rook   " << algebraic_notation[square] << std::endl;
  U64 occ = pieces & CState::sRMask[square];
  int index = Transform(occ, r_magic[square], rook_bits[square]);
  U64 moves = CState::sRMoves[square][index];
  Print(moves & ~white);
  std::cout << "Bishop " << algebraic_notation[square] << std::endl;
  occ = pieces & CState::sBMask[square];
  index = Transform(occ, b_magic[square], bishop_bits[square]);
  moves = CState::sBMoves[square][index];
  Print(moves & ~white);
}

void CStateTest::setUp()
{
}

void CStateTest::tearDown()
{
}
