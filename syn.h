#ifndef SYN_H
#define SYN_H
#include <stdio.h>
#include "AST.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"

#define maxCommandTokenCount 255


void PerformSyntax(wordListStr* wrdList, Node* DemoAST);

wordStr* GetToken(wordStr* currentWord, bool ignoreNewLines, bool end_approved);
wordStr* GetFirstToken(wordListStr* wrdList, bool ignoreNewLines, bool end_approved);

wordStr* prog_con(wordStr* currentWord, Node* DemoAST);
wordStr* statement(wordStr* currentWord, Node* parent);
wordStr* opt(wordStr* currentWord, Node* parent);
wordStr* builtin(wordStr* currentWord, Node* parent);
wordStr* arg(wordStr* currentWord, Node* parent);
wordStr* args_more(wordStr* currentWord, Node* parent);
wordStr* types(wordStr* currentWord, Node* parent);
wordStr* params(wordStr* currentWord, Node* parent);
wordStr* params_more(wordStr* currentWord, Node* parent);
wordStr* type_spec(wordStr* currentWord, Node* parent);
wordStr* write_params(wordStr* currentWord, Node* parent);
wordStr* write_params_more(wordStr* currentWord, Node* parent);
wordStr* option(wordStr* currentWord, Node* parent);
wordStr* return_value(wordStr* currentWord, Node* parent);
wordStr* expression(wordStr* currentWord, Node* parent);
wordStr* expression_more(wordStr* currentWord, Node* parent);
wordStr* term(wordStr* currentWord, Node* parent);

#endif