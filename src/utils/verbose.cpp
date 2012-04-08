#include "verbose.h"

#include <cstdlib>
#include <cstring>

std::map<Verbose::Level, String> Verbose::sLevels;
std::ofstream                    Verbose::sStream;
Verbose*                         Verbose::sInstance = NULL;
bool                             Verbose::sShouldUseColor = false;

#ifdef DEBUG
Uint32 Verbose::sMinLevel = Verbose::DBG;
#else
Uint32 Verbose::sMinLevel = Verbose::NTC;
#endif

#ifndef VERBOSE_LOG_FILE
Uint32 Verbose::sOutput = SCREEN | LOG;
#else
Uint32 Verbose::sOutput = SCREEN;
#endif

Verbose::Verbose():
  mIsStartOfLine(true)
{
  sShouldUseColor = ShouldUseColor();
}

Verbose::~Verbose()
{
  if (sOutput & LOG)
    sStream.close();
}

Verbose* Verbose::Instance()
{
  if (sInstance == NULL)
  {
    if (sOutput & LOG)
      sStream.open("log.txt", std::ios::out);

    sLevels[DBG] = "DEBUG";
    sLevels[NTC] = "NOTICE";
    sLevels[WRN] = "WARNING";
    sLevels[ERR] = "ERROR";
    sLevels[FTL] = "FATAL";
    sInstance = new Verbose();
  }
  return sInstance;
}

Verbose& Verbose::Print(rcString inMsg, Level inLevel)
{
  if (inLevel < sMinLevel)
    return *this;
  

  if (sOutput & LOG && sStream.is_open() && sStream.good())
  {
    if (mIsStartOfLine)
      sStream << Prefix(inLevel) << inMsg;
    else
      sStream << inMsg;
    sStream.flush();
  }

  if (sOutput & SCREEN)
  {
    std::string output;
    if (sShouldUseColor)
      output = ColorizeLevel(inMsg, inLevel);
    else if (mIsStartOfLine)
      output = Prefix(inLevel) + inMsg;
    else
      output = inMsg;

    switch (inLevel)
    {
      case ERR: {
        std::cerr << output << std::flush;
        break;
      }
      case FTL: {
        std::cerr << output << std::flush;
        exit(EXIT_FAILURE);
        break;
      }
      default: {
        std::cout << output << std::flush;
        break;
      }
    }
  }
  mIsStartOfLine = (inMsg.find('\n') != std::string::npos);
  return *this;
}

String Verbose::Prefix(Level inLevel)
{
  return "[" + sLevels[inLevel] + "] ";
}

String Verbose::ColorizeLevel(rcString inMsg, Verbose::Level inLevel)
{
  std::stringstream s;
  s << "\033[";
  switch (inLevel)
  {
    case DBG: s << REGULAR << ";" << GREEN;  break;
    case NTC: s << REGULAR << ";" << WHITE;  break;
    case WRN: s << REGULAR << ";" << YELLOW; break;
    case ERR: s << REGULAR << ";" << RED;    break;
    case FTL: s << REGULAR << ";" << RED;    break;
    default:  s << REGULAR << ";" << CYAN;   break;
  }
  s << "m" << inMsg << "\033[0m";

  return s.str();
}

String Verbose::Colorize(rcString inMsg, Color inColor, Style inStyle)
{
  if (!ShouldUseColor())
    return inMsg;

  std::stringstream s;
  s << "\033["
    << inStyle
    << ";"
    << inColor
    << "m"
    << inMsg
    << "\033[0m";

  return s.str();
}

bool Verbose::ShouldUseColor() {
#if WIN32
  // On Windows the TERM variable is usually not set, but the
  // console there does support colors.
  return true;
#else
  // On non-Windows platforms, we rely on the TERM variable.
  const char* term = getenv("TERM");

  if (term == NULL)
    return false;

  const bool term_supports_color =
    strcmp(term, "xterm") >= 0 ||
    strcmp(term, "xterm-color") >= 0 ||
    strcmp(term, "xterm-256color") >= 0 ||
    strcmp(term, "screen") >= 0 ||
    strcmp(term, "linux") >= 0 ||
    strcmp(term, "cygwin") >= 0;

  return term_supports_color;
#endif  // WIN32
}
