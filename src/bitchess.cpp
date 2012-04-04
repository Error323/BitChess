#include <iostream>

#include "version.h"

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	std::cout << HUMAN_NAME << std::endl;
}
