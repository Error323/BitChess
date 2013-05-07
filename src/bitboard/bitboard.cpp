#include "bitboard.h"

#include <cstdio>
#include <sstream>

namespace bboard
{

void Print3(const U64 a, const U64 b, const U64 c)
{
  unsigned char line;
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", i+1);

    line = (a >> (i * 8)) & 0xff;
    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\t%d", i+1);

    line = (b >> (i * 8)) & 0xff;
    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\t%d", i+1);

    line = (c >> (i * 8)) & 0xff;
    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\n");
  }

  printf("  a b c d e f g h \t  a b c d e f g h \t  a b c d e f g h\n");
}

void Print2(const U64 a, const U64 b)
{
  unsigned char line;
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", i+1);

    line = (a >> (i * 8)) & 0xff;
    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\t%d", i+1);

    line = (b >> (i * 8)) & 0xff;
    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\n");
  }

  printf("  a b c d e f g h \t  a b c d e f g h \n");
}

void Print1(const U64 board)
{
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", i+1);
    unsigned char line = (board >> (i * 8)) & 0xff;

    for (int j = 0; j < 8; j++)
      printf(" %s", ((line >> j) & 1) == 1 ? "1" : ".");

    printf("\n");
  }

  printf("  a b c d e f g h\n");
}

}
