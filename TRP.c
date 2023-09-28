#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdlib.h>

void TableAddItem(TRP* table, wordStr* item)
{

}
wordStr* TableFindItem()
{

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
