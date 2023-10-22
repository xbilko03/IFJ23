#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

//In use
void printTree(node* root, int level);
void BindNode(node* parent, node* child, char* direction);
node* CreateNode(char* content, char* type);
wordStr* SkipNewlines(wordStr* currentWord);
wordStr* prog_con(wordStr* currentWord, int* level, node** currentNode);
wordStr* args(wordStr* currentWord, int* level, node** currentNode);
wordStr* func(wordStr* currentWord, char* id, char* direction, int* level, node** currentNode);
wordStr* expression(wordStr* currentWord, wordStr* lastWord, char* direction, int* level, node** currentNode);
wordStr* option(wordStr* currentWord, int* level);

//unsued
int prog(wordStr* currentWord);
int builtin(wordStr* currentWord);
int types(wordStr* currentWord);
int type_spec(wordStr* currentWord);
int write_params(wordStr* currentWord);
int write_params_more(wordStr* currentWord);



void PerformSyntax(wordListStr* wrdList) {
	
	wordStr* currentWord = wrdList->first;
	
	int level = 1;

	currentWord = SkipNewlines(currentWord);

	node* program = CreateNode("root", "program");
	node* currentNode = program;

	currentWord = SkipNewlines(currentWord);
	currentWord = prog_con(currentWord, &level, &currentNode);

	//printf("ENTERING LAST PROGCON=%s\n\n", currentWord->content);

	printTree(program, 1);

	return;
}

void printTree(node* root, int level)
{
	node* currentNode = root;

	for (int i = 0; i < level; i++)
		printf(i == level - 1 ? "|-" : "  ");

	if(strcmp(currentNode->type,"string") == 0)
		printf("'TESTSTRING' of type '%s'\n", currentNode->type);
	else
		printf("'%s' of type '%s'\n", currentNode->content, currentNode->type);
	if (currentNode->left != NULL)
		printTree(currentNode->left, (level + 1));
	if (currentNode->right != NULL)
		printTree(currentNode->right, (level + 1) );
}

//GRAMMAR

wordStr* prog_con(wordStr* currentWord, int* level, node** currentNode)
{
	if (currentWord == NULL)
		return NULL;

	if(strcmp(currentWord->type,"newline") == 0)
		currentWord = SkipNewlines(currentWord);
	
	//<prog_con> id
	if (strcmp(currentWord->type, "identifier") == 0)
	{
		char* id = currentWord->content;
		wordStr* lastWord = currentWord;
		currentWord = currentWord->next;

		//<prog_con> func_id(<args>)
		if (strcmp(currentWord->content, "(") == 0)
		{
			currentWord = func(currentWord, id, "right", level, currentNode);

			//<prog_con> func_id(<args>) <prog_con>
			currentWord = prog_con(currentWord, level, currentNode);
			printf("level= %d \n", *level);
			return currentWord;
		}
		//<prog_con> id =
		else if (strcmp(currentWord->content, "=") == 0) 
		{
			node* newNode = CreateNode(id, "identifier");
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;
			(*level)++;

			currentWord = currentWord->next;
			currentWord = SkipNewlines(currentWord);
			//<prog_con> id = id

			if (strcmp(currentWord->type, "identifier") == 0)
			{
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
				if (strcmp(currentWord->content, "*") == 0 || strcmp(currentWord->content, "-") == 0)
				{
					char* direction = "left";
					currentWord = expression(currentWord, lastWord, direction, level, currentNode);

					//<prog_con> id = <expression> <prog_con>
					*currentNode = (*currentNode)->parent;
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
			}
		}

	}
	//<prog_con> <keyword>
	else if (strcmp(currentWord->type, "keyword") == 0)
	{
		//<prog_con> else
		if (strcmp(currentWord->content, "else") == 0)
		{
			node* newNode = CreateNode(currentWord->content, "branching");
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;
			(*level)++;

			currentWord = currentWord->next;
			currentWord = SkipNewlines(currentWord);

			currentWord = prog_con(currentWord, level, currentNode);
			return currentWord;
		}
		//<prog_con> let
		else if (strcmp(currentWord->content, "let") == 0)
		{
			currentWord = SkipNewlines(currentWord);
			currentWord = currentWord->next;
			//<prog_con> let id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				node* newNode = CreateNode(currentWord->content, "constant declaration");
				BindNode(*currentNode, newNode, "right");
				*currentNode = newNode;
				(*level)++;

				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);

				//<prog_con> let id <option>
				if (strcmp(currentWord->content, ":") == 0)
				{
					currentWord = option(currentWord, level);

					//<prog_con> let id <option> <typeIdentifier>
					if (strcmp(currentWord->type, "identifier(type)") == 0)
					{
						node* newNode = CreateNode(currentWord->content, "constant declaration");
						BindNode(*currentNode, newNode, "left");
						*currentNode = newNode;

						currentWord = currentWord->next;
						currentWord = SkipNewlines(currentWord);

						//<prog_con> let id <option> <typeIdentifier> =
						if (strcmp(currentWord->content, "=") == 0)
						{
							currentWord = currentWord->next;
							currentWord = SkipNewlines(currentWord);

							//<prog_con> let id <option> <typeIdentifier> = id
							if (strcmp(currentWord->type, "identifier") == 0)
							{
								char* id = currentWord->content;
								currentWord = currentWord->next;
								currentWord = SkipNewlines(currentWord);
								//<prog_con> let id <option> <typeIdentifier> = id(<args>)
								if (strcmp(currentWord->content, "(") == 0)
								{
									currentWord = func(currentWord, id, "right", level, currentNode);

									//<prog_con> let id <option> <typeIdentifier> = id(<args>) <prog_con>
									*currentNode = (*currentNode)->parent;
									*currentNode = (*currentNode)->parent;
									currentWord = prog_con(currentWord, level, currentNode);
									return currentWord;
								}
							}
						}
					}
				}
			}

		}
		//<prog_con> var
		else if (strcmp(currentWord->content, "var") == 0)
		{
			currentWord = SkipNewlines(currentWord);
			currentWord = currentWord->next;
			//<prog_con> var id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				char* id = currentWord->content;

				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);

				//<prog_con> var id <option>
				if (strcmp(currentWord->content, ":") == 0)
				{
					node* newNode = CreateNode(id, "variable declaration");
					BindNode(*currentNode, newNode, "right");
					*currentNode = newNode;
					(*level)++;

					currentWord = option(currentWord, level);

					//<prog_con> var id <option> <typeIdentifier>
					if (strcmp(currentWord->type, "identifier(type)") == 0)
					{
						node* newNode = CreateNode(currentWord->content, "constant declaration");
						BindNode(*currentNode, newNode, "left");
						*currentNode = newNode;

						currentWord = currentWord->next;
						currentWord = SkipNewlines(currentWord);

						//<prog_con> var id <option> <typeIdentifier> =
						if (strcmp(currentWord->content, "=") == 0)
						{
							currentWord = currentWord->next;
							currentWord = SkipNewlines(currentWord);

							//<prog_con> var id <option> <typeIdentifier> = id
							if (strcmp(currentWord->type, "identifier") == 0)
							{
								char* id = currentWord->content;
								currentWord = currentWord->next;
								currentWord = SkipNewlines(currentWord);
								//<prog_con> var id <option> <typeIdentifier> = id(<args>)
								if (strcmp(currentWord->content, "(") == 0)
								{
									currentWord = func(currentWord, id, "right", level, currentNode);

									//<prog_con> var id <option> <typeIdentifier> = id(<args>) <prog_con>
									*currentNode = (*currentNode)->parent;
									*currentNode = (*currentNode)->parent;
									currentWord = prog_con(currentWord, level, currentNode);
									return currentWord;
								}
							}
							//<prog_con> var id <option> <typeIdentifier> = <integer>
							else if (strcmp(currentWord->type, "integer") == 0)
							{
								node* newNode = CreateNode(currentWord->content, "integer");
								BindNode(*currentNode, newNode, "left");
								*currentNode = newNode;

								currentWord = currentWord->next;
								currentWord = SkipNewlines(currentWord);

								*currentNode = (*currentNode)->parent;
								currentWord = prog_con(currentWord, level, currentNode);
								return currentWord;
							}

						}
					}
					
				}
				//<prog_con> var id =
				else if (strcmp(currentWord->content, "=") == 0)
				{
					node* newNode = CreateNode(id, "variable declaration");
					BindNode(*currentNode, newNode, "right");
					*currentNode = newNode;
					(*level)++;

					currentWord = currentWord->next;
					currentWord = SkipNewlines(currentWord);

					//<prog_con> var id <option> = id
					if (strcmp(currentWord->type, "identifier") == 0)
					{
						char* id = currentWord->content;
						currentWord = currentWord->next;
						currentWord = SkipNewlines(currentWord);
						if (strcmp(currentWord->content, "(") == 0)
						{
							currentWord = func(currentWord, id, "left", level, currentNode);
							//<prog_con> var id <option> = id <prog_con>

							*currentNode = (*currentNode)->parent;
							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}

		}
		//<prog_con> while
		else if (strcmp(currentWord->content, "while") == 0) 
		{
			node* newNode = CreateNode(currentWord->content, "loop");
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;
			(*level)++;
			currentWord = currentWord->next;
			currentWord = SkipNewlines(currentWord);

			//<prog_con> while (
			if (strcmp(currentWord->content, "(") == 0)
			{
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
				//<prog_con> while (id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					wordStr* lastWord = currentWord;
					currentWord = SkipNewlines(currentWord);
					currentWord = currentWord->next;

					//<prog_con> while (<expression>
					if (strcmp(currentWord->content, ">") == 0)
					{
						char* direction = "left";
						currentWord = expression(currentWord, lastWord, direction, level, currentNode);
						//<prog_con> while (<expression>)
						if (strcmp(currentWord->content, ")") == 0)
						{
							currentWord = currentWord->next;
							currentWord = SkipNewlines(currentWord);

							//<prog_con> while (<expression>) <prog_con>
							*currentNode = (*currentNode)->parent;
							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}
		}
		//<prog_con> if
		else if(strcmp(currentWord->content, "if") == 0)
		{
			node* newNode = CreateNode(currentWord->content, "branching");
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;

			currentWord = SkipNewlines(currentWord);
			currentWord = currentWord->next;

			//<prog_con> if let
			if (strcmp(currentWord->content, "let") == 0)
			{
				currentWord = currentWord->next;
				currentWord = SkipNewlines(currentWord);
				//<prog_con> if let id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					node* newNode = CreateNode(currentWord->content, "constant declaration");
					BindNode(*currentNode, newNode, "left");
					*currentNode = newNode;

					currentWord = currentWord->next;
					currentWord = SkipNewlines(currentWord);

					//<prog_con> if let id <prog_con>
					*currentNode = (*currentNode)->parent;
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
			}

			//<prog_con> if (
			if (strcmp(currentWord->content, "(") == 0)
			{
				currentWord = SkipNewlines(currentWord);
				currentWord = currentWord->next;
				//<prog_con> if (id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					wordStr* lastWord = currentWord;
					currentWord = SkipNewlines(currentWord);
					currentWord = currentWord->next;
					
					//<prog_con> if (<expression>
					if (strcmp(currentWord->content, "<") == 0)
					{
						char* direction = "left";
						currentWord = expression(currentWord, lastWord, direction, level, currentNode);
						//<prog_con> if (<expression>)
						if (strcmp(currentWord->content, ")") == 0)
						{
							currentWord = SkipNewlines(currentWord);
							currentWord = currentWord->next;

							*currentNode = (*currentNode)->parent;
							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}
		}
	}
	
	if (strcmp(currentWord->content, "{") == 0)
	{
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);

		//<prog_con> { <prog_con>
		currentWord = prog_con(currentWord, level, currentNode);
		return currentWord;
	}
	else if (strcmp(currentWord->content, "}") == 0)
	{
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
		//<prog_con> { <prog_con>
		*currentNode = (*currentNode)->parent;
		currentWord = prog_con(currentWord, level, currentNode);

		return currentWord;
	}

	return currentWord; //return to prog_con
}
wordStr* func(wordStr* currentWord, char* id, char* direction, int* level, node** currentNode)
{
	node* newNode = CreateNode(id, "function");
	BindNode(*currentNode, newNode, direction);
	*currentNode = newNode;
	if(strcmp(direction,"right") == 0)
		(*level)++;

	currentWord = currentWord->next;
	
	currentWord = args(currentWord, level, &newNode);

	currentWord = SkipNewlines(currentWord);
	if (strcmp(currentWord->content, ")") == 0)
	{
		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);

		return currentWord;
	}
	else
		return NULL; //Should return error instead

}	
wordStr* args(wordStr* currentWord, int* level, node** currentNode) {
	
	node* newNode;

	if (strcmp(currentWord->type, "string") == 0) //is OK
	{
		newNode = CreateNode(currentWord->content, "string");
		BindNode(*currentNode, newNode, "left");
		*currentNode = newNode;
		currentWord = currentWord->next;
	}
	else if (strcmp(currentWord->type, "identifier") == 0) //is OK
	{
		newNode = CreateNode(currentWord->content, "identifier");
		BindNode(*currentNode, newNode, "left");
		*currentNode = newNode;
		currentWord = currentWord->next;
	}
	else if (strcmp(currentWord->type, "integer") == 0) //is OK
	{
		newNode = CreateNode(currentWord->content, "integer");
		BindNode(*currentNode, newNode, "left");
		*currentNode = newNode;
		currentWord = currentWord->next;
	}
	else
	{
		return currentWord; //No params
	}

	if (strcmp(currentWord->content, ",") == 0) //is OK
	{
		currentWord = currentWord->next;
		currentWord = args(currentWord, level, &newNode);
		return currentWord;
	}
	else
	{
		newNode = newNode->parent;
		*currentNode = newNode;
		(*level)--;
	}

	return currentWord;
}
wordStr* option(wordStr* currentWord, int* level) {
	currentWord = currentWord->next;
	currentWord = SkipNewlines(currentWord);

	return currentWord;
}
wordStr* expression(wordStr* currentWord, wordStr* lastWord, char* direction, int* level, node** currentNode)
{
	node* newNode = CreateNode(currentWord->content, "expression");
	BindNode(*currentNode, newNode, direction);
	*currentNode = newNode;
	if(strcmp(direction,"right") == 0)
		(*level)++;

	newNode = CreateNode(lastWord->content, "expression");
	BindNode(*currentNode, newNode, "left");
	*currentNode = newNode;

	currentWord = currentWord->next;
	currentWord = SkipNewlines(currentWord);
	if (strcmp(currentWord->type, "integer") == 0)
	{
		node* newNode = CreateNode(currentWord->content, "integer");
		BindNode(*currentNode, newNode, "right");

		*currentNode = (*currentNode)->parent;

		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
	}
	else if (strcmp(currentWord->type, "identifier") == 0)
	{
		node* newNode = CreateNode(currentWord->content, "identifier");
		BindNode(*currentNode, newNode, "right");

		*currentNode = (*currentNode)->parent;

		currentWord = currentWord->next;
		currentWord = SkipNewlines(currentWord);
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
	child->parent = parent;

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