#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdlib.h>

long HashFunction(char* key)
{
	long result = 0;
	int i;
	int a = strlen(key);
	for(i = 0; i < a; i++)
	{
		result+= key[i];
	}

	return result % hashTableSize;
}

void TableInit(TRP* table){
	table = malloc(sizeof(TRP));
	if (table == NULL){return;}
	for (int i = 0; i < hashTableSize; i++){
    		table->items[i] = NULL;
  	}
}

TRPitem* TableFindItem(TRP* table, char *key)
{
	int hash = HashFunction(key);
	TRPitem* current = table->items[hash];

	while (current != NULL) {
		if (current->type->content == key){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void TableAddItem(TRP* table, char* key, wordStr* type)
{
	int hash = HashFunction(key);
  	TRPitem *item = TableFindItem(table, key);

	if (item != NULL){ //already in table
		if (type != NULL){
			if (item->type == NULL){ // insert first

				item->type = malloc(sizeof(wordStr));
				if(item->type == NULL){return;}

				item->type = type;
				item->type->next = NULL;
			} else { // insert after
				while (item->type != NULL){
					if (item->type == type){
						//error code cant use same param in the function
					}
					if (item->type->next == NULL){
						wordStr* current = malloc(sizeof(wordStr));
						if (current == NULL){return;}

						current = type;
						item->type->next = current;

						return;
					}
					item->type = item->type->next;
				}
			}
		}
		return;
	} else { //not in the table
		
		TRPitem *new_item = malloc(sizeof(TRPitem));
		if (new_item == NULL){return;}

		new_item->key = key;

		if (type != NULL){
			new_item->type = malloc(sizeof(wordStr));
			if (new_item->type == NULL){return;}

			new_item->type = type;
			new_item->type->next = NULL;
			new_item->next = NULL;
		}

		if (table->items[hash] == NULL){ 
		table->items[hash] = new_item;
		return;
		} else {
			TRPitem *last_item = table->items[hash];

			while (last_item != NULL){ 
				if (last_item->next != NULL){
				last_item = last_item->next;
				} else {
				last_item->next = new_item;
				return;
				}
			}
		}
  	}
}
void TableRemoveTypes (wordStr* type){
	wordStr* current = type;
	wordStr* next;

	while (current != NULL){
		next = current->next;
		free(current);
		current = next;
	}
	type = NULL;
}
void TableRemoveTable(TRP* table)
{
	for (int i = 0; i < hashTableSize; i++){
		TRPitem *current = table->items[i];
		
		while (current != NULL){
			TableRemoveTypes(current->type);
			TRPitem *next = current->next;
			free(current);
			current = next;
		}
		table->items[i] = NULL;
	}
}
