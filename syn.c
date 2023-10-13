#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>

#define maxCommandTokenCount 255

node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList)
{
	wordStr* cToken = wrdList->first;
	//node* programRoot;
	node* cNode;


	while (cToken != NULL)
	{
		cNode = CreateNode(cToken->content, "testType");
		printf("Node = %s of type %s\n", cNode->content, cNode->type);




		//-------------------//
		cToken = cToken->next;
	}

	return;
}

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
void BindNode(node* parent, char* direction)
{

}