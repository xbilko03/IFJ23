#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void ExitProgram(int returnCode, char* message)
{
	fprintf(stderr, "%s", message);
	exit(returnCode);
}