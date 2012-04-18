#ifndef CSTATE_TEST_H
#define CSTATE_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class CStateTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(CStateTest);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST_SUITE_END();

public:
  CStateTest();
  ~CStateTest();

  void setUp();
  void tearDown();

  // Test Methods
  void test_create();
};

#endif // CSTATE_TEST_H
