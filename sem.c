#include "sem.h"

void PerformSemantics(Node** AST, TRP** global)
{
	Node* tree = *AST;
	*global = TableInit(*global);
	(*AST)->TRP = *global;
	Go_through(tree, *global, *global, NULL);
	return;
}

void Go_through(struct Node* root, struct TRP* table, struct TRP* global, TRPitem* function)
{
	if (root != NULL){
		if (strcmp(root->content, "body") == 0){

			TRP* local = NULL;
			local = TableInit(local);
			root->TRP = local; 
			table->next = local;
			if (strcmp(root->parent->content, "func") == 0){
				function = TableFindItem (global, root->parent->children[0]->content);
			}
			for (int i = 0; i < root->numChildren; i++){
				Type_of_node(root->children[i], local, global, function);
				Go_through (root->children[i], local, global, function);
			}
			function = NULL;
			table->next = NULL;
			return;
		} else {
			for (int i = 0; i < root->numChildren; i++){
				Type_of_node(root->children[i], table, global, function);
				Go_through(root->children[i], table, global, function);
			}
		}
	}
}

void Type_of_node (struct Node* root, TRP* table, struct TRP* global, TRPitem* function)
{

	if (strcmp(root->content, "let") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "let declaration";
		}
	} else if (strcmp(root->content, "var") == 0){
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
					if(strcmp(root->parent->children[i]->content, "Int2Double") == 0){
						type_of_variable->type = "Double";
					} else if (strcmp(root->parent->children[i]->content, "Double2Int") == 0){
						type_of_variable->type = "Int?";
					} else if (strcmp(root->parent->children[i]->type, "identifier") == 0){
						TRP* current = global;
						while(current != NULL){
							TRPitem* found = TableFindItem(current, root->parent->children[i]->content);
							if (found != NULL){
								return;
							}
							current = current->next;
						}
						ExitProgram(5, "Nedefinovana premenna\n");

					} else {
						type_of_variable->type = root->parent->children[i]->content;
					}
					if (strcmp(root->type, "let declaration") == 0){
						type_of_variable->content = "let declaration";
					}else {
						type_of_variable->content = "var declaration";
					}

				} else {
					if (strcmp(root->parent->children[i]->content, "readInt") == 0){
						if (strcmp(root->parent->children[1]->content, "Int?") != 0){
							ExitProgram(7,"Nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->content, "readDouble") == 0){
						if (strcmp(root->parent->children[1]->content, "Double") != 0){
							ExitProgram(7,"Nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->content, "readString") == 0){
						if (strcmp(root->parent->children[1]->content, "String") != 0){
							ExitProgram(7,"Nie je kompatibilne\n");
							return;
						} else {*content_in_variable = true;}
					} else if (strcmp(root->parent->children[i]->type, "string") == 0){
						if (strcmp(type_of_variable->type, "String") != 0){
							ExitProgram(7,"Nie je kompatibilne\n");
						}
					} else if (strcmp(root->parent->children[i]->type, "double") == 0){
						if (strcmp(type_of_variable->type, "String") == 0){
							ExitProgram(7,"Nie je kompatibilne\n");
						}
					} else if (strcmp(root->parent->children[i]->type, "integer") == 0){
						if (strcmp(type_of_variable->type, "String") == 0){
							ExitProgram(7,"Nie je kompatibilne\n");
						}
					}
				}
			}
			TableAddItem(&(*table), root->content, type_of_variable, content_in_variable);
		} else {
			if (root->parent->parent != NULL){

				if (strcmp(root->parent->parent->content, "if") != 0){
					ExitProgram(3,"Redefinicia premennej\n");
				}
			}
		}
	} else if (strcmp(root->content, "Int2Double") == 0){
		// zatial nefunguje v AST
	} else if (strcmp(root->content, "Double2Int") == 0){
		// zatial nefunguje v AST
	} else if (strcmp(root->content, "length") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			}
		}
	} else if (strcmp(root->content, "substring") == 0){
		for (int i = 0; i < root->numChildren; i++){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			} else if (strcmp(root->children[1]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			} else if (strcmp(root->children[2]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			}
		}
	} else if (strcmp(root->content, "ord") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "string") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			}
		}
	} else if (strcmp(root->content, "chr") == 0){
		if (root->children[0] != NULL){
			if (strcmp(root->children[0]->type, "integer") != 0 && strcmp(root->children[0]->type, "identifier") != 0){
				ExitProgram(4,"Zly typ vo funkcii\n");
			}
		}
	} else if (strcmp(root->content, "func") == 0){
		if (root->children[0] != NULL){
			TRPitem* found = TableFindItem(table, root->children[0]->content);
			if (found != NULL){
				ExitProgram(3, "Redefincia funkcie\n");
			}
		}
		wordStr* type_of_variable = NULL;
		type_of_variable = malloc(sizeof(wordStr));
		if (root->children[0]->content != NULL){
			root->children[0]->type = "func identfier";
			if (root->children[1]->content != NULL){
				root->children[1]->type = "return type";
				type_of_variable->content = "Return";
				type_of_variable->type = root->children[1]->content;
				TableAddItem(&(*table), root->children[0]->content, type_of_variable, false);
			} else {
				type_of_variable->content = "Return";
				type_of_variable->type = "Void";
				TableAddItem(&(*table), root->children[0]->content, type_of_variable, false);
		}
		}
	} else if (strcmp(root->type, "paramid") == 0){
		if (root->children[0]->content != NULL){
			if (root->parent->parent->content != NULL){
				TRPitem* found = TableFindItem(table, root->parent->parent->content);
				if (found != NULL){
					wordStr* type_of_variable = NULL;
					type_of_variable = malloc(sizeof(wordStr));
					type_of_variable->content = root->content;
					type_of_variable->type = root->children[0]->content;
					if (found->type == NULL){
						found->type = type_of_variable;
					} else {
						wordStr* current = found->type;
						while (current->next != NULL){
							current = found->type->next;
						}
						current->next = type_of_variable;
					}
				}
			}
		}
	} else if (strcmp(root->type, "identifier") == 0){
		TRP* current = global;
		if (function != NULL){
			while(current != NULL){
				TRPitem* found = TableFindItem(current, function->key);
				if (found != NULL){
					wordStr* is_there = found->type;
					while (is_there != NULL){
						if (strcmp(root->content, is_there->content) == 0){
							return;
						}
						is_there = is_there->next;
					}
				}
				current = current->next;
			}
		}
			current = global;
			while(current != NULL){
				TRPitem* found = TableFindItem(current, root->content);
			if (found != NULL){
				return;
			}
			current = current->next;
			}
			ExitProgram(5,"Nedefinovana premenna\n");
	} else if (strcmp(root->content, "return") == 0){
		// if (table == global && function == NULL){
		// 	printf ("--- ERROR --- return v global");
		// } else {
		// 	if (root->children[0] != NULL){
		// 		TRP* current = global;
		// 		TRPitem* found = NULL;
		// 		if (function != NULL){
		// 			while(current != NULL){
		// 				found = TableFindItem(current, function->key);
		// 				if (found != NULL){
		// 					break;
		// 				}
		// 				current = current->next;
		// 			}
		// 		}
		// 		if (strcmp(root->children[0]->type, "operator" ) == 0){
		// 			for (int i = 0; i < 2; i++){
		// 				if (strcmp(root->children[0]->children[i]->type, "identifier") == 0){
		// 					check_for_expresions (root->children[0]->children[i]->content, table, global, found);
		// 				} else {
		// 					if (strcmp(root->children[0]->children[i]->type, "integer") == 0){
		// 						if (strcmp(function->type->type, "Int") != 0){
		// 							printf ("--- ERROR --- zly return type");
		// 						}
		// 					} else if (strcmp(root->children[0]->children[i]->type, "double") == 0){
		// 						if (strcmp(function->type->type, "Double") != 0){
		// 							printf ("--- ERROR --- zly return type");
		// 						}
		// 					} else if (strcmp(root->children[0]->children[i]->type, "string") == 0){
		// 						if (strcmp(function->type->type, "String") != 0){
		// 							printf ("--- ERROR --- zly return type");
		// 						}
		// 					}
		// 				}
		// 			}
		// 		} else {
		// 			if (strcmp(root->children[0]->type, "identifier") == 0){
		// 					check_for_expresions (root->children[0]->content, table, global, found);
		// 			} else {
		// 				if (strcmp(root->children[0]->type, "integer") == 0){
		// 					if (strcmp(function->type->type, "Int") != 0){
		// 						printf ("--- ERROR --- zly return type");
		// 					}
		// 				} else if (strcmp(root->children[0]->type, "double") == 0){
		// 					if (strcmp(function->type->type, "Double") != 0){
		// 						printf ("--- ERROR --- zly return type");
		// 					}
		// 				} else if (strcmp(root->children[0]->type, "string") == 0){
		// 					if (strcmp(function->type->type, "String") != 0){
		// 						printf ("--- ERROR --- zly return type");
		// 					}
		// 				}
		// 			}
		// 		}
		// 	} else {
		// 		if (strcmp(function->type->type, "void") != 0){
		// 			printf ("--- ERROR --- zly return type");
		// 		}
		// 	}
		// }
	} else if (strcmp(root->type, "operator") == 0){
		// if (root->children[0] != NULL && root->children[1] != NULL){
		// 	if (strcmp(root->children[0]->type, "identifier") != 0){

		// 	} else {

		// 	}

		// 	if (strcmp(root->children[1]->type, "identifier") != 0){

		// 	} else {

		// 	}
		// }
	}
}
// void check_for_expresions (char* key, TRP* table, struct TRP* global, TRPitem* function){
// 	if (function != NULL){
// 		wordStr* is_there = function->type;
// 		while (is_there != NULL){
// 			if (strcmp(key, is_there->content) == 0){
// 				printf ("---- %s -- %s ----\n", is_there->type, function->type->type);
// 				if (strcmp(is_there->type, function->type->type) == 0 ){
// 					return;
// 				} else {
// 					printf ("--- ERROR --- zly return type");
// 					return;
// 				}
// 			}
// 			is_there = is_there->next;
// 		}
// 	}
// 	TRP* current = global;
// 	while(current != NULL){
// 		TRPitem* found = TableFindItem(current, key);
// 		if (found != NULL){
// 			if (found->content != NULL){
// 				printf (" --- %s ---", found->content);
// 			}
// 			printf ("------ SEM sa este dostanem ----\n");
// 			if (strcmp(found->type->type, function->type->type) == 0){
// 				return;
// 			}  printf ("------ 1----\n");
// 			if ((strcmp(found->type->type, "Int?") == 0) && (strcmp(function->type->type, "Int") == 0)){
// 				return;
// 			} 
// 			else {
// 				printf ("--- ERROR --- zly return type");
// 				return;
// 			}
// 		}
// 		current = current->next;
// 	}
// }