/**
 * @name IFJ2023 Compiler
 * @file codegen.h
 * @author Jozef Bilko - xbilko03, Viliam Chudáčik - xchuda06
 */

#ifndef CODEGEN_H
#define CODEGEN_H
#include <stdio.h>
#include "AST.h"
#include "symtable.h"
#include <string.h>

/* Pass through all the nodes of the DemoAST and puts the IFJCODE23 code to the stdout */
void PerformCodeGen(Node* DemoAST);

void ProcessNode(Node* c_node);
void PrintCode(char* code);
void INBUILT_WRITE(Node* c_node);
void DEFVAR(Node* c_var);
void MOVE(Node* c_var, Node* c_symb);
void ADD(Node* c_symb);
void SUB(Node* c_symb);
void IDIV(Node* c_symb);
void MUL(Node* c_symb);
void INBUILT_LENGTH(Node* c_node);
void INBUILT_INT2DOUBLE(Node* c_node);
void INBUILT_READSTRING(Node* c_node);
void INBUILT_READINT(Node* c_node);
void INBUILT_READDOUBLE(Node* c_node);
void INBUILT_WRITE(Node* c_node);
void INBUILT_DOUBLE2INT(Node* c_node);
void INBUILT_SUBSTRING(Node* c_node);
void INBUILT_ORD(Node* c_node);
void INBUILT_CHR(Node* c_node);

#endif