#include <stdio.h>
#include "functions.h"
#include <string.h>

void Go_through(struct Node* root, struct TRP* table);
void type_of_node (struct Node* root, TRP* table);

void PerformSemantics(Node** AST, TRP* global)
{
	Node* tree = *AST;
	global = NULL;
	global = malloc(sizeof(TRP));
	if (global == NULL){return;}
	Go_through(tree, global);
	return;
}

void Go_through(struct Node* root, struct TRP* table)
{
	//type_of_node();
	if (root != NULL){
		if (strcmp(root->content, "body") == 0){

			TRP* local = NULL;
			local = malloc (sizeof(TRP));
			if (local == NULL){return;}

			Go_through (root->children[0], local);
		} else {
			for (int i = 0; i < root->numChildren; i++){
				printf("%d: ", i);
				printf("%s %s\n", root->children[i]->content, root->children[i]->type);
				Go_through(root->children[i], table);
			}
		TRP* current = table;

		while (current != NULL) {
			if (current->next != NULL){
				current = current->next;
			} else {
				free(current);
				return;
			}
		}
		}
	}
}

void type_of_node (struct Node* root, TRP* table)
{

}
