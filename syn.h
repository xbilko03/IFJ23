/*
Name of project: Implementace překladače imperativního jazyka IFJ23
Author: Marcel Feiler
Login: xfeile00
*/

#ifndef SYN_H
#define SYN_H
#include <stdio.h>
#include "AST.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"

#define maxCommandTokenCount 255

/*
PerformSyntax -> general function for performing syntax control
returns: pass if main prog_con function was without errors
params: wrdList => list of tokens, DemoAST => root node necessary for AST structure
*/
void PerformSyntax(wordListStr* wrdList, Node* DemoAST);
/*
GetToken -> function for getting next tokens until EOF
returns: content and type of token
params: wrdList => list of tokens, ignoreNewLines => boolean value considering ignoring new lines or not, end_approved => boolean value considering if actual loaded token can be EOF
*/
wordStr* GetToken(wordStr* currentWord, bool ignoreNewLines, bool end_approved);
/*
GetFirstToken -> function for getting first token
returns: content and type of token
params: wrdList => list of tokens, ignoreNewLines => boolean value considering ignoring new lines or not, end_approved => boolean value considering if actual loaded token can be EOF
*/
wordStr* GetFirstToken(wordListStr* wrdList, bool ignoreNewLines, bool end_approved);
/*
prog_con -> containing rules for main body of program excluding return
returns: latest loaded token is success
params: wrdList => list of tokens, ignoreNewLines => boolean value considering ignoring new lines or not, end_approved => boolean value considering if actual loaded token can be EOF
*/
wordStr* prog_con(wordStr* currentWord, Node* DemoAST);
/*
statement -> containing rules for all possible statements including return
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* statement(wordStr* currentWord, Node* parent);
/*
opt -> control performing for 3rd rule of prog_con and its subcontrols
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* opt(wordStr* currentWord, Node* parent);
/*
builtin -> checking grammar rules for all builtin function excluding write function
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* builtin(wordStr* currentWord, Node* parent);
/*
arg -> checking given args of function declaration of 1st prog_con rule (function)
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* arg(wordStr* currentWord, Node* parent);
/*
args_more -> checking given args of function declaration in case of more than one arg in 1st prog_con rule (function)
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* args_more(wordStr* currentWord, Node* parent);
/*
types -> necessary branching function for possible notation of 1st prog_con rule (function)
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* types(wordStr* currentWord, Node* parent);
/*
params -> checking param in case of called user defined function
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* params(wordStr* currentWord, Node* parent);
/*
params_more -> checking params in case there are many of them during call of user defined function 
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* params_more(wordStr* currentWord, Node* parent);
/*
type_spec -> controlling whether given data types are valid
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* type_spec(wordStr* currentWord, Node* parent);
/*
write_params -> controlling param of called builtin function write
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* write_params(wordStr* currentWord, Node* parent);
/*
write_params_more -> controlling params in case of many when called builtin function write
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* write_params_more(wordStr* currentWord, Node* parent);
/*
option -> controlling all reachable scenarios and rules for 2nd prog_con rule (declaration/definition of variable)
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* option(wordStr* currentWord, Node* parent);
/*
return_value -> controlling possible rules for tokens after return token
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* return_value(wordStr* currentWord, Node* parent);
/*
expression -> syntax checking for expression in code
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* expression(wordStr* currentWord, Node* parent);
/*
expression_more -> syntax checking for all expressions in case of many of them in code
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* expression_more(wordStr* currentWord, Node* parent);
/*
term -> checking syntax rules for actual term, whether it's able to apply it to exact rule or not
returns: latest loaded token is success
params: currentWord => actual token, parent => actual AST parent node loaded 
*/
wordStr* term(wordStr* currentWord, Node* parent);

#endif