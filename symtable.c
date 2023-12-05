#include "symtable.h"

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

TRP* TableInit(TRP* table){
	table = malloc(sizeof(TRP));
	if (table == NULL){return NULL;}
	for (int i = 0; i < hashTableSize; i++){
    	table->items[i] = NULL;
  	}
	table->next = NULL;
	return table;
}

TRPitem* TableFindItem(TRP* table, char *key)
{
	int hash = HashFunction(key);
	TRPitem* current = table->items[hash];
	
	while (current != NULL) {
		if (strcmp(current->key, key) == 0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

void TableAddItem(TRP* table, char* key, wordStr* type, bool* content)
{
	int hash = HashFunction(key);
  	TRPitem *item = TableFindItem(table, key);
	if (item != NULL){ //already in table
			
		if (type != NULL){
			if (item->type == NULL){ // insert first
				printf ("tu niesom\n");
				item->type = type;
				item->type->next = NULL;
			} else { // insert after
				wordStr* active = item->type;
				while (active != NULL){
					if (strcmp(active->content, type->content) == 0){
						printf ("ERROR - same param in function\n");
					}
					if (active->next == NULL){
						active->next = type;
						return;
					}
					active = active->next;
				}
			}
		}
		return;
	} else { //not in the table
		if (table->items[hash] == NULL){
			table->items[hash] = malloc(sizeof(TRPitem));
			if (table->items[hash] == NULL){return;}
			table->items[hash]->key = key;
			table->items[hash]->type = type;
			table->items[hash]->content = content;
			table->items[hash]->next = NULL;
		} else {
			TRPitem *last_item = table->items[hash];

			while (last_item != NULL){
				if (last_item->next != NULL){
					last_item = last_item->next;
				} else {
					last_item->key = key;
					last_item->type = type;
					last_item->content = content;
					last_item->next = NULL;
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
void Print_tables(Node* root){
	if (root != NULL){
		if (strcmp(root->content, "root") == 0){
			for (int i = 0; i < 255; i++){
				if (root->TRP->items[i] != NULL){
					wordStr* current = root->TRP->items[i]->type;
					printf ("\n- %s - ", root->TRP->items[i]->key);
					while (current != NULL){
						printf ("- %s %s -", current->content, current->type);
						current = current->next;
					}
				}
			}
			printf("\n-------------------------------");
		}
		if (strcmp(root->content, "body") == 0){
			for (int i = 0; i < 255; i++){
				if (root->TRP->items[i] != NULL){
					wordStr* current = root->TRP->items[i]->type;
					printf ("\n- %s - ", root->TRP->items[i]->key);
					while (current != NULL){
						printf ("- %s %s -", current->content, current->type);
						current = current->next;
					}
				}
			}
			printf("\n------------------------\n");
		}
		for (int i = 0; i < root->numChildren; i++){
			Print_tables (root->children[i]);
		}
	}
}