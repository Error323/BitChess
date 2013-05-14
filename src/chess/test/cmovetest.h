#ifndef CMOVE_TEST_H
#define CMOVE_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class CMoveTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(CMoveTest);
  CPPUNIT_TEST(create);
  CPPUNIT_TEST_SUITE_END();

public:
  CMoveTest();
  ~CMoveTest();

  void setUp();
  void tearDown();

  // Test Methods
  void create();
};

#endif // CMOVE_TEST_H
