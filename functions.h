#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define hashTableSize 255

//---functions.c---

typedef struct TRPitem {
	char* content;
	struct TRPitem* next;
} TRPitem;

typedef struct Node{
	struct Node **children;
	struct Node *parent;
	int numChildren;
	char* content;
	TRPitem *TRP;
	char* type;
} Node;

typedef struct wordStr wordStr;
struct wordStr {
	char* content;
	char* type;
	struct	wordStr* next;
};
typedef struct {
	struct wordStr* first;
	struct wordStr* last;
} wordListStr;



typedef struct TRP{
	TRPitem* items[hashTableSize];
}TRP;

//------Lex.c------
void PerformLex(wordListStr* wrdList);
//-----------------
//------Syn.c------
void PerformSyntax(wordListStr* wrdList, Node* DemoAST);
//-----------------
//------Sem.c------
void PerformSemantics(Node* DemoAST);
//-----------------
//------TRP.c------
//-----------------
//----Codegen.c----
void PerformCodeGen(Node* DemoAST);
//-----------------
//-----error.c-----
void ExitProgram(int returnCode, char* message);
//-----------------
//-----functions.c-----
void DeleteWordList(wordListStr* wordList);
long HashFunction(char* key);
Node* Node_insert(Node** AST, char* content, TRPitem* TRP, char* type);
int Node_merge(Node* children, Node** parent);
void* checked_malloc(size_t size);
void* checked_realloc(void* ptr, size_t size);
void print_AST(Node* tree, bool* flag, int depth, bool isLast);
//-----------------
#endif