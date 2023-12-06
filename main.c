/**
 * @name IFJ2023 Compiler
 * @file main.c
 * @author Viliam Chudáčik - xchuda06, Jozef Bilko - xbilko03, Marcel Feiler - xfeile00, Juraj Budai - xbudai02
 */

#include <stdio.h>
#include "AST.h"
#include <stdlib.h>
#include "lex.h"
#include "syn.h"
#include "sem.h"
#include "codegen.h"
#include "lex_struct.h"
#include "symtable.h"

int main(int argc, char **argv)
{
	//Create Word List
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr));
	wordList->first = malloc(sizeof(wordStr));
	wordList->last = wordList->first;
	wordList->first->type = NULL;
	wordStr* LastToken = wordList->first;
	
	//Fill Word List and Check for Lex Rules
	Tokenizer(LastToken);

	Node* AST = NULL;
	TRP* global = NULL;
	Node* root = Node_insert(&AST, "root", NULL, "root");

	PerformSyntax(wordList, &(*root));
	PerformSemantics(&root, &global);

	PerformCodeGen(&(*root));
	return 0;
}
