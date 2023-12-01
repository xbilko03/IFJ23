#ifndef SEM_H
#define SEM_H
#include <stdio.h>
#include "AST.h"
#include "lex_struct.h"
#include <string.h>

void PerformSemantics(Node** AST, TRP** global);
void Go_through(struct Node* root, struct TRP* table, struct TRP* global, TRPitem* function);
void Type_of_node (struct Node* root, TRP* table, struct TRP* global, TRPitem* function);
#endif