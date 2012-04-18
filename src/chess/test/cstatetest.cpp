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
  CState::InitializeChessState();

  CState game_state;
  std::cout << game_state.ToString();
}

void CStateTest::setUp()
{

}

void CStateTest::tearDown()
{

}
