#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define hashTableSize 255

//---functions.c---
typedef struct wordStr wordStr;
struct wordStr{
	char* content;
	char* type;
	struct	wordStr* next;
};
typedef struct {
	struct wordStr* first;
	struct wordStr* last;
} wordListStr;
typedef struct{
	struct wordStr* items[hashTableSize];
}TRP;
//------Lex.c------
void PerformLex(wordListStr* wrdList);
//-----------------
//------Syn.c------
typedef struct {
	char* type;
	char* content;
	struct node* left;
	struct node* right;
} node;
void PerformSyntax();
//-----------------
//------Sem.c------
void PerformSemantics();
//-----------------
//------TRP.c------
//-----------------
//----Codegen.c----
void PerformCodeGen();
//-----------------
//-----error.c-----
void ExitProgram(int returnCode, char* message);
//-----------------
//-----functions.c-----
void DeleteWordList(wordListStr* wordList);
long HashFunction(char* key);
//-----------------
#endif