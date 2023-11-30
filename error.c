#include "error.h"

void ExitProgram(int returnCode, char* message)
{
	fprintf(stderr, "%s", message);
	exit(returnCode);
}