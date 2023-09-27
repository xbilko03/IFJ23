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

	PerformSyntax();

	PerformSemantics();

	PerformCodeGen();

	DeleteWordList(wordList);
	
	printf("%ld\n", HashFunction("buchac"));
	printf("%ld\n", HashFunction("gg"));
	printf("%ld\n", HashFunction("esim"));
	printf("%ld\n", HashFunction("76"));
	printf("%ld\n", HashFunction("76"));
	printf("%ld\n", HashFunction("76"));
	printf("%ld\n", HashFunction("esim"));
	printf("%ld\n", HashFunction("gg"));
	printf("%ld\n", HashFunction("gg"));

	printf("\nEnd\n");
	return 0;
}