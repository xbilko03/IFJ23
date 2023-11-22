#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

wordStr* GetToken(wordStr* currentWord, bool ignoreNewLines, bool end_approved);
wordStr* GetFirstToken(wordListStr* wrdList, bool ignoreNewLines, bool end_approved);

wordStr* prog_con(wordStr* currentWord, Node* DemoAST);
wordStr* statement(wordStr* currentWord, Node* parent);
wordStr* opt(wordStr* currentWord, Node* parent);
wordStr* builtin(wordStr* currentWord, Node* parent);
wordStr* arg(wordStr* currentWord, Node* parent);
wordStr* args_more(wordStr* currentWord, Node* parent);
wordStr* types(wordStr* currentWord, Node* parent);
wordStr* params(wordStr* currentWord, Node* parent);
wordStr* params_more(wordStr* currentWord, Node* parent);
wordStr* type_spec(wordStr* currentWord, Node* parent);
wordStr* write_params(wordStr* currentWord, Node* parent);
wordStr* write_params_more(wordStr* currentWord, Node* parent);
wordStr* option(wordStr* currentWord, Node* parent);
wordStr* return_value(wordStr* currentWord, Node* parent);
wordStr* expression(wordStr* currentWord, Node* parent);
wordStr* expression_more(wordStr* currentWord, Node* parent);
wordStr* term(wordStr* currentWord, Node* parent);

//node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList, Node* DemoAST) {
	wordStr* currentWord = GetFirstToken(wrdList, true, true); //GET HEADER

	//Node* DemoAST = NULL;

	//EOF Prevention
	if (currentWord == NULL) {
		 
		return;
	}

	 
	// *********************TUTORIAL********************
	//node* cNode;
	//cNode = CreateNode(cToken->content, "testType");
	// 	

	currentWord = prog_con(currentWord, &(*DemoAST)); //START OF SYNTAX
	bool* flag = malloc(5000 * sizeof(bool));
	 

    free(flag);
	
	return;
}


	//########################################
	/*
	65-70. <statement>
	*/
wordStr* statement(wordStr* currentWord, Node* parent) {
	 

	//########################################
	/*
	65. <statement> -> <variant> ID <option> EOL <statement>
	*/
	// let || var
	if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		Node *let_or_var2 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		//ID
		if(strcmp(currentWord->type, "identifier") == 0) {
			Node_insert(&let_or_var2, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing identifier in variable definition in statement\n");

		//<option> SKIPFALSE
		currentWord = option(currentWord, &(*let_or_var2));

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after statement variant ID definition in statement\n");

		//<statement> continuing recursively
		return statement(currentWord, &(*parent));
	}

	//########################################
	/*
	66. <statement> -> <id_type> EOL <statement>
	*/
	else if ((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
		 
		/*
		29. <id_type> -> <builtin_extra> //Including write NOT CHECHED
		*/
		if((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			/*
			31. <builtin_extra> -> write(<write_params>)
			*/
			//write
			if(strcmp(currentWord->content, "write") == 0) {
				Node* write1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");
				//<write_params>
				 
				
				//SKIPTRUE
				currentWord = write_params(currentWord, &(*write1)); //should return ) if success
				//if((result = write_params(currentWord))) return result;
				 
				
				//)
				if (strcmp(currentWord->content, ")") == 0) {
					currentWord = GetToken(currentWord, false, false);
				}
				else ExitProgram(2, "Missing ) in write function calling\n");

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing EOL after id_type in statement section\n");

				//OK
				return statement(currentWord, &(*parent));
			}
			/*
			32. <builtin_extra> -> <builtin>
			*/
			else{
				//<builtin> SKIPFALSE
				currentWord = builtin(currentWord, &(*parent));

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing EOL after id_type (builtin case) in statement section\n");

				//OK
				return statement(currentWord, &(*parent));
			}



		}
		/*
		30. <id_type> -> ID <opt>
		*/
		else {
			// ID
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = GetToken(currentWord, true, false);
			}
			else ExitProgram(2, "Missing ID in id <opt>, statement section");

			if(strcmp(currentWord->content, "=") == 0) {
				Node* assign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				Node_insert(&assign1, tmp_content, NULL, tmp_type);
				currentWord = opt(currentWord, &(*assign1));
			}
			else {
				Node* funcid1 = Node_insert(&parent, tmp_content, NULL, tmp_type);
				currentWord = opt(currentWord, &(*funcid1));
			}
			//<opt> SKIPFALSE
			//if((result = opt(currentWord))) return result;
			 
			//EOL
			if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
			}
			else ExitProgram(2, "Missing EOL after id_type (ID <opt>) in statement section\n");


			//OK
			return statement(currentWord, &(*parent));
		}
		

	}

	//########################################
	/*
	67. <statement> -> if <expression> {<statement>} else {<statement>} <statement>
	*/
	// if
	else if (strcmp(currentWord->content, "if") == 0) {
		Node* if2 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);	//NODE IF IN STATEMENT
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord, &(*if2));
		 
		//if((result = expression(currentWord))) return result;
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in if before statement\n");

		//<statement> SKIPTRUE
		Node* body2 = Node_insert(&if2, "body", NULL, "body");
		currentWord = statement(currentWord, &(*body2));
		//if((result = prog_con(currentWord))) return result;

		 

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");


		Node* else2 = Node_insert(&if2, currentWord->content, NULL, currentWord->type); //NODE ELSE
		// else
		if (strcmp(currentWord->content, "else") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing keyword else in if section\n");

		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in if before else statement\n");

		//<statement> SKIPTRUE
		currentWord = statement(currentWord, &(*else2));

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");
	
		// EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing newline after if statement to border new command\n");
		//<statement>
		return statement(currentWord, &(*parent));



	}

	//########################################
	/*
	68. <statement> -> while <expression> {<statement>} EOL <statement>
	*/
	//while
	else if (strcmp(currentWord->content, "while") == 0) {
		Node* while2 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord, &(*while2));
		//if((result = expression(currentWord))) return result;
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		//{				
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { before while statement to border new command\n");
		
		//<statement> SKIPTRUE
		Node* body2 = Node_insert(&while2, "body", NULL, "body");
		currentWord = statement(currentWord, &(*body2));
		
		//}				
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing } after while statement to border new command\n");
		
		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after if statement in statement\n");
		
		//<statement>
		return statement(currentWord, &(*parent));
	}
	//########################################
	/*
	69. <statement> -> return <return_value> EOL <statement> NOT CHECKED NOT FINISHED
	*/
	else if(strcmp(currentWord->content, "return") == 0) {
		currentWord = GetToken(currentWord, false, false); // CAUTION

		//<return_value> SKIPFALSE
		currentWord = return_value(currentWord, &(*parent)); // return EOL if success

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after return in statement\n");

		//<statement>
		return statement(currentWord, &(*parent));
	}


	//########################################
	/*
	70. <statement> -> eps
	*/
	else if(strcmp(currentWord->content, "}") == 0) {
		 
		return currentWord;
	}

	return currentWord;
}

wordStr* prog_con(wordStr* currentWord, Node* DemoAST) {
	 
	 

	/*
	Node* DemoAST = NULL;
	Node* test1 = Node_insert(&DemoAST, "test1", NULL, "test");
	Node* test2 = Node_insert(&test1, "test2", NULL, "test");
	Node_insert(&test1, "test22", NULL, "test");
	Node_insert(&test1, "test23", NULL, "test");
	Node* test3 = Node_insert(&test2, "test3", NULL, "test");
	Node_insert(&test2, "test3", NULL, "test");
	Node_insert(&test3, "test41", NULL, "test");
	Node_insert(&test3, "test42", NULL, "test");
	Node_insert(&test3, "test43", NULL, "test");
	bool* flag = malloc(500 * sizeof(bool));
    print_AST(DemoAST, flag, 0, false);
    free(flag);
	*/

	//########################################
	/*
	1. <prog_con> -> func FID (<arg>) <types> EOL <prog_con> NOT CHECKED
	*/
	//func
	if(strcmp(currentWord->content, "func") == 0) {
		 
		currentWord = GetToken(currentWord, true, false);

		//FID
		if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing FID\n");
	

		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in function definition\n");

		//<arg> SKIPTRUE
		currentWord = arg(currentWord, &(*DemoAST)); 
		//after ARG

		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ) in function definition\n");

		//<types> SKIPFALSE
		currentWord = types(currentWord, &(*DemoAST));
		//if((result = types(currentWord))) return result;

		//EOF prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		 

		//EOL
		if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = GetToken(currentWord, true, true);
		}
		else ExitProgram(2, "Missing EOL after function definition in prog_con section\n");

		//EOF prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		//<prog_con> continuing recursively
		return prog_con(currentWord, &(*DemoAST));
	}

	//########################################
	/*
	2. <prog_con> -> <variant> ID <option> EOL <prog_con>
	*/
	// let || var
	else if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		Node* let_or_var1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		//ID
		if(strcmp(currentWord->type, "identifier") == 0) {
			Node_insert(&let_or_var1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing identifier in variable definition\n");

		//<option> SKIPFALSE
		currentWord = option(currentWord, &(*let_or_var1)); //returns newline

		//EOF Prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		//EOL
		if(strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, true);
		}
		else ExitProgram(2, "Missing EOL after variable definition in prog_con section\n");

		//EOF prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		//<prog_con> continuing recursively
		return prog_con(currentWord, &(*DemoAST));
	}

	//########################################
	/*
	3. <prog_con> -> <id_type> <prog_con>
	*/
	else if ((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
		/*
		29. <id_type> -> <builtin_extra> //Including write
		*/
		if((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			/*
			31. <builtin_extra> -> write(<write_params>)
			*/
			//write
			if(strcmp(currentWord->content, "write") == 0) {
				Node* write1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);	// NODE WRITE
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");

				//<write_params> SKIPTRUE
				 
				currentWord = write_params(currentWord, &(*write1));	// NODE WRITE_PARAMS
				
				 
				
				//)
				if (strcmp(currentWord->content, ")") == 0) {
					currentWord = GetToken(currentWord, false, true);
				}
				else ExitProgram(2, "Missing ) in write function calling\n");

				//EOF Prevention
				if (currentWord == NULL) {
					 
					return currentWord;
				}

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
				}
				else ExitProgram(2, "Missing EOL after id_type in statement section\n");

				//EOF prevention
				if (currentWord == NULL) {
					 
					return currentWord;
				}

				//OK
				return prog_con(currentWord, &(*DemoAST));
			}
			/*
			32. <builtin_extra> -> <builtin>
			*/
			else{
				//<builtin> SKIPFALSE
				currentWord = builtin(currentWord, &(*DemoAST));

				//EOF Prevention
				if (currentWord == NULL) {
					 
					return currentWord;
				}

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
				}
				else ExitProgram(2, "Missing EOL after id_type (builtin case) in statement section\n");

				//EOF prevention
				if (currentWord == NULL) {
					 
					return currentWord;
				}
				
				//OK
				return prog_con(currentWord, &(*DemoAST));
			}



		}
		/*
		30. <id_type> -> ID <opt>
		*/
		else {
			// ID
			char* tmp_content = currentWord->content;
			char* tmp_type = currentWord->type;
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = GetToken(currentWord, true, false);
			}
			else ExitProgram(2, "Missing ID in ID<opt>, section prog_con\n");

			if(strcmp(currentWord->content, "=") == 0) {
				Node* assign1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);
				Node_insert(&assign1, tmp_content, NULL, tmp_type);
				currentWord = opt(currentWord, &(*assign1));
			}
			else {
				Node* funcid1 = Node_insert(&DemoAST, tmp_content, NULL, tmp_type);
				currentWord = opt(currentWord, &(*funcid1));
			}


			//EOF Prevention
			if (currentWord == NULL) {
				 
				return currentWord;
			}

			//EOL
			if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
			}
			else ExitProgram(2, "Missing EOL after id_type (ID <opt>) in statement section\n");

			//EOF prevention
			if (currentWord == NULL) {
				 
				return currentWord;
			}

			//OK
			return prog_con(currentWord, &(*DemoAST));
		}
		

	}

	//########################################
	/*
	4. <prog_con> -> if <expression> {<statement>} else {<statement>} <prog_con>
	*/
	// if
	else if (strcmp(currentWord->content, "if") == 0) {
		Node* if1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord, &(*if1));
		//if((result = expression(currentWord))) return result;
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in if before statement\n");

		//<statement> SKIPTRUE
		Node* body1 = Node_insert(&if1, "body", NULL, "body");
		currentWord = statement(currentWord, &(*body1));
		//if((result = prog_con(currentWord))) return result;

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");

		// else
		Node* else1 = Node_insert(&if1, currentWord->content, NULL, currentWord->type);
		if (strcmp(currentWord->content, "else") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing keyword else in if section\n");

		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in if before else statement\n");

		//<statement> SKIPTRUE 
		currentWord = statement(currentWord, &(*else1));

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			 
			currentWord = GetToken(currentWord, false, true);
			// 
		}
		else ExitProgram(2, "Missing } in if after statement\n");

		//EOF Prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}
		
		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, true);
			 
		}
		else ExitProgram(2, "Missing newline after if statement to border new command\n");

		//EOF prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		//<prog_con>
		return prog_con(currentWord, &(*DemoAST));



	}

	//########################################
	/*
	5. <prog_con> -> while <expression> {<statement>} <prog_con> NOT CHECKED
	*/
	//while
	else if (strcmp(currentWord->content, "while") == 0) {
		Node* while1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord, &(*while1));
		
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		//{				
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { before while statement to border new command\n");
		
		//<statement> SKIPTRUE
		Node* body1 = Node_insert(&while1, "body", NULL, "body");
		currentWord = statement(currentWord, &(*body1));
		//if((result = prog_con(currentWord))) return result;

		//}				
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing } after while statement to border new command\n");

		//EOF Prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, true);
		}

		//EOF prevention
		if (currentWord == NULL) {
			 
			return currentWord;
		}
		
		//<prog_con>
		return prog_con(currentWord, &(*DemoAST));
	}

	//########################################
	/*
	7. <prog_con> -> eps EOF
	*/
	else if(currentWord->next == NULL) {
		 
		return currentWord;
	}

	//ERROR
	else ExitProgram(2, "Wrong first keyword in prog_con section\n");
	
	return currentWord;
}

wordStr* opt(wordStr* currentWord, Node* parent) { //NOT CHECKED
	 
	//########################################
	/*
	*48. <opt> -> (<params>)
	*/
	//(
	if(strcmp(currentWord->content, "(") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<params> SKIPTRUE
		currentWord = params(currentWord, &(*parent));//returns )

		//)
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true); //NEWLINE CONTINUES
		}
		else ExitProgram(2, "Missing ) in rule 4 in opt (<params>)\n");

		//OK
		return currentWord;
	}
	/*
	49.-52. FOR NOW, WILL OVERWRITE
	*/
	else if(strcmp(currentWord->content, "=") == 0) {
		currentWord = GetToken(currentWord, true, false);

		/*
		51. = <builtin>
		*/
		//<builtin> WITHOUT WRITE readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
			//<builtin> SKIPFALSE SHOULD RETURN EOL
			currentWord = builtin(currentWord, &(*parent));
			//OK
			return currentWord;
		}

		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, false, true);
			/*
			49. <opt> -> = ID EOL
			*/
			// EOL
			if(strcmp(currentWord->type, "newline") == 0) {
				//OK
				return currentWord;
			}
			/*
			50. <opt> -> = ID (<params>)
			*/
			//(
			else if(strcmp(currentWord->content, "(") == 0) {
				currentWord = GetToken(currentWord, true, false);
				//<params> SKIPTRUE
				currentWord = params(currentWord, &(*parent));
				//)
				if(strcmp(currentWord->content, ")") == 0) {
					currentWord = GetToken(currentWord, false, true);
				}
				//OK
				return currentWord;
			}

			/*
			part of 52. if first is operand is ID not expr
			*/
			// <sign> after ID
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				//SKIPFALSE
				currentWord = expression_more(currentWord, &(*parent)); //have to return token after EOL if correct

				//EOL already checked in exp_more
				//OK
				return currentWord;
			}

			else ExitProgram(2, "Wrong token after ID token in opt\n");

		}

		/*
		another part of 52. <term> excluding ID as term
		*/
		else if ((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			char* tmp_content1 = currentWord->content;
			char* tmp_type1 = currentWord->type;
			currentWord = GetToken(currentWord, true, false);
			
			if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				Node_insert(&sign1, tmp_content1, NULL, tmp_type1); //operand1
				currentWord = expression_more(currentWord, &(*sign1)); //return token after EOL
			}
			else {
				Node* operand1 = Node_insert(&parent, tmp_content1, NULL, tmp_type1); //operand1
				currentWord = expression_more(currentWord, &(*operand1)); //return token after EOL
			}

			//<expression_more> SKIPFALSE

			//OK
			return currentWord;
		}

		else ExitProgram(2, "Wrong expression after = in opt\n");


		return currentWord;
	}

	else ExitProgram(2, "Wrong expression after ID in ID_TYPE\n");

	//OK
	return currentWord;
}

//########################################
/*
<builtin> rule implementation
*/
wordStr* builtin(wordStr* currentWord, Node* parent) {
	
	 
	//########################################
	/*
	*37. <builtin> -> readString ()
	*/
	//readString
	if(strcmp(currentWord->content, "readString") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in readString function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in readString function\n");
		//OK
		return currentWord;
	}
	//readInt
	else if(strcmp(currentWord->content, "readInt") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		 
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in readInt function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in readInt function\n");
		//OK
		return currentWord;
	}
	//readDouble
	else if(strcmp(currentWord->content, "readDouble") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in readDouble function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in readDouble function\n");
		//OK
		return currentWord;
	}
	//Int2Double
	else if(strcmp(currentWord->content, "Int2Double") == 0) {
		Node* int2double1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in Int2Double function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&int2double1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters chr_t in Int2Double function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in Int2Double function\n");
		//OK
		return currentWord;
	}
	//Double2Int
	else if(strcmp(currentWord->content, "Double2Int") == 0) {
		Node* double2int1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in Double2Int function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&double2int1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters double_t in Double2Int function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in Double2Int function\n");
		//OK
		return currentWord;
	}
	//length
	else if(strcmp(currentWord->content, "length") == 0) {
		Node* length1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in length function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&length1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters length_t in length function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in length function\n");
		//OK
		return currentWord;
	}
	//substring
	else if(strcmp(currentWord->content, "substring\n") == 0) {
		Node* substring1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in substring function\n");

		//of
		Node* of1 = Node_insert(&substring1, currentWord->content, NULL, currentWord->type);
		if(strcmp(currentWord->content, "of") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing of in substring function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&of1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters length_t in substring function\n");
		//,
		if(strcmp(currentWord->content, ",") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing , in substring function after of\n");

		//startingAt
		Node* startingAt1 = Node_insert(&substring1, currentWord->content, NULL, currentWord->type);
		if(strcmp(currentWord->content, "startingAt") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing startingAt in substring function\n");
		//<chr_t>
		Node_insert(&startingAt1, currentWord->content, NULL, currentWord->type);
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters chr_t in substring function\n");
		//,
		if(strcmp(currentWord->content, ",") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing , in substring function after startingAt\n");

		//endingBefore
		Node* endingBefore1 = Node_insert(&substring1, currentWord->content, NULL, currentWord->type);
		if(strcmp(currentWord->content, "endingBefore") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing endingBefore in substring function\n");
		//<chr_t>
		Node_insert(&endingBefore1, currentWord->content, NULL, currentWord->type);
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters chr_t in substring function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in length function\n");
		//OK
		return currentWord;
	}
	//ord
	else if(strcmp(currentWord->content, "ord") == 0) {
		Node* ord1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in ord function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&ord1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters length_t in ord function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in ord function\n");
		//OK
		return currentWord;
	}
	//chr
	else if(strcmp(currentWord->content, "chr") == 0) {
		Node* chr1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in chr function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			Node_insert(&chr1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters chr_t in chr function\n");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing ) in chr function\n");
		//OK
		return currentWord;
	}
	else ExitProgram(2, "Wrong builtin name\n");


	
	return currentWord;
}

//########################################
/*
<arg> rule implementation
*/
wordStr* arg(wordStr* currentWord, Node* parent) {
	//int result;
	 
	//########################################
	/*
	*8. <arg> -> <param_name> ID : <type_spec> <args_more> NOT CHECKED
	*/
	//<param_name> NOTE: INCLUDING _
	if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {

		char* tmp_id = currentWord->content; // param_name cannot be same as ID name
		currentWord = GetToken(currentWord, true, false);
		
		//ID
		if ((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, tmp_id) != 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ID in arg or id has same name as param_name\n");
		
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing : in arg\n");
		
		//<type_spec> SKIPTRUE
		currentWord = type_spec(currentWord, &(*parent));
		//if((result = type_spec(currentWord))) return result;

		//<args_more> SKIPTRUE
		currentWord = args_more(currentWord, &(*parent));
		//if((result = args_more(currentWord))) return result;

		//OK
		return currentWord;
	}
	//########################################
	/*
	*9. <arg> -> eps
	*/
	// eps
	else if(strcmp(currentWord->content, ")")) {
		return currentWord;
	}
	else ExitProgram(2, "Missing param_name or ) in arg\n");

	return currentWord;
}

//########################################
/*
<args_more> rule implementation
*/
wordStr* args_more(wordStr* currentWord, Node* parent) {
	//int result;
	 
	/*
	10. <args_more> -> , PARAM_NAME ID : <type_spec> <args_more> NOT CHECKED
	*/
	//,
	if (strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);

		char* tmp_id = currentWord->content; // for comparing param_name with id

		//PARAM_NAME
		if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing param_name\n");
		
		//ID
		if ((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, tmp_id) != 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ID in args_more or id has same name as param_name\n");

		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing , in args_more\n");

		//<type_spec> SKIPTRUE
		currentWord = type_spec(currentWord, &(*parent));
		//if((result = type_spec(currentWord))) return result;

		//<args_more>
		return args_more(currentWord, &(*parent));
	}

	//11. <args_more> -> eps
	//)
	else if(strcmp(currentWord->content, ")") == 0) {
		return currentWord;
	}
	//NOT OKAY
	else ExitProgram(2, "Missing , in args_more\n");

	return currentWord;
}

wordStr* types(wordStr* currentWord, Node* parent) { // NOT CHECKED
	//int result;
	 
	/*
	12. <types> -> -> <type_spec> {statement_with_return} 
	*/
	// ->
	if (strcmp(currentWord->content, "->") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//<type_spec> SKIPTRUE
		currentWord = type_spec(currentWord, &(*parent));
		//if((result = type_spec(currentWord))) return result;

		//{
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in function definition\n");

		//<statement_with_return> SKIPTRUE
		currentWord = statement(currentWord, &(*parent)); //OVERWRITE to STATEMENT
		//if((result = prog_con(currentWord))) return result;
		//after <statement_with_return>

		//}
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing } in function definition\n");

		//OK
		return currentWord;
	}

	/*
	13. <types> -> {<statement_with_return>} NOT CHECKED
	*/
	//{
	else if(strcmp(currentWord->content, "{") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<statement_with_return> SKIPTRUE
		currentWord = statement(currentWord, &(*parent));
		//if((result = prog_con(currentWord))) return result;
		//after <statement_with_return>

		//}
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing } in types rule, func definition\n");

		//OK
		return currentWord;
	}

	//NOT OKAY
	else ExitProgram(2, "missing -> or { in function definition\n");

	return currentWord;
}

//########################################
/*
<params> rule implementation CHANGE RULES NOT CHECKED
*/
//<params>
wordStr* params(wordStr* currentWord, Node* parent) {
	 
	//ID
	if (strcmp(currentWord->type, "identifier") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//whether : || <params_more>
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);

			//<term>
			currentWord = term(currentWord, &(*parent));
			//if((result = term(currentWord))) return result;

			//<params_more> SKIPTRUE
			currentWord = params_more(currentWord, &(*parent));
			//if((result = params_more(currentWord))) return result;


		}
		//<params_more>
		else {
			//<params_more> SKIPTRUE
			currentWord = params_more(currentWord, &(*parent));
			//if((result = params_more(currentWord))) return result;
		}

		//OK
		return currentWord;
	}
	//<term> without ID
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
		currentWord = GetToken(currentWord, true, false);
		//<params_more> SKIPTRUE
		currentWord = params_more(currentWord, &(*parent));
		//if((result = params_more(currentWord))) return result;
		//OK
		return currentWord;
	}
	/*
	55. <params> -> eps
	*/
	else if(strcmp(currentWord->content, ")") == 0) {
		//OK
		return currentWord;
	}
	//NOT OK
	else ExitProgram(2, "Wrong parameter in called function\n");


	return currentWord;
}

//########################################
/*
<params_more> rule implementation CHANGE RULES NOT CHECKED
*/
wordStr* params_more(wordStr* currentWord, Node* parent) {
	//int result;
	 

	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//ID
		if (strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, true, false);

			//whether : || <params_more>
			//:
			if (strcmp(currentWord->content, ":") == 0) {
				currentWord = GetToken(currentWord, true, false);

				//<term> SKIPTRUE
				currentWord = term(currentWord, &(*parent));
				//if((result = term(currentWord))) return result;

				//<params_more>
				return params_more(currentWord, &(*parent));


			}
			//<params_more>
			else {
				return params_more(currentWord, &(*parent));
			}

			//OK
			return currentWord;
		}
		//<term> without ID
		else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			currentWord = GetToken(currentWord, true, false);
			//<params_more> SKIPTRUE
			return params_more(currentWord, &(*parent));
			//OK
			return currentWord;
		}
		//NOT OK
		else ExitProgram(2, "Wrong parameter after , in params_more\n");
		
	}
	/*
	58. <params_more> -> eps
	*/
	else if(strcmp(currentWord->content, ")") == 0) {
		return currentWord;
	}
	else ExitProgram(2, "Missing ',' or wrong parameter in params_more\n");

	return currentWord;
}

//########################################
/*
<type_spec> rule implementation
*/
wordStr* type_spec(wordStr* currentWord, Node* parent) {
	
	 
	//14.-19. <type_spec>
	if (strcmp(currentWord->content, "Double")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Double?")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int?")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String?")) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	//missing TYPE_SPEC
	else ExitProgram(2, "Missing type_spec\n");

	//OK
	return currentWord;
}

//########################################
/*
<write_params> rule implementation
*/
wordStr* write_params(wordStr* currentWord, Node* parent) {
	//int result;
	 
	/*
	33. <write_params> -> <term> <write_params_more>
	*/
	//<term>
	if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		 
		//<write_params_more> SKIPTRUE
		currentWord = write_params_more(currentWord, &(*parent)); // should return )
		//if((result = write_params_more(currentWord))) return result;
		 
		//OK
		return currentWord;
	}
	/*
	34. <write_params> -> eps
	*/
	else if(strcmp(currentWord->content, ")")) {
		//OK
		return currentWord;
	}
	else ExitProgram(2, "Missing write params or ) in write function calling\n");	

	return currentWord;
}

//########################################
/*
<write_params_more> rule implementation
*/
wordStr* write_params_more(wordStr* currentWord, Node* parent) {
	
	 
	/*
	35. <write_params_more> -> , <term> <write_params_more> NOT CHECKED
	*/
	//,
	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<term>
		if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
			Node_insert(&parent, currentWord->content, NULL, currentWord->type);//ANOTHER PARAMS NODE FROM WRITE
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing term in write_params_more part\n");
		//<write_params_more>
		return write_params_more(currentWord, &(*parent));
	}
	/*
	36. <write_params_more> -> eps
	*/
	//)
	else if(strcmp(currentWord->content, ")") == 0) {
		//OK
		return currentWord;
	}
	else ExitProgram(2, "Wrong parameter in write_params_more part\n");

	return currentWord;
}

//########################################
/*
<option> rule implementation NOT CHECKED
*/
wordStr* option(wordStr* currentWord, Node* parent) {
	//LET or VAR is PARENT
	 
	/*
	22. <option> -> : <type_spec> <assigns>
	*/
	//:
	if(strcmp(currentWord->content, ":") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<type_spec> SKIPFALSE bc after typespec can be EOL
		currentWord = type_spec(currentWord, &(*parent));
		
		//<assigns>
		//24. <assigns> -> =
		//=
		if(strcmp(currentWord->content, "=") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		//25. <assigns> -> eps EOL
		//EOL
		else if(strcmp(currentWord->type, "newline") == 0) {
			//OK
			return currentWord;
		}
		else ExitProgram(2, "Missing = in variable definition\n");

		//whether <builtin> OR ID<opt> OR <expression>

		//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
			

			//into <builtin> SKIPFALSE
			currentWord = builtin(currentWord, &(*parent)); //return token after )
			//if((result = builtin(currentWord))) return result;

			//OK
			return currentWord;
		}	
		//ID
		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, false, true);

			//(
			if(strcmp(currentWord->content, "(") == 0) {
				currentWord = GetToken(currentWord, true, false);

				//<params> SKIPTRUE
				currentWord = params(currentWord, &(*parent));
				//if((result = params(currentWord))) return result;
				//)
				if (strcmp(currentWord->content, ")")) {
					currentWord = GetToken(currentWord, false, true);
				}
				else ExitProgram(2, "Missing ) in variable definition near called function params\n");
				//OK
				return currentWord;
			}
			//ID in the beginning of expression <expression>
			//<sign>
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = GetToken(currentWord, true, false);

				//<term> SKIPTRUE
				currentWord = term(currentWord, &(*parent));

				//<expression_more> SKIPFALSE
				currentWord = expression_more(currentWord, &(*parent)); // return newline if success
				
				//OK
				return currentWord;
			}
			// EOL
			else if(strcmp(currentWord->type, "newline") == 0) {
				//OK
				return currentWord;
			}
			else ExitProgram(2, "Missing ( or sign after ID in typed definition of variable\n");
		
			return currentWord;
		}
		//<expression>??
		else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			Node_insert(&parent, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, false, true);
			
			//EOF Prevention
			if (currentWord == NULL) {
				 
				return currentWord;
			}

			//<EOL>
			if(strcmp(currentWord->type, "newline") == 0) {
				return currentWord;
			}

			//<sign>
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = GetToken(currentWord, true, false);

				//<term> SKIPTRUE
				currentWord = term(currentWord, &(*parent));

				//<expression_more> SKIPFALSE
				currentWord = expression_more(currentWord, &(*parent)); // return newline if success
				
				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing newline in option part expression, statement");

				//OK
				return currentWord;
			}

			else  ExitProgram(2, "Missing sign or newline in expression, section option");

		}


	}
	/*
	23. <option> -> <assign>
	*/
	else if(strcmp(currentWord->content, "=") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//<assign> -> = <expression>
		//<assign> -> = <builtin>
		//<assign> -> = ID <preopt>
		//=

		//whether <builtin> OR ID<opt> OR <expression>

		//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			//into <builtin> SKIPFALSE
			currentWord = builtin(currentWord, &(*parent));
			//if((result = builtin(currentWord))) return result;

			//OK
			return currentWord;
		}	
		//ID
		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, false, true);
			/*
			46. <preopt> -> (<params>)
			*/
			//(
			if(strcmp(currentWord->content, "(") == 0) {
				currentWord = GetToken(currentWord, true, false);

				//<params> SKIPTRUE
				currentWord = params(currentWord, &(*parent));
				
				//)
				if (strcmp(currentWord->content, ")")) {
					currentWord = GetToken(currentWord, false, true);
				}
				else ExitProgram(2, "Missing ) in variable definition near called function params\n");
				//OK
				return currentWord;
			}
			//ID in the beginning of expression <expression>
			//<sign>
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = GetToken(currentWord, true, false);

				//<term> SKIPTRUE
				currentWord = term(currentWord, &(*parent));

				//<expression_more>
				currentWord = expression_more(currentWord, &(*parent));

				//OK
				return currentWord;
			}
			/*
			47. <preopt> -> eps EOL
			*/
			//EOL
			else if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = GetToken(currentWord, true, false);
				return currentWord;
				//MAYBE RETURN CURRENTWORD?
			}
			else ExitProgram(2, "Missing ID or function or builtin func after = in option\n");
		}
	}

	//if there is just let a || var a and newline
	else if(strcmp(currentWord->type, "newline") == 0) {
		//currentWord = currentWord->next; //MAYBE CHECK
		return currentWord;
	}

	else ExitProgram(2, "There is no : or = after ID in prog_con variable definition beginning\n");

	return currentWord;
}

//########################################
/*
<return_value> rule implementation
*/
wordStr* return_value(wordStr* currentWord, Node* parent) {
	 
	
	/*
	<return_value>
	*/
	if(strcmp(currentWord->type, "identifier") == 0) {
		currentWord = GetToken(currentWord, false, true);
		//1. case -> EOL
		if(strcmp(currentWord->type, "newline") == 0) {
			return currentWord;
		}

		//2. case -> <sign>
		else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
			//not another token, but going straight to expression_word
			//SKIPFALSE
			currentWord = expression_more(currentWord, &(*parent)); // will return EOL if success

			//OK
			return currentWord;
		}
		//3. case -> !
		else if(strcmp(currentWord->content, "!") == 0) {
			currentWord = GetToken(currentWord, false, true);
			return currentWord;
		}
		else ExitProgram(2, "Missing sign or newline in expression in return_value\n");

	}
	/*
	<expression>
	*/
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
		//SKIPFALSE
		currentWord = expression(currentWord, &(*parent));
		return currentWord;
	}
	/*
	eps EOL
	*/
	else if(strcmp(currentWord->type, "newline") == 0) {
		return currentWord;
	}

	else ExitProgram(2, "Wrong return value token in return_value section\n");

	
	return currentWord;
}

//########################################
/*
<expression> rule implementation
*/
wordStr* expression(wordStr* currentWord, Node* parent) {
	//int result;
	 
	/*
	81. <expression> -> <term> <sign> <term> <expression_more> NOT CHECKED
	*/
	if(strcmp(currentWord->content, "let") == 0) { //in case of let a
		 
		currentWord = GetToken(currentWord, true, false);
		if(strcmp(currentWord->type,"identifier") == 0) {
			currentWord = GetToken(currentWord, true, false);
			return currentWord;
		}
		else ExitProgram(2, "Missing identifier near let in expression\n");
	}

	char* tmp_type = currentWord->type; // FOR CORRECT TREE STRUCTURE OF EXPRESSION, SIGN-PARENT, TERM(S) LEAFS
	char* tmp_content = currentWord->content;	
	//<term> SKIPFALSE bc idtype
	currentWord = term(currentWord, &(*parent));

	

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		Node_insert(&sign1, tmp_content, NULL, tmp_type);
		currentWord = GetToken(currentWord, true, false);
		//<term> SKIPFALSE
		currentWord = term(currentWord, &(*sign1)); //SECOND OPERAND
		//if((result = term(currentWord))) return result;
		//<expression_more>
		currentWord = expression_more(currentWord, &(*sign1));
		//if((result = expression_more(currentWord))) return result; //getting token after EOL from expression_more
	}
	// for example "let a" can be expression too

	//OK
	return currentWord;
}

//########################################
/*
<expression_more> rule implementation
*/
wordStr* expression_more(wordStr* currentWord, Node* parent) {
	//int result;
	 
	/*
	81. <expression_more> -> <sign> <term> <expression_more> EOL
	*/

	//<sign>
	//Conditions for AST
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		if((strcmp(parent->content, "+") != 0) && (strcmp(parent->content, "-") != 0) && (strcmp(parent->content, "*") != 0) && (strcmp(parent->content, "/") != 0) && (strcmp(parent->content, "==") != 0) && (strcmp(parent->content, "!=") != 0) && (strcmp(parent->content, "<") != 0) && (strcmp(parent->content, ">") != 0) && (strcmp(parent->content, "<=") != 0) && (strcmp(parent->content, ">=") != 0) && (strcmp(parent->content, "??") != 0)) {
			Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
			currentWord = term(currentWord, &(*sign1));
		}
		else {
			currentWord = GetToken(currentWord, true, false);
			currentWord = term(currentWord, &(*parent));
		}

		//<term> SKIPFALSE
		//if((result = term(currentWord))) return result;

		//<expression_more> continuing recursively
		return expression_more(currentWord, &(*parent));
	}
	//EOL
	else if ((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, ")") == 0)  || (strcmp(currentWord->content, "{") == 0)) {
		//OK
		 
		return currentWord;
	}
	else ExitProgram(2, "Missing newline or sign in expression (expression_more)\n");

	//OK
	return currentWord;
}

//########################################
/*
<term> rule implementation return token after term
*/
wordStr* term(wordStr* currentWord, Node* parent) {
	
	 
	//"STRINGLIT" || INTLIT || DOUBLELIT || ID
	if(strcmp(currentWord->type, "string") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "integer") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "float") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "identifier") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->content, "let") == 0) {
		Node* let1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		if(strcmp(currentWord->type, "identifier") == 0) {
			Node_insert(&let1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, false, true);
			return currentWord;
		}
		else ExitProgram(2, "Missing ID after let or var in term section\n");

	}
	else ExitProgram(2, "Wrong term\n");

	return currentWord;
}

wordStr* GetToken(wordStr* currentWord, bool ignoreNewLines, bool end_approved) {
	currentWord = currentWord->next;
	if(ignoreNewLines == true) {
			while (currentWord != NULL && strcmp(currentWord->content, "newline") == 0) { //Skip all newlines before first valid token
				currentWord = currentWord->next;
			}
	}
	if ((currentWord == NULL) && (end_approved == true)) {
		 
		return NULL;//EOF
	}
	else if ((currentWord == NULL) && (end_approved == false)) {
		ExitProgram(2, "Unexpected EOF\n");
	}
	return currentWord;
}

wordStr* GetFirstToken(wordListStr* wrdList, bool ignoreNewLines, bool end_approved) {
	wordStr* currentWord = wrdList->first;
	if(ignoreNewLines == true) {
			while (currentWord != NULL && strcmp(currentWord->content, "newline") == 0) { //Skip all newlines before first valid token
				currentWord = currentWord->next;
			}
	}
	if ((currentWord == NULL) && (end_approved == true)) {
		 
		return NULL;//EOF
	}
	else if ((currentWord == NULL) && (end_approved == false)) {
		ExitProgram(2, "Unexpected EOF\n");
	}
	return currentWord;
}
