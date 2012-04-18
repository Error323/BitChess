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
  int table_size = 8000*2;
  int hash_codes = 2*7 + 1 + 1 + 2 + 1;
  CState::Initialize(table_size, hash_codes);
  CState::InitializeMoveTables();

  CState game_state;
  std::cout << game_state.ToString();
}

void CStateTest::setUp()
{

}

void CStateTest::tearDown()
{

}
