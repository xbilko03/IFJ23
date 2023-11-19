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
	for (int i = 0; i < hashTableSize; i++){
    	table->items[i] = NULL;
  	}
}

TRPitem* TableFindItem(TRP* table, char *key)
{
	int hash = HashFunction(key);
	TRPitem* current = table->items[hash];

	while (current != NULL) {
		if (current->type == key){
		return current;
		}
		current = current->next;
	}
	return NULL;
}

void TableAddItem(TRP* table, char* key, wordStr* type, char* content)
{
	int hash = get_hash(key);
  	TRPitem *item = ht_search(table, key);

	if (item != NULL){
		item->type = type;
		return;
	} else {
	
		TRPitem *new_item = malloc(sizeof(TRPitem));
		if (new_item == NULL){return;}

		new_item->content = content;
		new_item->key = key; 
		new_item->type = type;
		new_item->next = NULL;

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
void TableRemoveTable(TRP* table, wordStr* item)
{
	for (int i = 0; i < hashTableSize; i++){
		TRPitem *current = table->items[i];
		
		while (current != NULL){
		TRPitem *next = current->next;
		free(current);
		current = next;
		}
		table->items[i] = NULL;
	}
}
