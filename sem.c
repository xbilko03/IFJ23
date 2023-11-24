#include <stdio.h>
#include "functions.h"
#include <string.h>

void Go_through(struct Node* root, struct TRP* table, struct TRP* global);
void Type_of_node (struct Node* root, TRP* table, struct TRP* global);

void PerformSemantics(Node** AST, TRP* global)
{
	Node* tree = *AST;
	global = TableInit(global);
	Go_through(tree, global, global);
	return;
}

void Go_through(struct Node* root, struct TRP* table, struct TRP* global)
{
	if (root != NULL){
		
		if (strcmp(root->content, "body") == 0){

			TRP* local = NULL;
			local = TableInit(local);
			
			if(local == NULL){
				printf("heloooooooooooooo");
			}
			table->next = local;
			printf("--- zakladam lokalnu tabulku ---\n");
			if (root->children[0] != NULL){
				Go_through (root->children[0], local, global);
			}
			return;
		} else {
			for (int i = 0; i < root->numChildren; i++){
				printf("%d: ", i);
				Type_of_node(root->children[i], table, global);
				Go_through(root->children[i], table, global);
			}
			TRP* current = table;

			while (current != NULL) {
				if (current->next != NULL){
					current = current->next;
				} else {
					//TableRemoveTable(current);
					return;
				}
			}
		}
	}
}

void Type_of_node (struct Node* root, TRP* table, struct TRP* global)
{
	printf("%s %s\n", root->content, root->type);
}
