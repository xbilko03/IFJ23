#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
	printf("\nBegin\n");
	
	//Create Word List
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr*));
	wordList->first = malloc(sizeof(wordStr*));
	wordList->last = malloc(sizeof(wordStr*));
	
	//Fill Word List and Check for Lex Rules
	PerformLex(wordList);

	PerformSyntax(wordList);

	PerformSemantics();

	PerformCodeGen();

	DeleteWordList(wordList);

	printf("\nEnd\n");
	return 0;
}