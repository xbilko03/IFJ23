#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lex_struct.h"
#define hashTableSize 255

typedef struct TRPitem {
	char* key;
	bool* content;
	struct wordStr* type;
	struct TRPitem* next;
} TRPitem;

typedef struct TRP{
	TRPitem* items[hashTableSize];
	struct TRP* next;
}TRP;

long HashFunction(char* key);
TRP* TableInit(TRP* table);
TRPitem* TableFindItem(TRP* table, char *key);
void TableAddItem(TRP* table, char* key, wordStr* type, bool* content);
void TableRemoveTable(TRP* table);

#endif