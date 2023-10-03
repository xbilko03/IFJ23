#include <stdio.h>
#include "functions.h"

void PerformSyntax(wordListStr* wrdList)
{
	wordStr* currentWord = wrdList->first;
	
	while(currentWord != NULL)
	{
		printf("%s  -  %s\n",currentWord->content, currentWord->type);
		currentWord = currentWord->next;
	}
	return;
}