#include "timer.h"
#include "verbose.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>

std::vector<String>      Timer::sTasks;
std::map<String, Uint32> Timer::sTaskCounters;
std::map<String, double> Timer::sSumTimings;
std::map<String, double> Timer::sMinTimings;
std::map<String, double> Timer::sMaxTimings;

Timer::Timer(rcString inName):
  mIsInitialized(true),
  mTaskName(inName)
{
  if (std::find(sTasks.begin(), sTasks.end(), inName) == sTasks.end())
  {
    sTasks.push_back(inName);
    sTaskCounters[inName] = 0;
    sSumTimings[inName]   = 0.0;
    sMaxTimings[inName]   = 0.0;
    sMinTimings[inName]   = std::numeric_limits<double>::max();
  }
  mStartTime = clock();
}

Timer::~Timer() {
  mEndTime = clock();

  if (!mIsInitialized)
    return;

  sTaskCounters[mTaskName]++;
  double diff_time = (mEndTime - mStartTime) / (double)CLOCKS_PER_SEC;
  sSumTimings[mTaskName] += diff_time;
  sMinTimings[mTaskName] = std::min<double>(sMinTimings[mTaskName], diff_time);
  sMaxTimings[mTaskName] = std::max<double>(sMaxTimings[mTaskName], diff_time);
}

String Timer::GetReport(int inPrecision)
{
#ifndef ENABLE_PROFILING
  (void) inPrecision;
  return "";
#else
  std::string report;
  report += SpacePadding("Name",  NAME_SPACING);
  report += SpacePadding("Min",   CELL_SPACING);
  report += SpacePadding("Max",   CELL_SPACING);
  report += SpacePadding("Avg",   CELL_SPACING);
  report += SpacePadding("Sum",   CELL_SPACING);
  report += SpacePadding("Calls", CELL_SPACING);

  report = Verbose::Colorize(report, Verbose::WHITE, Verbose::BOLD) + "\n\n";

  for (int i = 0, n = sTasks.size(); i < n; i++)
  {
    std::stringstream line;
    String padded_line = SpacePadding(sTasks[i], NAME_SPACING);
    line << std::setiosflags(std::ios::fixed)
         << std::setprecision(inPrecision)
         << sMinTimings[sTasks[i]];
    padded_line += SpacePadding(line.str(), CELL_SPACING);
    line.str("");

    line << std::setiosflags(std::ios::fixed)
         << std::setprecision(inPrecision)
         << sMaxTimings[sTasks[i]];
    padded_line += SpacePadding(line.str(), CELL_SPACING);
    line.str("");

    line << std::setiosflags(std::ios::fixed)
         << std::setprecision(inPrecision)
         << (sSumTimings[sTasks[i]] / sTaskCounters[sTasks[i]]);
    padded_line += SpacePadding(line.str(), CELL_SPACING);
    line.str("");

    line << std::setiosflags(std::ios::fixed)
         << std::setprecision(inPrecision)
         << sSumTimings[sTasks[i]];
    padded_line += SpacePadding(line.str(), CELL_SPACING);
    line.str("");

    line << sTaskCounters[sTasks[i]];
    padded_line += SpacePadding(line.str(), CELL_SPACING);
    line.str("");

    Verbose::Color color = (i % 2) == 0 ? Verbose::WHITE : Verbose::CYAN;
    report += Verbose::Colorize(padded_line, color) + "\n";
  }
  report += Verbose::Colorize("\nTimings are in seconds...\n", Verbose::YELLOW);
  return report;
#endif
}

String Timer::SpacePadding(rcString inString, int inSpaces)
{
  String padded = inString;
  int n = inSpaces - inString.size();
  if (n-1 < 0)
    return padded.substr(0, padded.size()+n-4) + "... ";

  for (int i = 0; i < n; i++)
    padded += " ";

  return padded;
}
