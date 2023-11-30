#ifndef CODEGEN_H
#define CODEGEN_H
#include <stdio.h>
#include "AST.h"
#include "symtable.h"
#include <string.h>

void PerformCodeGen(Node* DemoAST);
void ProcessNode(Node* c_node);
void PrintCode(char* code);
void INBUILT_WRITE(Node* c_node);
void DEFVAR(Node* c_var);
void MOVE(Node* c_var, Node* c_symb);

#endif