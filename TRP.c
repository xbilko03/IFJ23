#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdlib.h>

void TableAddItem(TRP* table, TRPitem* item)
{
	TRPitem* currentItem;
	long index = HashFunction(item->content) % hashTableSize;
	if (table->items[index] == NULL)
		table->items[index] = item;
	else
	{
		currentItem = table->items[index];
		while (currentItem->next != NULL)
			currentItem = currentItem->next;
		currentItem->next = item;
	}
}
TRPitem* TableCreateItem(char* content)
{
	TRPitem* cItem = malloc(sizeof(TRPitem*));
	cItem->next = NULL;
	cItem->content = malloc(sizeof(char) * (1 + strlen(content)));
	strcpy(cItem->content, content);

	return cItem;
}
TRPitem* TableFindItem(TRP* table,char* contentSearch)
{
	long index = HashFunction(contentSearch) % hashTableSize;
	TRPitem* cItem;
	
	if (table == NULL)
		return NULL;

	cItem = table->items[index];
		
	do
	{
		if (strcmp(cItem->content, contentSearch) == 0)
			return cItem;
	} while (cItem != NULL);

	return NULL;
}
void TableRemoveItem(TRP* table, wordStr* item)
{

}
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
