#ifndef CODEGEN_H
#define CODEGEN_H
#include <stdio.h>
#include "AST.h"
#include "symtable.h"
#include <string.h>

/* Pass through all the nodes of the DemoAST and puts the IFJCODE23 code to the stdout */
void PerformCodeGen(Node* DemoAST);

#endif