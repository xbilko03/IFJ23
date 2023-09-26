#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//---functions.c---
struct word;
struct wordList;
//------Lex.c------
void PerformLex();
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