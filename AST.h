#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "symtable.h"
#include "lex_struct.h"
#define hashTableSize 255

typedef struct Node{
	struct Node **children;
	struct Node *parent;
	int numChildren;
	char* content;
	TRPitem *TRP;
	char* type;
} Node;

void DeleteWordList(wordListStr* wordList);
Node* Node_insert(Node** AST, char* content, TRPitem* TRP, char* type);
void* checked_malloc(size_t size);
void* checked_realloc(void* ptr, size_t size);
void print_AST(Node* tree, bool* flag, int depth, bool isLast);
#endif
