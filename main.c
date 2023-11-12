#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include "lex.h"

int main(int argc, char **argv)
{
	printf("\nBegin\n");
	
	//Create Word List
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr));
	wordList->first = malloc(sizeof(wordStr));
	wordList->last = wordList->first;
	wordList->first->type = NULL;
	wordStr* LastToken = wordList->first;
	
	//Fill Word List and Check for Lex Rules
	Tokenizer(LastToken);

	PrintWordList(wordList->first);

	printf("--------------------------------\n\n");

	PerformSyntax(wordList);

	PerformSemantics();

	PerformCodeGen();

	//DeleteWordList(wordList);

	printf("\nEnd\n");
	return 0;
}
