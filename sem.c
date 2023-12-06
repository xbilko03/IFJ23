/**
 * @name IFJ2023 Compiler
 * @file sem.c
 * @author Juraj Budai - xbudai02
 */

#include "sem.h"

void PerformSemantics(Node** AST, TRP** global)
{
	Node* tree = *AST;
	*global = TableInit(*global);
	(*AST)->TRP = *global;
	Go_through_function(tree, *global); 
	Go_through(tree, *global, *global, NULL);
	return;
}

void Go_through(Node* root, TRP* table, TRP* global, TRPitem* function) //function that will iterate through AST and check tokens
{
	if (root != NULL){
		if (strcmp(root->content, "body") == 0){ //making a local TRP to store variables
			table->next = root->TRP;
			TRP* local = root->TRP;
			if (strcmp(root->parent->content, "func") == 0){ // checking if functions is declared
				function = TableFindItem (global, root->parent->children[0]->content);
				bool is_there = check_for_return (root->parent, global, function);
				if (is_there == false){
					ExitProgram(6,"There is no return in the function\n");
				}
			}
			for (int i = 0; i < root->numChildren; i++){
				Type_of_node(root->children[i], local, global, function);
				Go_through (root->children[i], local, global, function);
			}
			function = NULL;
			table->next = NULL; //end of the local TRP
			return;
		} else {
			for (int i = 0; i < root->numChildren; i++){
				Type_of_node(root->children[i], table, global, function);
				Go_through(root->children[i], table, global, function);
			}
		}
	}
}

void Go_through_function(Node* root, TRP* global) // help function to find user functions
{
	if (root != NULL){
		if (strcmp(root->content, "func") == 0){
			root->children[0]->type = "func identifier";
				TRPitem* function = TableFindItem (global, root->children[0]->content);
				if (function == NULL){ //if there is no function in TRP add or print error when redeclared
					Add_function_to_symtable (root, global);
				} else{
					ExitProgram(9,"Redeclaration of function\n");
				}
		} else if (strcmp(root->content, "body") == 0){ //assign the TRP in the AST where the body is

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
	for (int i = 0; i < root->children[0]->numChildren; i++){ //store params for function
		wordStr* type_of_var = malloc(sizeof(wordStr));
		type_of_var->content = root->children[0]->children[i]->children[0]->content;
		type_of_var->type = root->children[0]->children[i]->children[0]->children[0]->content;
		type_of_var->next = NULL;
		TableAddItem (&(*global), root->children[0]->content, type_of_var, false);
	}
}

char* check_for_type (Node* root, char* type, TRP* global, char* identifier_or_value, TRPitem* function){ //checking type or return and compatibility of identifiers
	char* looking_for_type = NULL;
	if (identifier_or_value == NULL){
		identifier_or_value = malloc(sizeof(identifier_or_value)*11);
	}
	if (strcmp(root->type, "operator") == 0 || strcmp(root->type, "compare") == 0){ // checking if operand and operator can go together
		looking_for_type = check_for_type (root->children[0], type, global, identifier_or_value, function);
	
		char * help = malloc(sizeof(identifier_or_value)*11);
		strcpy(help, identifier_or_value);
		looking_for_type = check_for_type (root->children[1], type, global, identifier_or_value, function);
		
		if (strcmp(root->type, "compare") == 0){
			if (type == NULL){
				return "bool";
			}
			ExitProgram(7,"Cannot assign a bool to variable\n");
		}
	} else if (strcmp(root->type, "integer") == 0){
		looking_for_type = "Int";
		strcpy(identifier_or_value, "value");
	} else if (strcmp(root->type, "double") == 0){
		looking_for_type = "Double";
		strcpy(identifier_or_value, "value");
	} else if (strcmp(root->type, "string") == 0){
		looking_for_type = "String";
		strcpy(identifier_or_value, "value");
	} else if (strcmp(root->content, "nil") == 0){ 
		if (strcmp(type, "Int?") != 0 || strcmp(type, "String?") != 0 || strcmp(type, "Double?") != 0){
			ExitProgram(7,"Cannot assign nil to variable with type?\n");
		}
	} else if (strcmp(root->type, "identifier") == 0){
		if (type != NULL){
			if (strcmp(type, "void") == 0){
				ExitProgram(7,"Cannot assign void function to variable\n");
			}
		}
		if (function != NULL){
			wordStr* is_there = function->type;
	 		while (is_there != NULL){
				if (strcmp(root->content, is_there->content) == 0){
					looking_for_type = is_there->type;
					break;
				}
				is_there = is_there->next;
			}
		}
		if (looking_for_type == NULL){
			TRP* current = global;
			TRPitem* found = NULL;
			while (found == NULL && current != NULL){
				found = TableFindItem(current, root->content);
				current = current->next;
			}
			if (found != NULL){
				looking_for_type = found->type->type;
				strcpy(identifier_or_value, "identifier");
			} else {
				if (root->children != NULL){
					ExitProgram(3,"Undefined function\n");
				} else {
					ExitProgram(5,"Undefined variable\n");
				}
			}
		}
	} else if (strcmp(root->content, "readInt") == 0){
		looking_for_type = "Int";
	} else if (strcmp(root->content, "readDouble") == 0){
		looking_for_type = "Double";
	} else if (strcmp(root->content, "readString") == 0){
		looking_for_type = "String";
	} else if (strcmp(root->content, "write") == 0){
		ExitProgram(7,"Cannot assign void to variable\n");
	} else if (strcmp(root->content, "Int2Double") == 0){
		looking_for_type = "Double";
	} else if (strcmp(root->content, "Double2Int") == 0){
		looking_for_type = "Int";
	} else if (strcmp(root->content, "length") == 0){
		looking_for_type = "Int";
	} else if (strcmp(root->content, "substring") == 0){
		looking_for_type = "String";
	} else if (strcmp(root->content, "ord") == 0){
		looking_for_type = "Int";
	} else if (strcmp(root->content, "chr") == 0){
		looking_for_type = "String";
	} 
	if (looking_for_type != NULL && type != NULL){
		if (strcmp(looking_for_type, type) == 0
			||((strcmp(looking_for_type, "Int") == 0 || strcmp(looking_for_type, "Int?") == 0) && (strcmp(type, "Int") == 0 || strcmp(type, "Int?") == 0))
			||((strcmp(looking_for_type, "Double") == 0 || strcmp(looking_for_type, "Double?") == 0) && (strcmp(type, "Double") == 0 || strcmp(type, "Double?") == 0))
			||((strcmp(looking_for_type, "String") == 0 || strcmp(looking_for_type, "String?") == 0) && (strcmp(type, "String") == 0 || strcmp(type, "String?") == 0))){
			return type;
		} else if (strcmp(identifier_or_value, "value") == 0 && ((strcmp(looking_for_type, "Int") == 0) 
															|| (strcmp(looking_for_type, "Int?") == 0) 
															|| (strcmp(looking_for_type, "Double") == 0) 
															|| (strcmp(looking_for_type, "Double?") == 0))){
			return type;
		} else {
			ExitProgram(7,"Incompatible types\n");
		}
	} else if (looking_for_type != NULL && type == NULL){
		return looking_for_type;
	} 
	return "xx";
}

void Type_of_node (struct Node* root, TRP* table, struct TRP* global, TRPitem* function){ //checking if content in the node is used correctly
	if (strcmp(root->content, "let") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "let declaration";
		}
	} else if (strcmp(root->content, "var") == 0){
		if (root->children[0] != NULL){
			root->children[0]->type = "var declaration";
		}
	} else if (strcmp(root->type, "let declaration") == 0 || strcmp(root->type, "var declaration") == 0){ //adding variables to TRP
		TRP* current = global;
		TRPitem* found = NULL;
		while (found == NULL && current != NULL){ //looking if the variable is already in the table
			found = TableFindItem(current, root->content);
			if (found != NULL){break;}
			current = current->next;
		}
		if (found == NULL || (found != NULL && (current != table))){
			bool let_content = false;
			wordStr* type_of_variable = malloc(sizeof(wordStr));
			if (root->children != NULL){
				if(strcmp(root->children[0]->type, "identifier(type)") == 0){ // type of variable is in declaration
					if(root->parent->children[1] != NULL){ // also there is value for variable
						if (strcmp(root->children[0]->content, "Int") == 0 || strcmp(root->children[0]->content, "Int?") == 0){
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "String") == 0 || strcmp(root->children[0]->content, "String?") == 0){
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "Double") == 0 || strcmp(root->children[0]->content, "Double") == 0){
							type_of_variable->type = check_for_type (root->parent->children[1], root->children[0]->content, global, NULL, NULL);
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						}
						TableAddItem(&(*table), root->content, type_of_variable, &let_content); // if everything is ok, variable will be added to TRP
					} else { //only tzpe of variable is in the function
						
						if (strcmp(root->children[0]->content, "Int") == 0 || strcmp(root->children[0]->content, "Int?") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "String") == 0 || strcmp(root->children[0]->content, "String?") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						} else if (strcmp(root->children[0]->content, "Double") == 0 || strcmp(root->children[0]->content, "Double") == 0){
							type_of_variable->type = root->children[0]->content;
							type_of_variable->next = NULL;
							if (strcmp(root->type, "let declaration") == 0){
								type_of_variable->content = "let declaration";
								let_content = true;
							}
						}
						TableAddItem(&(*table), root->content, type_of_variable, &let_content);
					}
				}
			} else { // type of variable is not in the declaration, need to assign the type by value
				if(root->parent->children[1] != NULL){
					type_of_variable->type = check_for_type(root->parent->children[1], NULL, global, NULL, NULL);
					type_of_variable->next = NULL;
					if (strcmp(root->type, "let declaration") == 0){
						type_of_variable->content = "let declaration";
						let_content = true;
					}
					if (strcmp(type_of_variable->type, "xx")==0){
						ExitProgram(8,"Cannot deduce type of variable\n");
					}
					TableAddItem(&(*table), root->content, type_of_variable, &let_content);
				}
			} 
		} else { //if let declaration is after if
			if (root->parent->parent != NULL){
				if (strcmp(root->parent->parent->content, "if") == 0){
					return;
				}
			}
			ExitProgram(3,"Redefinition of variable\n");
		}
	} else if (strcmp(root->type, "identifier") == 0){
		TRP* current = global;
		if (function != NULL){
			while(current != NULL){
				TRPitem* found = TableFindItem(current, function->key); //checking if the identifier is parameter of function
				if (found != NULL){
					wordStr* is_there = found->type;
					while (is_there != NULL){
						if (strcmp(root->content, is_there->content) == 0){
							if (root->parent != NULL && strcmp(root->parent->content, "=") == 0){
								check_for_type (root->parent->children[1],is_there->type, global, NULL, NULL); //if identifier was found chceck the operations with it
							}
							return;
						}
						is_there = is_there->next;
					}
				}
				current = current->next;
			}
		}
			current = global; //checking if identifier is in the TRP
			while(current != NULL){
				TRPitem* found = TableFindItem(current, root->content);
			if (found != NULL){
				if (root->parent != NULL && strcmp(root->parent->content, "=") == 0){
					if (found->type->content != NULL && strcmp(found->type->content, "let declaration") == 0){
						ExitProgram(9,"Assigning again to let variable\n");
					}
					check_for_type (root->parent->children[1],found->type->type, global, NULL, function);
				}
				if (root->children != NULL){ //identfier is function
					int count_of_params = -1;
					wordStr* count = found->type;
					while (count != NULL){
						count_of_params += 1;
						if (strcmp(count->content, "Return") != 0){
							for(int i = 0; i < root->numChildren; i++){
								if (root->children[i]->children != NULL){ //check for usage of parameters in call of a function
									char* type_of_variable = check_for_type(root->children[i]->children[0], NULL, global, NULL, found);
									if (strcmp(type_of_variable, found->type->type) == 0 
										|| ((strcmp(type_of_variable, "Int") == 0) && (strcmp(found->type->type, "Int?") == 0))
										|| ((strcmp(type_of_variable, "Int?") == 0) && (strcmp(found->type->type, "Int") == 0))
										|| ((strcmp(type_of_variable, "String") == 0) && (strcmp(found->type->type, "String?") == 0))
										|| ((strcmp(type_of_variable, "String?") == 0) && (strcmp(found->type->type, "String") == 0))
										|| ((strcmp(type_of_variable, "Double") == 0) && (strcmp(found->type->type, "Double?") == 0))
										|| ((strcmp(type_of_variable, "Double?") == 0) && (strcmp(found->type->type, "Double") == 0))){
									} else {
										ExitProgram(4,"Incorrect type of parameters in a function\n");
									}
								} else {	//check for usage of parameters in call of a function
									char* type_of_variable = check_for_type(root->children[i], NULL, global, NULL, found);
									if (strcmp(type_of_variable, found->type->type) == 0 
										|| ((strcmp(type_of_variable, "Int") == 0) && (strcmp(found->type->type, "Int?") == 0))
										|| ((strcmp(type_of_variable, "Int?") == 0) && (strcmp(found->type->type, "Int") == 0))
										|| ((strcmp(type_of_variable, "String") == 0) && (strcmp(found->type->type, "String?") == 0))
										|| ((strcmp(type_of_variable, "String?") == 0) && (strcmp(found->type->type, "String") == 0))
										|| ((strcmp(type_of_variable, "Double") == 0) && (strcmp(found->type->type, "Double?") == 0))
										|| ((strcmp(type_of_variable, "Double?") == 0) && (strcmp(found->type->type, "Double") == 0))){
									} else {
										ExitProgram(4,"Incorrect type of parameters in a function\n");
									}
								}
							}
						}
						count = count->next;
					}
					if (count_of_params != root->numChildren){
						ExitProgram(4,"Incorrect number of parameters in a function\n");
					}
				}
				return;
			}
			current = current->next;
			}
			ExitProgram(5,"Undefined variable\n");
	} else if (strcmp(root->content, "if") == 0 || strcmp(root->content, "while") == 0){
		if (strcmp(root->children[0]->content, "let") == 0){ //let declaration can be after if
			return;
		} else if (strcmp(root->children[0]->type, "compare") == 0){ //expect let declaration there can be only bool type
			check_for_type(root->children[0], NULL, global, NULL, function);
		} else {
				ExitProgram(7,"Non-bool type in the if or while\n");
		}
	} else if (strcmp(root->content, "Int2Double") == 0 || strcmp(root->content, "chr") == 0){ //checking if the parameter in the function is a wrong type
		if (root->children[0] != NULL){
			check_for_type(root->children[0], "Int", global, NULL, NULL);
		} else {
			ExitProgram(4,"Incorrect number of parameters in function\n");
		}
	} else if (strcmp(root->content, "Double2Int") == 0){ //checking if the parameter in the function is a wrong type
		if (root->children[0] != NULL){
			check_for_type(root->children[0], "Double", global, NULL, NULL);
		} else {
			ExitProgram(4,"Incorrect number of parameters in function\n");
		}
	} else if (strcmp(root->content, "length") == 0 || strcmp(root->content, "ord") == 0){ //checking if the parameter in the function is a wrong type
		if (root->children[0] != NULL){
			check_for_type(root->children[0], "String", global, NULL, NULL);
		} else {
			ExitProgram(4,"Incorrect number of parameters in function\n");
		}
	} else if (strcmp(root->content, "substring") == 0){ 
		if (root->children != NULL){
			if(root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL){
				char* help1 = check_for_type(root->children[0]->children[0], NULL, global, NULL, NULL);
				if (strcmp(help1, "String") != 0){
					ExitProgram(4,"Incorrect type of parameter in a function\n");
				}
				char* help2 = check_for_type(root->children[1]->children[0], NULL, global, NULL, NULL);
				if (strcmp(help2, "Int") != 0){
					ExitProgram(4,"Incorrect type of parameter in a function\n");
				}
				char* help3 = check_for_type(root->children[2]->children[0], NULL, global, NULL, NULL);
				if (strcmp(help3, "Int") != 0){
					ExitProgram(4,"Incorrect type of parameter in a function\n");
				}
			} else {
				ExitProgram(4,"Incorrect number of parameters in function\n");
			}
		}
	} else if (strcmp(root->content, "return") == 0){ 
		if (table == global && function == NULL){ 
			ExitProgram(9,"Return where not expected\n");
		}
		if (function != NULL){
			if (strcmp(function->type->type, "void") != 0){
				if (root->children == NULL){
					ExitProgram(6,"Missing expression in return\n");
				return;
				}
				char* type_of_variable = check_for_type(root->children[0], NULL, global, NULL, function); //checking if the type after return is correct
				if (strcmp(type_of_variable, function->type->type) == 0 
					|| ((strcmp(type_of_variable, "Int") == 0) && (strcmp(function->type->type, "Int?") == 0))
					|| ((strcmp(type_of_variable, "Int?") == 0) && (strcmp(function->type->type, "Int") == 0))
					|| ((strcmp(type_of_variable, "String") == 0) && (strcmp(function->type->type, "String?") == 0))
					|| ((strcmp(type_of_variable, "String?") == 0) && (strcmp(function->type->type, "String") == 0))
					|| ((strcmp(type_of_variable, "Double") == 0) && (strcmp(function->type->type, "Double?") == 0))
					|| ((strcmp(type_of_variable, "Double?") == 0) && (strcmp(function->type->type, "Double") == 0))){
				} else {
					ExitProgram(4,"Incorrect type of expression in return\n");
				}
			} else {
				if (root->children != NULL){ //if there is something after return in void function
					ExitProgram(6,"Void function has no return\n");
				}
			}
		}
	} 
}

bool check_for_return (Node* root, TRP* global, TRPitem* function){ //help function to find "return" in the body of function
	if (function != NULL){
		if (strcmp(root->content, "return") == 0){
			return true;
		}
		if (strcmp(function->type->type, "void") == 0){
			return true;
		} else {
			for (int i = 0; i < root->numChildren; i++){
				if (strcmp(root->content, "return") == 0){
					return true;
				}
				bool is_there = check_for_return(root->children[i], global, function);
				if (is_there){
					return true;
				}
			}
			return false;
		}
	}
	return false;
}