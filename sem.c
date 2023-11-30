#include <stdio.h>
#include "functions.h"
#include <string.h>

void Go_through(struct Node* root, struct TRP* table, struct TRP* global);
void Type_of_node (struct Node* root, TRP* table, struct TRP* global);

void PerformSemantics(Node** AST, TRP** global)
{
	Node* tree = *AST;
	*global = TableInit(*global);
	Go_through(tree, *global, *global);
	return;
}

void Go_through(struct Node* root, struct TRP* table, struct TRP* global)
{
	//printf("Address of var: %p %p\n", &(*table), &(*global));
	if (root != NULL){
		if (strcmp(root->content, "body") == 0){

			TRP* local = NULL;
			local = TableInit(local);
			//root->TRP = local;
			table->next = local;
			printf("--- zakladam lokalnu tabulku ---\n");
			if (root->children[0] != NULL){
				Type_of_node(root->children[0], local, global);
				Go_through (root->children[0], local, global);
			}
			return;
		} else {
			for (int i = 0; i < root->numChildren; i++){
				printf("%d: ", i);
				Type_of_node(root->children[i], table, global);
				Go_through(root->children[i], table, global);
			}
			// TRP* current = table;

			// while (current != NULL) {
			// 	if (current->next != NULL){
			// 		current = current->next;
			// 	} else {
			// 		//TableRemoveTable(current);
			// 		return;
			// 	}
			// }
		}
	}
}

void Type_of_node (struct Node* root, TRP* table, struct TRP* global)
{

	if (strcmp(root->content, "write") == 0){
		return;
	} else if (strcmp(root->content, "let") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "let declaration";
		}
	} else if (strcmp(root->content, "var") == 0){ //chyba pre func foo pretypovanie
		if (root->children[0] != NULL){
			root->children[0]->type = "var declaration";
		}
	} else if (strcmp(root->type, "let declaration") == 0 || strcmp(root->type, "var declaration") == 0){

		if (TableFindItem(table, root->content) == NULL){

			bool* content_in_variable = malloc(sizeof(bool));
			*content_in_variable = false;
			wordStr* type_of_variable = NULL;

			for (int i = 1; i < root->parent->numChildren; i++){
				if (i == 1){
					type_of_variable = malloc(sizeof(wordStr));
					type_of_variable->type = root->parent->children[i]->content;
					if (strcmp(root->type, "let declaration") == 0){
						type_of_variable->content = "let declaration";
					}else {
						type_of_variable->content = "var declaration";
					}

				} else {
					if (strcmp(root->parent->children[i]->content, "readInt") == 0){
						if (strcmp(root->parent->children[1]->content, "Int?") != 0){
							printf("EROOOR nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->content, "readDouble") == 0){
						if (strcmp(root->parent->children[1]->content, "Double") != 0){
							printf("EROOOR nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->content, "readString") == 0){
						if (strcmp(root->parent->children[1]->content, "String") != 0){
							printf("EROOOR nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->type, "string") != 0){
						printf("EROOOR nie je kompatibilne\n");
					} else if (strcmp(root->parent->children[i]->type, "double") != 0){
						printf("EROOOR nie je kompatibilne\n");
					} else if (strcmp(root->parent->children[i]->type, "integer") != 0){
						printf("EROOOR nie je kompatibilne\n");
					}
				}
			}
			TableAddItem(&(*table), root->content, type_of_variable, content_in_variable);
		} else {
			if (root->parent->parent != NULL){

				if (strcmp(root->parent->parent->content, "if") == 0){
					printf("VSETKO V PORAAAAAAAAAAAAAADKU\n");
				} else {printf("EROOOOOR redeklaracia\n");}
			}
		}
	} else if (strcmp(root->content, "Int2Double") == 0){
		// zatial nefunguje v AST
	} else if (strcmp(root->content, "Double2Int") == 0){
		// zatial nefunguje v AST
	} else if (strcmp(root->content, "length") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii length");
			}
		}
	} else if (strcmp(root->content, "substring") == 0){
		for (int i = 0; i < root->numChildren; i++){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii substring");
			} else if (strcmp(root->children[1]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii substring");
			} else if (strcmp(root->children[2]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii substring");
			}
		}
	} else if (strcmp(root->content, "ord") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii ord");
			}
		}
	} else if (strcmp(root->content, "chr") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				printf("EROOOOOOOR vo funkcii ord");
			}
		}
	}

	printf("%s %s\n", root->content, root->type);
}
