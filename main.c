#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include "lex.h"

int main(int argc, char **argv)
{
	printf("\nBegin\n");
	
	//Create Word List
	wordListStr* wordList;
	wordList = malloc(sizeof(wordListStr));
	wordList->first = malloc(sizeof(wordStr));
	wordList->last = wordList->first;
	wordList->first->type = NULL;
	wordStr* LastToken = wordList->first;
	
	//Fill Word List and Check for Lex Rules
	Tokenizer(LastToken);

	//DEMO AST
	Node* DemoAST = NULL;
	Node* test1 = Node_insert(&DemoAST, "test1", NULL, "test");
	Node* test2 = Node_insert(&test1, "test2", NULL, "test");
	Node_insert(&test1, "test22", NULL, "test");
	Node_insert(&test1, "test23", NULL, "test");
	Node* test3 = Node_insert(&test2, "test3", NULL, "test");
	Node_insert(&test2, "test3", NULL, "test");
	Node_insert(&test3, "test41", NULL, "test");
	Node_insert(&test3, "test42", NULL, "test");
	Node_insert(&test3, "test43", NULL, "test");
	bool* flag = malloc(500 * sizeof(bool));
    print_AST(DemoAST, flag, 0, false);
    free(flag);
	//END DEMO AST

	Node* AST = NULL;

	PrintWordList(wordList->first);

	printf("--------------------------------\n\n");

	PerformSyntax(wordList);

	PerformSemantics();

	PerformCodeGen();

	//DeleteWordList(wordList);

	printf("\nEnd\n");
	return 0;
}
