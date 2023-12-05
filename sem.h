#ifndef SEM_H
#define SEM_H
#include <stdio.h>
#include "AST.h"
#include "lex_struct.h"
#include <string.h>
#include "error.h"

void PerformSemantics(Node** AST, TRP** global);
void Go_through(struct Node* root, struct TRP* table, struct TRP* global, TRPitem* function);
void Type_of_node (struct Node* root, TRP* table, struct TRP* global, TRPitem* function);
void Go_through_function(Node* root, TRP* global);
void Add_function_to_symtable (Node* root, TRP* global);
char* check_for_type (Node* root, char* type, TRP* global, char* identifier_or_value);
//void check_for_expresions (char* key, TRP* table, struct TRP* global, TRPitem* function);
#endif