#include <stdio.h>
#include "functions.h"

void PerformSyntax(wordListStr* wrdList)
{
	wordStr* currentWord = wrdList->first;
	
	printf("%s\n",currentWord->content);
	currentWord = currentWord->next;
	printf("%s\n",currentWord->content);

	return;
}