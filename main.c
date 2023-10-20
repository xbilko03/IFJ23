#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
	printf("\nBegin\n-----------\n");
	
	//Create Word List
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr));
	wordList->first = NULL;
	wordList->last = NULL;
	
	//Fill Word List and Check for Lex Rules
	PerformLex(wordList);

	PerformSyntax(wordList);

	PerformSemantics();

	PerformCodeGen();

	DeleteWordList(wordList);

	printf("\n-----------\nEnd\n");
	return 0;
}
