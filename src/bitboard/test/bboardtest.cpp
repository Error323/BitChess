#include "bboardtest.h"
#include "../bitboard.h"

#include <iostream>

using namespace bboard;

CPPUNIT_TEST_SUITE_REGISTRATION(BBoardTest);

BBoardTest::BBoardTest()
  : CppUnit::TestFixture()
{

}

BBoardTest::~BBoardTest()
{

}

void BBoardTest::test_create()
{
  std::cout << std::endl;
  U64 black = C64(0xAA55AA55AA55AA55);
  U64 white = ~black;
  Print(white);
  CPPUNIT_ASSERT_EQUAL(Uint8(1), LSBIndex(white));

  U64 lsb[64];
  for (Uint8 i = 0; i < 64; i++)
  {
    lsb[i] = C64(0);
    lsb[i] |= (C64(1) << i);
    CPPUNIT_ASSERT_EQUAL(i, LSBIndex(lsb[i]));
  }

  CPPUNIT_ASSERT_EQUAL(32, CountBits(black));
}

void BBoardTest::setUp()
{

}

void BBoardTest::tearDown()
{

}
