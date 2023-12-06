/**
 * @name IFJ2023 Compiler
 * @file error.c
 * @author Jozef Bilko - xbilko03
 */

#include "error.h"

void ExitProgram(int returnCode, char* message)
{
	fprintf(stderr, "%s", message);
	exit(returnCode);
}