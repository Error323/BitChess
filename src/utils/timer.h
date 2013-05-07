#ifndef TIMER_H
#define TIMER_H

#include <ctime>

namespace timer {

inline double GetRealTime()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return double(ts.tv_sec) + 1e-9 * double(ts.tv_nsec);
}

inline double GetCpuTime()
{
  timespec ts;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  return double(ts.tv_sec) + 1e-9 * double(ts.tv_nsec);
}

} // namespace timer

#endif
