#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
	printf("\nBegin\n");
	
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr*));
	wordList->first = malloc(sizeof(wordStr*));
	wordList->last = malloc(sizeof(wordStr*));
	PerformLex(wordList);

	while(wordList->first->next != NULL)
	printf("testing: %s\n", wordList->first->content);

	printf("\nEnd\n");
	return 0;
}