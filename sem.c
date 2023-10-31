#include <stdio.h>
#include "functions.h"

void doPreorder(struct node* root);

void PerformSemantics(node** AST)
{
	node* tree = *AST;

    doPreorder(tree);

    printf("%s\n", tree->content);
    printf("%s\n", tree->type);
	return;
}

void doPreorder(struct node* root)
{
    if (root != NULL)
    {
        printf("NODE=%s\n", root->content);
        doPreorder(root->left);
        doPreorder(root->right);
    }
}