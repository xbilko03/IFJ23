#include <stdio.h>
#include "functions.h"

#define maxCommandTokenCount 255

command* CreateCommand(wordStr* Words[maxCommandTokenCount], unsigned tokenCount, char* type);

void PerformSyntax(wordListStr* wrdList)
{
	wordStr* currentWord = wrdList->first;

	wordStr* tokenList[maxCommandTokenCount] = {NULL};
	unsigned i = 0;

	while (currentWord != NULL)
	{
		/*
		* program
		*	command** variableList
		*	command** functionsList
		*	command** body
		* function
		*	command** params
		*	command** body
		* if
		*	command** condition
		*	command** body
		* else
		*	command** body
		*/

		tokenList[i] = currentWord; //Let Tokens form a command
		
		if (i >= 10) //Command creation condition is just temporary, just to test smth
		{
			CreateCommand(tokenList, i, "commandType"); //Save command
			i = 0;
		}
		
		i++;
		currentWord = currentWord->next;
	}

	return;
}

command* CreateCommand(wordStr* Words[maxCommandTokenCount], unsigned tokenCount, char* type)
{
	unsigned i;
	printf("command = ");
	for (i = 0; i < tokenCount; i++)
	{
		printf("%s", Words[i]->content);
	}
	printf("\n");
	return NULL;
}

