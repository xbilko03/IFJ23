#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
//------Lex.c------
void PerformLex(wordListStr* wrdList);
//-----------------
//------Syn.c------
//-----------------
//------Sem.c------
//-----------------
//------TRP.c------
//-----------------
//----Codegen.c----
//-----------------
//-----error.c-----
void ExitProgram(int returnCode, char* message);
//-----------------
#endif