#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

void DeleteWordList(wordListStr* wordList)
{
	wordStr* currentWord = wordList->first;
	wordStr* nextWord;

	while(currentWord != NULL)
	{
		free(currentWord->type);
		free(currentWord->content);
		nextWord = currentWord->next;
		free(currentWord);
		currentWord = nextWord;
	}
	free(wordList);
}