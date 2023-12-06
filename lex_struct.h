/**
 * @name IFJ2023 Compiler
 * @file lex_struct.h
 * @author Jozef Bilko - xbilko03, Viliam Chudáčik - xchuda06
 */

#ifndef LEX_STRUCT_H
#define LEX_STRUCT_H
typedef struct wordStr{
	char* content;
	char* type;
	struct	wordStr* next;
} wordStr;
typedef struct wordListStr{
	struct wordStr* first;
	struct wordStr* last;
} wordListStr;
#endif