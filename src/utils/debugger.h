#ifndef DEBUGGER_HDR
#define DEBUGGER_HDR

#ifndef NDEBUG
#include <cstdio>
#include <cstdlib>

#include <execinfo.h>

#define BREAKPOINT asm("int $3")
#define END() BREAKPOINT
#define FORMAT_STRING "***ASSERTION FAILED***\n\n\tfile\t%s\n\tline\t%d\n\tfunc\t%s\n\tcond\t%s\n"
#define FORMAT_STRING_MSG FORMAT_STRING"\ttext\t"

#define BACKTRACE()                                         \
  do {                                                      \
    static const char* filename = "callstack.txt";          \
    FILE* fp = fopen(filename, "w");                        \
    void* addresses[16];                                    \
    size_t size = backtrace(addresses, 16);                 \
    for (size_t i = 0; i < size-2; i++)                     \
      fprintf(fp, "%p\n", addresses[i]);                    \
    fclose(fp);                                             \
    fprintf(stderr, "addr2line -e <prog> @%s\n", filename); \
  } while (0)

#define ASSERT(cond)                                                        \
  do {                                                                      \
    if ( !(cond) ) {                                                        \
      FATAL(FORMAT_STRING, __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond); \
      BACKTRACE();                                                          \
      END();                                                                \
    }                                                                       \
  } while (0)

#define ASSERT_MSG(cond, ...)                                                   \
  do {                                                                          \
    if ( !(cond) ) {                                                            \
      FATAL(FORMAT_STRING_MSG, __FILE__, __LINE__, __PRETTY_FUNCTION__, #cond); \
      FATAL(__VA_ARGS__);                                                       \
      FATAL("\n");                                                              \
      BACKTRACE();                                                              \
      END();                                                                    \
    }                                                                           \
  } while (0)

#define FATAL(...)                \
  do {                            \
    fprintf(stderr, __VA_ARGS__); \
  } while (0)

#else // NDEBUG
#define ASSERT(cond)
#define ASSERT_MSG(cond, ...)
#endif // NDEBUG

#endif // DEBUGGER_HDR
