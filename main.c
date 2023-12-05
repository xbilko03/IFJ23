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
	//printf("\nBegin\n");
	
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

	//PrintWordList(wordList->first);

	//printf("--------------------------------\n\n");

	PerformSyntax(wordList, &(*root));
	bool* flag = malloc(500 * sizeof(bool));
    
	print_AST(AST, flag, 0, false);
    free(flag);

	PerformSemantics(&root, &global);

	PerformCodeGen(&(*root));

	//DeleteWordList(wordList);

	//printf("\nEnd\n");
	return 0;
}