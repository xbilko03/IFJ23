/**
 * @name IFJ2023 Compiler
 * @file AST.h
 * @author Viliam Chudáčik - xchuda06
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "symtable.h"
#include "lex_struct.h"

typedef struct TRP TRP;
typedef struct Node{
	struct Node **children;
	struct Node *parent;
	int numChildren;
	char* content;
	TRP *TRP;
	char* type;
} Node;

Node* Node_insert(Node** AST, char* content, TRP* TRP, char* type);
void* checked_malloc(size_t size);
void* checked_realloc(void* ptr, size_t size);
#endif
