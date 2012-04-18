#ifndef BBOARD_TEST_H
#define BBOARD_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class BBoardTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(BBoardTest);
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST_SUITE_END();

public:
  BBoardTest();
  ~BBoardTest();

  void setUp();
  void tearDown();

  // Test Methods
  void test_create();
};

#endif // BBOARD_TEST_H
