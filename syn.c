#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

//In use
node* CreateNode(char* content, char* type);
wordStr* SkipNewlines(wordStr* currentWord);
wordStr* prog_con(wordStr* currentWord, unsigned* level, node* currentNode);
wordStr* args(wordStr* currentWord, unsigned* level);
wordStr* option(wordStr* currentWord, unsigned* level);
wordStr* func(wordStr* currentWord, unsigned* level);
wordStr* expression(wordStr* currentWord, unsigned* level);

//unsued
int prog(wordStr* currentWord);
int builtin(wordStr* currentWord);
int types(wordStr* currentWord);
int type_spec(wordStr* currentWord);
int write_params(wordStr* currentWord);
int write_params_more(wordStr* currentWord);



void PerformSyntax(wordListStr* wrdList) {
	
	wordStr* currentWord = wrdList->first;
	
	unsigned level = 1;

	currentWord = SkipNewlines(currentWord);

	node* program = CreateNode("root", "program");
	//node* currentNode;

	int i = 0; //TEMPORARY, remove when ready

	while (currentWord != NULL && i++ < 24)
	{
		//NewLine Workaround
		currentWord = SkipNewlines(currentWord);
		//NewLine Workaround

		currentWord = prog_con(currentWord, &level, program);

		//NewLine Workaround
		currentWord = SkipNewlines(currentWord);
		//NewLine Workaround
	}



	printf("\n\nNodeTree = %s of type %s", (program->content), program->type);
	if (program->left != NULL)
		printf("\n\nLeft = %s of type %s", (program->left->content), program->left->type);

	if (program->right != NULL)
		printf("\n\nRight = %s of type %s",(program->right->content), program->right->type);
	return;
}


//GRAMMAR

wordStr* prog_con(wordStr* currentWord, unsigned* level, node* currentNode)
{
	printf("--------\nprog_con:\n-------\n");

	//identifier
	if (strcmp(currentWord->type, "identifier") == 0)
	{
		char* functName = currentWord->content;
		printf("%s\n", currentWord->content);
		//Take nextToken
		currentWord = currentWord->next;
		//identifier(args)
		if (strcmp(currentWord->content, "(") == 0)
		{
			currentWord = func(currentWord, level);
			currentNode->right = CreateNode(functName, "functionCall");
		}
		else if (strcmp(currentWord->content, "=") == 0)
		{
			printf("%s\n", currentWord->content);
			currentWord = expression(currentWord, level);
		}
			
	}
	else if (strcmp(currentWord->type, "keyword") == 0)
	{
		printf("%s\n", currentWord->content);
		if (strcmp(currentWord->content, "else") == 0)
		{
			currentWord = currentWord->next;
			currentWord = SkipNewlines(currentWord);
		}
		if (strcmp(currentWord->content, "let") == 0 
			|| 
			strcmp(currentWord->content, "var") == 0)
		{
			//let
			currentWord = SkipNewlines(currentWord);
			currentWord = currentWord->next;
			//let a
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);

				if (strcmp(currentWord->content, ":") == 0)
					currentWord = option(currentWord, level);
			}			
			//let a Int?
			if (strcmp(currentWord->type, "identifier(type)") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
			}
			//let a Int? =
			if (strcmp(currentWord->content, "=") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = expression(currentWord, level);
			}
			//toto tu je pizdec, pujde to jinam
			if (strcmp(currentWord->type, "integer") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
			}

			//let a Int? = readInt
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				currentWord = SkipNewlines(currentWord);
				currentWord = currentWord->next;
				//readInt(args)
				if (strcmp(currentWord->content, "(") == 0)
					currentWord = func(currentWord, level);
			}
			//let a Int? = readInt()
		}
		else if (strcmp(currentWord->content, "if") == 0
			||
			strcmp(currentWord->content,"while") == 0)
		{
			//if
			currentWord = SkipNewlines(currentWord);
			currentWord = currentWord->next;

			//if (
			if (strcmp(currentWord->content, "(") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
			}
			
			//if let
			if (strcmp(currentWord->content, "let") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
			}

			//if a
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = SkipNewlines(currentWord);
				currentWord = currentWord->next;
								
				if (strcmp(currentWord->type, "sign") == 0) //It is expression
				{
					printf("%s\n", currentWord->content);
					currentWord = currentWord->next;
					currentWord = expression(currentWord, level);
				}
				if (strcmp(currentWord->content, ")") == 0)
				{
					printf("%s\n", currentWord->content);
					currentWord = currentWord->next;
				}
			}
		}	

	}

	if (strcmp(currentWord->content, "{") == 0)
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
		(*level)++;
	}
	else if (strcmp(currentWord->content, "}") == 0)
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
		(*level)--;
	}

	return currentWord; //return to prog_con
}
wordStr* func(wordStr* currentWord, unsigned* level)
{
	printf("%s\n", currentWord->content);
	//identifier(args
	currentWord = currentWord->next;
	currentWord = args(currentWord, level);

	currentWord = SkipNewlines(currentWord);
	printf("%s\n", currentWord->content);
	//identifier(args)
	if (strcmp(currentWord->content, ")") == 0)
	{
		//CreateNode(functionName, functionType)
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
		return currentWord;
	}

	return currentWord;
}	
wordStr* args(wordStr* currentWord, unsigned* level) {
	if (strcmp(currentWord->type, "string") == 0) //is OK
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
	}
	else if (strcmp(currentWord->type, "identifier") == 0) //is OK
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
	}
	if (strcmp(currentWord->content, ",") == 0) //is OK
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
		currentWord = args(currentWord, level);
		return currentWord;
	}
	return currentWord;
}
wordStr* option(wordStr* currentWord, unsigned* level) {

	printf("%s\n", currentWord->content);
	currentWord = currentWord->next;
	currentWord = SkipNewlines(currentWord);

	return currentWord;
}
//expression je dofakany, zitra opravime
wordStr* expression(wordStr* currentWord, unsigned* level)
{
	currentWord = currentWord->next;
	currentWord = SkipNewlines(currentWord);

	if (strcmp(currentWord->type, "integer") == 0)
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
	}
	if (strcmp(currentWord->type, "identifier") == 0)
	{
		printf("%s\n", currentWord->content);
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
		if (strcmp(currentWord->content, "(") == 0)
			currentWord = func(currentWord, level);
		else if (strcmp(currentWord->type, "sign") == 0)
		{
			printf("%s\n", currentWord->content);
			currentWord = currentWord->next;
			currentWord = SkipNewlines(currentWord);
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
			}
			else if (strcmp(currentWord->type, "integer") == 0)
			{
				printf("%s\n", currentWord->content);
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
			}
		}
	}
	return currentWord;
}

//NODES//

node* CreateNode(char* content, char* type)
{
	node* newNode = malloc(sizeof(node));
	newNode->type = malloc(sizeof(char) * (strlen(type) + 1));
	newNode->content = malloc(sizeof(char) * (strlen(content) + 1));
	strcpy(newNode->type, type);
	strcpy(newNode->content, content);
	newNode->right = NULL;
	newNode->left = NULL;

	return newNode;
}
void BindNode(node* parent, node* child, char* direction) 
{
	if (strcmp(direction, "left") == 0) {
		parent->left = child;
	}
	else if (strcmp(direction, "right") == 0) {
		parent->right = child;
	}
}

//OTHER//

wordStr* SkipNewlines(wordStr* currentWord)
{
	if (currentWord == NULL)
		return NULL;
	while (currentWord == NULL || strcmp(currentWord->content, "newline") == 0) //Skip all newlines before first valid token
	{
		currentWord = currentWord->next;
	}
	return currentWord;
}