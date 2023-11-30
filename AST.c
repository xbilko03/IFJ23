#include "AST.h"

void DeleteWordList(wordListStr* wordList)
{
	wordStr* currentWord = wordList->first;
	wordStr* nextWord;

	while(currentWord != NULL)
	{
		free(currentWord->type);
		free(currentWord->content);
		nextWord = currentWord->next;
		free(currentWord);
		currentWord = nextWord;
	}
	free(wordList);
}

Node* Node_insert(Node** parentAST, char* content, TRPitem* TRP, char* type)
{
	Node* childrenAST = (Node*)checked_malloc(sizeof(Node));
	childrenAST->type = type;
	childrenAST->children = NULL;
	childrenAST->numChildren = 0;
	childrenAST->content = content;
	childrenAST->TRP = TRP;

	if(*parentAST == NULL)
	{
		*parentAST = childrenAST;
		return childrenAST;
	}
	
	childrenAST->parent = *parentAST;
	(*parentAST)->numChildren++;
	(*parentAST)->children = (Node**)checked_realloc((*parentAST)->children, sizeof(Node*) * ((*parentAST)->numChildren));
	(*parentAST)->children[(*parentAST)->numChildren-1] = childrenAST;
	return childrenAST;
}

void* checked_malloc(size_t size)
{
	void* ptr = malloc(size);
	if(ptr == NULL)
	{
		ExitProgram(99, "malloc error\n");
	}
	return ptr;
}

void* checked_realloc(void* ptr, size_t size)
{
	void* newPtr = realloc(ptr, size);
	if(newPtr == NULL)
	{
		ExitProgram(99, "realloc error\n");
	}
	return newPtr;
}

//DEBUG
void print_AST(Node* tree, bool* flag, int depth, bool isLast)
{
    if (tree == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    if (isLast) {
        printf("+---");
        flag[depth] = NULL;
    } else {
        printf("+---");
    }
	printf(" %s\n", tree->content);


    int it = 0;
    for (int i = 0; i < tree->numChildren; i++, it++) {
        print_AST(tree->children[i], flag, depth + 1, it == ((tree->children[i]->numChildren)-1));

        flag[depth] = true;
    }
}
//END DEBUG