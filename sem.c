#include "sem.h"

void PerformSemantics(Node** AST, TRP** global)
{
	Node* tree = *AST;
	*global = TableInit(*global);
	(*AST)->TRP = *global;
	Go_through_function(tree, *global);
	Go_through(tree, *global, *global, NULL);
	Print_tables(tree);
	return;
}

void Go_through(Node* root, TRP* table, TRP* global, TRPitem* function)
{
	if (root != NULL){
		if (strcmp(root->content, "body") == 0){
			table->next = root->TRP;
			TRP* local = root->TRP;
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

void Go_through_function(Node* root, TRP* global)
{
	if (root != NULL){
		if (strcmp(root->content, "func") == 0){
			root->children[0]->type = "func identifier";
				TRPitem* function = TableFindItem (global, root->children[0]->content);
				if (function == NULL){
					Add_function_to_symtable (root, global);
				} else{
					//error - redeklaracia funkcie
				}
		} else if (strcmp(root->content, "body") == 0){

			TRP* local = NULL;
			local = TableInit(local);
			root->TRP = local;
		}

		for (int i = 0; i < root->numChildren; i++){
			Go_through_function (root->children[i], global);
		}
	}
}

void Add_function_to_symtable (Node* root, TRP* global){
	if (strcmp(root->children[1]->content, "body") != 0){ //function has a return type
		wordStr* type_of_variable = malloc(sizeof(wordStr));
		type_of_variable->content = "Return";
		type_of_variable->type = root->children[1]->content;
		type_of_variable->next = NULL;
		TableAddItem (&(*global), root->children[0]->content, type_of_variable, false);
	}
	for (int i = 0; i < root->children[0]->numChildren; i++){
		wordStr* type_of_var = malloc(sizeof(wordStr));
		type_of_var->content = root->children[0]->children[i]->children[0]->content;
		type_of_var->type = root->children[0]->children[i]->children[0]->children[0]->content;
		type_of_var->next = NULL;
		TableAddItem (&(*global), root->children[0]->content, type_of_var, false);
	}
}

char* check_for_type (Node* root, char* type, TRP* global, char* identifier_or_value){
	char* looking_for_type = NULL;
	if (strcmp(root->type, "operator") == 0 || strcmp(root->type, "compare") == 0){ 
		looking_for_type = malloc(sizeof(char)*6);
		char* second_operand = malloc(sizeof(char)*6);
		looking_for_type = check_for_type (root->children[0], type, global, identifier_or_value);
		second_operand = check_for_type (root->children[1], type, global, identifier_or_value);
		if (identifier_or_value != NULL){
			printf ("---hello - %s ---\n", identifier_or_value);
		}
		if (strcmp(looking_for_type, second_operand) != 0){
			//error 
		}
	} else if (strcmp(root->type, "integer") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Int";
		identifier_or_value = malloc(sizeof(char)*6);
		identifier_or_value = "value";
	} else if (strcmp(root->type, "double") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Double";
		identifier_or_value = malloc(sizeof(char)*6);
		identifier_or_value = "value";
	} else if (strcmp(root->type, "string") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "String";
		identifier_or_value = malloc(sizeof(char)*6);
		identifier_or_value = "value";
	} else if (strcmp(root->content, "nil") == 0){ //neviem ci je root->content/type pri hodnote nil
		if (strcmp(type, "Int?") != 0 || strcmp(type, "String?") != 0 || strcmp(type, "Double?") != 0){
			//error - nil tam kde sa neocakava
		}
	} else if (strcmp(root->type, "identifier") == 0){
		TRP* current = global;
		TRPitem* found = NULL;
		while (found == NULL && current != NULL){
			found = TableFindItem(current, root->content);
			current = current->next;
		}
		if (found != NULL){
			looking_for_type = malloc(sizeof(looking_for_type)*6);
			looking_for_type = found->type->type;
			identifier_or_value = malloc(sizeof(char)*11);
			identifier_or_value = "identifier";
		} else {
			//error - nedefinovana premenna alebo funkcia 3 5
			// ak je za tym nieco je to funkcia
		}
	} else if (strcmp(root->content, "readInt") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Int";
	} else if (strcmp(root->content, "readDouble") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Double";
	} else if (strcmp(root->content, "readString") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "String";
	} else if (strcmp(root->content, "write") == 0){
		//error
	} else if (strcmp(root->content, "Int2Double") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Double";
	} else if (strcmp(root->content, "Double2Int") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Int";
	} else if (strcmp(root->content, "length") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Int";
	} else if (strcmp(root->content, "substring") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "String";
	} else if (strcmp(root->content, "ord") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "Int";
	} else if (strcmp(root->content, "chr") == 0){
		looking_for_type = malloc(sizeof(looking_for_type)*6);
		looking_for_type = "String";
	} else if (strcmp(root->content, "func") == 0){
		//error
	}
	if (looking_for_type != NULL && type != NULL){
		if (strcmp(looking_for_type, type) == 0
			||((strcmp(looking_for_type, "Int") == 0 || strcmp(looking_for_type, "Int?") == 0) && (strcmp(type, "Int") == 0 || strcmp(type, "Int?") == 0))
			||((strcmp(looking_for_type, "Double") == 0 || strcmp(looking_for_type, "Double?") == 0) && (strcmp(type, "Double") == 0 || strcmp(type, "Double?") == 0))
			||((strcmp(looking_for_type, "String") == 0 || strcmp(looking_for_type, "String?") == 0) && (strcmp(type, "String") == 0 || strcmp(type, "String?") == 0))){
			return type;
		} else{
			//error incompatible 7
		}
	} else if (looking_for_type != NULL && type == NULL){
		return looking_for_type;
	} 
	return "xx";
}

void Type_of_node (struct Node* root, TRP* table, struct TRP* global, TRPitem* function){
	if (strcmp(root->content, "let") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "let declaration";
		}
	} else if (strcmp(root->content, "var") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "var declaration";
		}
	} else if (strcmp(root->type, "let declaration") == 0 || strcmp(root->type, "var declaration") == 0){
		TRP* current = global;
		TRPitem* found = NULL;
		while (found == NULL && current != NULL){
			found = TableFindItem(current, root->content);
			current = current->next;
		}
		if (found == NULL){
			bool let_content = false;
			wordStr* type_of_variable = malloc(sizeof(wordStr));
			if (root->children != NULL){
				if(strcmp(root->children[0]->type, "identifier(type)") == 0){ // type of variable is in declaration
					if(root->parent->children[1] != NULL){ // also there is value for variable
						if (strcmp(root->children[0]->content, "Int") == 0 || strcmp(root->children[0]->content, "Int?") == 0){
							printf("%s %s\n", root->content, root->type);
							printf ("%s %s %s\n",root->parent->children[1]->content,root->parent->children[1]->type, root->children[0]->content);
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "String") == 0 || strcmp(root->children[0]->content, "String?") == 0){
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "Double") == 0 || strcmp(root->children[0]->content, "Double") == 0){
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						}
						TableAddItem(&(*table), root->content, type_of_variable, &let_content);
					} else {
						
						if (strcmp(root->children[0]->content, "Int") == 0 || strcmp(root->children[0]->content, "Int?") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "String") == 0 || strcmp(root->children[0]->content, "String?") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "Double") == 0 || strcmp(root->children[0]->content, "Double") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								let_content = true;
							}
						}
						TableAddItem(&(*table), root->content, type_of_variable, &let_content);
					}
				}
			} else { // type of variable is not in the declaration
				if(root->parent->children[1] != NULL){
					type_of_variable->type = check_for_type(root->parent->children[1], NULL, global, NULL);
					type_of_variable->next = NULL;
					if (strcmp(root->type, "let declaration") == 0){
						let_content = true;
					}
					if (strcmp(type_of_variable->type, "xx")==0){
						//error - nejde odvodit typ prememnej 8
					}
					TableAddItem(&(*table), root->content, type_of_variable, &let_content);
				}
			} 
		} else {
			//error - redifinicia premennej
		}
	}
	// printf("%s %s\n", root->content, root->type);
}