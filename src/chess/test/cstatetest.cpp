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
  CState::InitializeChessState();
}

void CStateTest::setUp()
{
}

void CStateTest::tearDown()
{
}
