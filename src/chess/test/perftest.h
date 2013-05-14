#ifndef PERF_TEST_H
#define PERF_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../../bitboard/bitboard.h"
#include "../cstate.h"

class PerfTest : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(PerfTest);
  CPPUNIT_TEST(start);
  CPPUNIT_TEST_SUITE_END();

public:
  PerfTest();
  ~PerfTest();

  void setUp();
  void tearDown();

  // Test Methods
  void start();

private:
  U64 perft(int depth, CState &state);
};

#endif // PERF_TEST_H
