#include "AST.h"

Node* Node_insert(Node** parentAST, char* content, TRP* TRP, char* type)
{
	//alocate memory for new node and set its values
	Node* childrenAST = (Node*)checked_malloc(sizeof(Node));
	childrenAST->type = type;
	childrenAST->children = NULL;
	childrenAST->numChildren = 0;
	childrenAST->content = content;
	childrenAST->TRP = TRP;

	//if parent is null, set parent to new node and return
	if(*parentAST == NULL)
	{
		*parentAST = childrenAST;
		return childrenAST;
	}
	//else, set parent of new node to parent, increment number of children of parent, and add new node to children of parent
	childrenAST->parent = *parentAST;
	(*parentAST)->numChildren++;
	(*parentAST)->children = (Node**)checked_realloc((*parentAST)->children, sizeof(Node*) * ((*parentAST)->numChildren));
	(*parentAST)->children[(*parentAST)->numChildren-1] = childrenAST;
	return childrenAST;
}

//Function that allocates memory and exits program if malloc fails
void* checked_malloc(size_t size)
{
	void* ptr = malloc(size);
	if(ptr == NULL)
	{
		ExitProgram(99, "malloc error\n");
	}
	return ptr;
}

//Function that reallocates memory and exits program if realloc fails
void* checked_realloc(void* ptr, size_t size)
{
	void* newPtr = realloc(ptr, size);
	if(newPtr == NULL)
	{
		ExitProgram(99, "realloc error\n");
	}
	return newPtr;
}