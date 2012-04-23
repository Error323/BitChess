#include "version.h"

#include "utils/verbose.h"
#include "utils/debugger.h"

int main(int argc, char **argv)
{
  (void) argc;
  (void) argv;
  NoticeLine(HUMAN_NAME);
  return 0;
}
