#ifndef LEX_H
#define LEX_H
#include <ctype.h> 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "functions.h"

typedef enum MachineStates
{
    Start,
    String_start,
    String_end,
    String_escape,
    Operator3,
    Comment_one_line,
    Comment_multi_line_start,
    Comment_multi_line_end1,
    Comment_multi_line_end2,
    Compare_start,
    Compare_end,
    Open_bracket,
    Close_bracket,
    Open_block,
    Close_block,
    Colon,
    Comma,
    Operator,
    Operator4,
    Arrow,
    Assign,
    Compare,
    ExclamationMark,
    Compare2,
    Identifier,
    Identifier_type,
    Identifier_name,
    Integer,
    Double_start,
    Double,
    Double_exp,
    Double_exp_op,
    Double_exp_op2,
    Newline,
    Error,
    Endoffile,
    End,
    String_multi,
    String_multi_escape,
    String_multi_end1,
    String_multi_end2,
    String_multi_end3,
    String_content,
    String_end_multi
} MachineStates;



MachineStates StateMachine (char input, MachineStates currentState);
void Tokenizer(wordStr *wordList);
void AddToken(MachineStates state, wordStr **wordList, char* word);
char* EscapeSequence(char *word, bool multi);
char HexEscape(char* word, int *i, int *lenght);
bool Keyword(char* word);
bool Type(char* word);
void PrintWordList(wordStr *wordList);

#endif