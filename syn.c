
#include "syn.h"

#define maxCommandTokenCount 255

//node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList, Node* DemoAST) {
	printf("begin\n");
	if(wrdList->first->content == NULL) {
		return;
	}
	wordStr* currentWord = GetFirstToken(wrdList, true, true); //GET HEADER

	//Node* DemoAST = NULL;

	//EOF Prevention
	if (currentWord == NULL) {
		printf("END of file\n");
		return;
	}	

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
	printf("####################IN STATEMENT:%s####################\n", currentWord->content);

	//########################################
	/*
	65. <statement> -> <variant> ID <option> EOL <statement>	OK
	*/
	// let || var
	if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		Node *let_or_var2 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);

		//<option> SKIPFALSE
		currentWord = option(currentWord, &(*let_or_var2));
		printf("vratene:%s\n", currentWord->content);

		//possible }
		if (strcmp(currentWord->content, "}") == 0) {
			return currentWord;
		}

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

		printf("###################IN_STATEMENT_IDTYPE#####################\n");

		/*
		29. <id_type> -> <builtin_extra> //Including write NOT CHECHED
		*/
		if((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			/*
			31. <builtin_extra> -> write(<write_params>)
			*/
			//write
			if(strcmp(currentWord->content, "write") == 0) {
				Node* write1 = Node_insert(&parent, currentWord->content, NULL, "function");
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");
				//<write_params>
				//printf("%s", currentWord->content);
				
				//SKIPTRUE
				currentWord = write_params(currentWord, &(*write1)); //should return ) if success
				//if((result = write_params(currentWord))) return result;
				////printf("aktualny token po vykone funkcie write:%s\n", currentWord->content);
				
				//)
				if (strcmp(currentWord->content, ")") == 0) {
					currentWord = GetToken(currentWord, false, false);
				}
				else ExitProgram(2, "Missing ) in write function calling\n");

				// possible }
				if (strcmp(currentWord->content, "}") == 0) {
					return currentWord;
				}

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

				// possible }
				if (strcmp(currentWord->content, "}") == 0) {
					return currentWord;
				}

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
				//printf("assigning\n");
				Node* assign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				Node_insert(&assign1, tmp_content, NULL, tmp_type); // left side of =
				currentWord = opt(currentWord, &(*assign1));
			}
			else {
				Node* funcid1 = Node_insert(&parent, tmp_content, NULL, tmp_type);
				currentWord = opt(currentWord, &(*funcid1));
			}
			//<opt> SKIPFALSE
			//if((result = opt(currentWord))) return result;

			//printf("IDOPT\n");

			// possible }
			if (strcmp(currentWord->content, "}") == 0) {
				return currentWord;
			}

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
		//possible EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}

		//printf("curcont:%s\n", currentWord->content);
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

		////printf("token:%s\n", currentWord->content);

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
		else ExitProgram(2, "Missing { in elseif before else statement\n");

		//<statement> SKIPTRUE
		Node* elsebody2 = Node_insert(&else2, "body", NULL, "body"); //NODE ELSE BODY
		currentWord = statement(currentWord, &(*elsebody2));

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");
	
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

		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}

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
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } after while statement to border new command\n");
		
		
		//<statement>
		return statement(currentWord, &(*parent));
	}
	//########################################
	/*
	69. <statement> -> return <return_value> EOL <statement> NOT CHECKED NOT FINISHED
	*/
	else if(strcmp(currentWord->content, "return") == 0) {
		Node* return2 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, false); // CAUTION

		//<return_value> SKIPFALSE
		currentWord = return_value(currentWord, &(*return2)); // return EOL if success

		//possible }
		if (strcmp(currentWord->content, "}") == 0) {
			return currentWord;
		}

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
		printf("END OF ACTUAL STATEMENT -> Returning to prog_con\n");
		return currentWord;
	}

	return currentWord;
}

wordStr* prog_con(wordStr* currentWord, Node* DemoAST) {
	printf("####################IN PROG_CON####################\n");
	//printf("currentinprogcon:%s\n", currentWord->content);

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
	1. <prog_con> -> func FID (<arg>) <types> EOL <prog_con> OK
	*/
	//func
	if(strcmp(currentWord->content, "func") == 0) {
		Node* func1 = Node_insert(&DemoAST, currentWord->content, NULL, currentWord->type);
		//printf("<prog_con> -> func FID (<arg>) <types> {<statement>} <prog_con>");
		currentWord = GetToken(currentWord, true, false);

		//FID
		Node* fid1 = Node_insert(&func1, currentWord->content, NULL, currentWord->type);
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
		currentWord = arg(currentWord, &(*fid1)); 
		//after ARG

		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ) in function definition\n");

		//<types> SKIPFALSE
		currentWord = types(currentWord, &(*func1));
		//if((result = types(currentWord))) return result;
		//printf("afterfunctypes:%s\n", currentWord->content);

		//EOF prevention
		if (currentWord == NULL) {

			printf("END of file\n");

			return currentWord;
		}

		//printf("tokenFUNC:%s\n", currentWord->content);


		//EOL
		if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = GetToken(currentWord, true, true);
		}
		

		//EOF prevention
		if (currentWord == NULL) {
			printf("END of file\n");
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

		//<option> SKIPFALSE
		currentWord = option(currentWord, &(*let_or_var1)); //returns newline


		//EOF Prevention
		if (currentWord == NULL) {

			printf("END of file\n");
			return currentWord;
		}

		//EOL
		//printf("token:%s\n", currentWord->content);
		if(strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, true);
		}
		else ExitProgram(2, "Missing EOL after variable definition in prog_con section\n");

		//EOF prevention
		if (currentWord == NULL) {
			printf("END of file\n");

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
				Node* write1 = Node_insert(&DemoAST, currentWord->content, NULL, "function");	// NODE WRITE
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");

				//<write_params> SKIPTRUE
				//printf("%s", currentWord->content);

				currentWord = write_params(currentWord, &(*write1));	// NODE WRITE_PARAMS
				
				////printf("aktualny token po vykone funkcie write:%s\n", currentWord->content);
				
				//)
				if (strcmp(currentWord->content, ")") == 0) {
					currentWord = GetToken(currentWord, false, true);
				}
				else ExitProgram(2, "Missing ) in write function calling\n");

				//EOF Prevention
				if (currentWord == NULL) {
					printf("END of file\n");
					return currentWord;
				}

				if(strcmp(currentWord->content, "}") == 0) {
				//OK
				return currentWord;
				}

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
				}
				else ExitProgram(2, "Missing EOL after id_type in statement section\n");

				//EOF prevention
				if (currentWord == NULL) {

					printf("END of file\n");

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
					printf("END of file\n");
					return currentWord;
				}

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
				}
				else ExitProgram(2, "Missing EOL after id_type (builtin case) in statement section\n");

				//EOF prevention
				if (currentWord == NULL) {
					
					printf("END of file\n");
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
				printf("END of file\n");
				return currentWord;
			}

			//EOL
			if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, true);
			}
			else ExitProgram(2, "Missing EOL after id_type (ID <opt>) in statement section\n");

			//EOF prevention
			if (currentWord == NULL) {

				printf("END of file\n");

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

		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
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
		else ExitProgram(2, "Missing { in elseif before else statement\n");

		//<statement> SKIPTRUE
		Node* elsebody1 = Node_insert(&else1, "body", NULL, "body"); //NODE ELSE BODY 
		currentWord = statement(currentWord, &(*elsebody1));

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			//printf("aktual tokenhere:%s\n", currentWord->type);
			currentWord = GetToken(currentWord, true, true);

		}
		else ExitProgram(2, "Missing } in if after statement\n");

		//EOF prevention
		if (currentWord == NULL) {
			printf("END of file\n");
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

		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		
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
			currentWord = GetToken(currentWord, true, true);
		}
		else ExitProgram(2, "Missing } after while statement to border new command\n");

		//EOF Prevention
		if (currentWord == NULL) {
			printf("END of file\n");

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
		printf("END OF FILE\n");
		return currentWord;
	}

	//ERROR
	else ExitProgram(2, "Wrong first keyword in prog_con section\n");
	
	return currentWord;
}

wordStr* opt(wordStr* currentWord, Node* parent) { //NOT CHECKED

	printf("###################IN_OPT#####################\n");
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
			char* tmp_type = currentWord->type;
			char* tmp_content = currentWord->content;
			currentWord = GetToken(currentWord, false, true);

			//EOF Prevention
			if (currentWord == NULL) {
				//printf("sfdf\n");
				Node_insert(&parent, tmp_content, NULL, tmp_type);
				return currentWord;
			}

			/*
			49. <opt> -> = ID EOL
			*/
			// EOL
			if((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, "}") == 0)) {
				//OK
				//printf("IDEOL\n");
				Node_insert(&parent, tmp_content, NULL, tmp_type);

				return currentWord;
			}
			/*
			50. <opt> -> = ID (<params>)
			*/
			//(
			else if(strcmp(currentWord->content, "(") == 0) {
				Node* fid1 = Node_insert(&parent, tmp_content, NULL, tmp_type);
				currentWord = GetToken(currentWord, true, false);
				//<params> SKIPTRUE
				currentWord = params(currentWord, &(*fid1));
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
				Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				Node_insert(&sign1, tmp_content, NULL, tmp_type);
				currentWord = expression_more(currentWord, &(*sign1)); //have to return token after EOL if correct

				//EOL already checked in exp_more
				//OK
				return currentWord;
			}

			else ExitProgram(2, "Wrong token after ID token in opt\n");

		}

		/*
		another part of 52. <term> excluding ID as term
		*/
		else if ((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0)) {
			//printf("inopt\n");
			char* tmp_content1 = currentWord->content;
			char* tmp_type1 = currentWord->type;
			currentWord = GetToken(currentWord, false, true); // change possib of EOF, prog_con OR statement
			
			//EOF Prevention
			if (currentWord == NULL) {
				//printf("sfdf\n");
				Node_insert(&parent, tmp_content1, NULL, tmp_type1);
				return currentWord;
			}
			//printf("currentT:\n");

			// EOL
			if((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, "}") == 0)) {
				//OK
				//printf("IDEOL\n");
				Node_insert(&parent, tmp_content1, NULL, tmp_type1);

				return currentWord;
			}

			
			
			if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
				Node_insert(&sign1, tmp_content1, NULL, tmp_type1); //operand1
				currentWord = expression_more(currentWord, &(*sign1)); //return token after EOL
			}
			else if(strcmp(currentWord->type, "newline") == 0) { // id = 5
				Node* operand1 = Node_insert(&parent, tmp_content1, NULL, tmp_type1); //operand1
				currentWord = expression_more(currentWord, &(*operand1)); //return token after EOL
				return currentWord;
			}
			else {
				ExitProgram(2, "Missing sign or newline in section <opt>\n");
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
	
	printf("###################IN_BUILTIN#####################:%s\n", currentWord->content);
	//########################################
	/*
	*37. <builtin> -> readString ()
	*/
	//readString
	if(strcmp(currentWord->content, "readString") == 0) {
		Node_insert(&parent, currentWord->content, NULL, "function");
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
		Node_insert(&parent, currentWord->content, NULL, "function");
		//printf("in readINT\n");
    
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
		Node_insert(&parent, currentWord->content, NULL, "function");
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
		Node* int2double1 = Node_insert(&parent, currentWord->content, NULL, "function");
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
		Node* double2int1 = Node_insert(&parent, currentWord->content, NULL, "function");
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in Double2Int function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "double") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
		Node* length1 = Node_insert(&parent, currentWord->content, NULL, "function");
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
	else if(strcmp(currentWord->content, "substring") == 0) {
		Node* substring1 = Node_insert(&parent, currentWord->content, NULL, "function");
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
		Node* ord1 = Node_insert(&parent, currentWord->content, NULL, "function");
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
		Node* chr1 = Node_insert(&parent, currentWord->content, NULL, "function");
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
	printf("###################IN_ARG#####################\n");
	//########################################
	/*
	*8. <arg> -> <param_name> ID : <type_spec> <args_more> DONE
	*/
	//<param_name> NOTE: INCLUDING _
	if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {
		Node* paramname1 = Node_insert(&parent, currentWord->content, NULL, "paramname");
		char* tmp_id = currentWord->content; // param_name cannot be same as ID name
		currentWord = GetToken(currentWord, true, false);
		
		//ID
		Node* paramid1 = Node_insert(&paramname1, currentWord->content, NULL, "paramid");
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
		currentWord = type_spec(currentWord, &(*paramid1));
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
	else if(strcmp(currentWord->content, ")") == 0) {
		return currentWord;
	}
	else ExitProgram(2, "Missing param_name or ) in arg\n");

	return currentWord;
}

//########################################
/*
<args_more> rule implementation	DONE
*/
wordStr* args_more(wordStr* currentWord, Node* parent) {
	//int result;
	printf("###################IN_ARGS_MORE#####################\n");
	/*
	10. <args_more> -> , PARAM_NAME ID : <type_spec> <args_more> NOT CHECKED
	*/
	//,
	if (strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);

		char* tmp_id = currentWord->content; // for comparing param_name with id

		//PARAM_NAME
		Node* paramname1 = Node_insert(&parent, currentWord->content, NULL, "paramname");
		if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing param_name\n");
		
		//ID
		Node* paramid1 = Node_insert(&paramname1, currentWord->content, NULL, "paramid");
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
		currentWord = type_spec(currentWord, &(*paramid1));
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

wordStr* types(wordStr* currentWord, Node* parent) { // OK
	//int result;
	printf("###################IN_TYPES#####################\n");
	/*
	12. <types> -> -> <type_spec> {statement_with_return} 
	*/
	// ->
	if (strcmp(currentWord->content, "->") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//<type_spec> SKIPTRUE
		currentWord = type_spec(currentWord, &(*parent));
		
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}

		//{
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in function definition\n");

		//<statement_with_return> SKIPTRUE
		Node* funcbody1 = Node_insert(&parent, "body", NULL, "body");
		currentWord = statement(currentWord, &(*funcbody1)); //OVERWRITE to STATEMENT
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
	13. <types> -> {<statement_with_return>} OK
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
<params> rule implementation	OK
*/
//<params>
wordStr* params(wordStr* currentWord, Node* parent) {
	printf("###################IN_PARAMS#####################\n");
	//ID
	if (strcmp(currentWord->type, "identifier") == 0) {
		Node* pid1 = Node_insert(&parent, currentWord->content, NULL, "paramname");
		currentWord = GetToken(currentWord, true, false);

		//whether : || <params_more>
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);

			//<term>
			currentWord = term(currentWord, &(*pid1));
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
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0)) {

		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
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
<params_more> rule implementation	OK
*/
wordStr* params_more(wordStr* currentWord, Node* parent) {
	//int result;
	printf("###################IN_PARAMS_MORE#####################\n");

	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//ID
		if (strcmp(currentWord->type, "identifier") == 0) {
			Node* pid1 = Node_insert(&parent, currentWord->content, NULL, "paramname");
			currentWord = GetToken(currentWord, true, false);

			//whether : || <params_more>
			//:
			if (strcmp(currentWord->content, ":") == 0) {
				currentWord = GetToken(currentWord, true, false);

				//<term> SKIPTRUE
				currentWord = term(currentWord, &(*pid1));
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
		else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0)) {
			Node_insert(&parent, currentWord->content, NULL, currentWord->type);
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
<type_spec> rule implementation	OK
*/
wordStr* type_spec(wordStr* currentWord, Node* parent) {
	
	printf("###################IN_TYPE_SPEC#####################\n");
	//14.-19. <type_spec>
	if (strcmp(currentWord->content, "Double") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Double?") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int?") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String?") == 0) {
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

	printf("###################IN_WRITE_PARAMS#####################\n");
	/*
	33. <write_params> -> <term> <write_params_more>
	*/
	//<term>
	if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0) || (strcmp(currentWord->type, "string") == 0)) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		////printf("tu som:%s\n", currentWord->content);
		//<write_params_more> SKIPTRUE
		currentWord = write_params_more(currentWord, &(*parent)); // should return )
		//if((result = write_params_more(currentWord))) return result;
		////printf("vraciam sa s:%s\n", currentWord->content);
		//OK
		return currentWord;
	}
	/*
	34. <write_params> -> eps
	*/
	else if(strcmp(currentWord->content, ")") == 0) {
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
	
	printf("###################IN_WRITE_PARAMS_MORE#####################\n");
	/*
	35. <write_params_more> -> , <term> <write_params_more> NOT CHECKED
	*/
	//,
	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<term>
		if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0) || (strcmp(currentWord->type, "string") == 0)) {
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
<option> rule implementation	OK
*/
wordStr* option(wordStr* currentWord, Node* parent) {
	//LET or VAR is PARENT
	printf("###################IN_OPTION#####################\n");

	// <preoption> -> ID <option>
	if(strcmp(currentWord->type, "identifier") == 0) {
		Node* id1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, false);
	

		/*
		22. <option> -> : <type_spec> <assigns>	OK
		*/
		//:
		if(strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);

			//char* tmp_type = currentWord->type;
			//char* tmp_content = currentWord->content;
			//<type_spec> SKIPFALSE bc after typespec can be EOL
			currentWord = type_spec(currentWord, &(*id1));


			//<assigns>
			//24. <assigns> -> =
			//=
			if(strcmp(currentWord->content, "=") == 0) {
				currentWord = GetToken(currentWord, false, false);
			}
			//25. <assigns> -> eps EOL
			//EOL
			else if(strcmp(currentWord->type, "newline") == 0) {
				//OK
				return currentWord;
			}
			//possible }
			else if(strcmp(currentWord->content, "}") == 0) {
				//OK
				//printf("tokenik:%s\n", currentWord->content);
				return currentWord;
			}

			else ExitProgram(2, "Missing = in variable definition\n");

			//whether <builtin> OR ID<opt> OR <expression>


			//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
			if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
				

				//into <builtin> SKIPFALSE
				currentWord = builtin(currentWord, &(*parent)); //return token after )
				if (currentWord == NULL) {
					printf("END of file\n");
					return currentWord;
				}
				//if((result = builtin(currentWord))) return result;

				//OK
				return currentWord;
			}	
			//ID
			else if(strcmp(currentWord->type, "identifier") == 0) {
				//printf("tok:%s\n", currentWord->content);
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
				currentWord = GetToken(currentWord, false, true);

				if (currentWord == NULL) {
					printf("END of file\n");
					Node_insert(&id1, tmp_content, NULL, tmp_type);
					return currentWord;
				}

				//(
				if(strcmp(currentWord->content, "(") == 0) {
					//now assigning function ID 
					Node* fid1 = Node_insert(&parent, tmp_content, NULL, tmp_type);

					currentWord = GetToken(currentWord, true, false);

					//<params> SKIPTRUE
					currentWord = params(currentWord, &(*fid1));
					//if((result = params(currentWord))) return result;
					//)
					printf("after params:%s\n", currentWord->content);
					if (strcmp(currentWord->content, ")") == 0) {
						currentWord = GetToken(currentWord, false, true);
					}
					else ExitProgram(2, "Missing ) in variable definition near called function params\n");
					//OK
					return currentWord;
				}
				//ID in the beginning of expression <expression>
				//<sign>
				else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type); //adding sign to root
				

					Node_insert(&sign1, tmp_content, NULL, tmp_type); //CHANGE bc of AST logic

					//currentWord = GetToken(currentWord, true, false);

					//<term> SKIPTRUE
					//currentWord = term(currentWord, &(*sign1)); //returns token after term

					//<expression_more> SKIPFALSE
					currentWord = expression_more(currentWord, &(*sign1)); // return newline if success
					
					//OK
					return currentWord;
				}

				//possible EOF

				//possible }
				else if (strcmp(currentWord->content, "}") == 0) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}

				// EOL
				else if(strcmp(currentWord->type, "newline") == 0) {
					//OK
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}
				else ExitProgram(2, "Missing ( or sign after ID in typed definition of variable\n");
			
				return currentWord;
			}
			//<expression>??
			else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0) || (strcmp(currentWord->content, "nil") == 0)) {	//CHANGE TO TYPE NIL
				//printf("tunee\n");
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
				currentWord = GetToken(currentWord, false, true);
				

				//EOF Prevention
				if (currentWord == NULL) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					printf("END of file\n");
					return currentWord;
				}

				//possible }
				if (strcmp(currentWord->content, "}") == 0) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}

				//<EOL>
				if(strcmp(currentWord->type, "newline") == 0) {
					//currentWord = GetToken(currentWord, true, true);
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}
			

				//<sign>
				if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type); //adding sign to root
					Node_insert(&sign1, tmp_content, NULL, tmp_type);
					

					//<expression_more> SKIPFALSE
					currentWord = expression_more(currentWord, &(*sign1)); // return newline if success
					//printf("after\n");
					//EOF Prevention
					if (currentWord == NULL) {
						printf("END of files\n");
						return currentWord;
					}

					//possible }
					if (strcmp(currentWord->content, "}") == 0) {
						return currentWord;
					}

					//EOL
					if(strcmp(currentWord->type, "newline") == 0) {					
						return currentWord;
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

				//EOF Prevention
				if (currentWord == NULL) {
					printf("END of files\n");
					return currentWord;
				}
				//if((result = builtin(currentWord))) return result;

				//OK
				return currentWord;
			}	
			//ID
			else if(strcmp(currentWord->type, "identifier") == 0) {
				//printf("tok:%s\n", currentWord->content);
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
				currentWord = GetToken(currentWord, false, true);

				//EOF Prevention
				if (currentWord == NULL) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					printf("END of files\n");
					return currentWord;
				}
				/*
				46. <preopt> -> (<params>)
				*/
				//(
				if(strcmp(currentWord->content, "(") == 0) {
					Node* fid = Node_insert(&parent, tmp_content, NULL, tmp_type);
					currentWord = GetToken(currentWord, true, false);

					//<params> SKIPTRUE
					currentWord = params(currentWord, &(*fid));
					//printf("after params:%s\n", currentWord->content);
					//)
					if (strcmp(currentWord->content, ")") == 0) {
						currentWord = GetToken(currentWord, false, true);
					}
					else ExitProgram(2, "Missing ) in variable definition near called function params\n");

					//OK
					return currentWord;
				}
				//ID in the beginning of expression <expression>
				//<sign>
				else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type); //adding sign to root
					Node_insert(&sign1, tmp_content, NULL, tmp_type);
					

					//<expression_more> SKIPFALSE
					currentWord = expression_more(currentWord, &(*sign1)); // return newline if success
					//printf("after\n");
					//EOF Prevention
					if (currentWord == NULL) {
						printf("END of files\n");
						return currentWord;
					}

					//possible }
					if (strcmp(currentWord->content, "}") == 0) {
						return currentWord;
					}

					//EOL
					if(strcmp(currentWord->type, "newline") == 0) {					
						return currentWord;
					}
					else ExitProgram(2, "Missing newline in option part expression, statement");

					//OK
					return currentWord;
				}
				/*
				47. <preopt> -> eps EOL
				*/
				//EOL
				else if((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, "}") == 0)) {
					//currentWord = GetToken(currentWord, true, false);
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
					//MAYBE RETURN CURRENTWORD?
				}
				else ExitProgram(2, "Missing ID or function or builtin func after = in option\n");
			}

			//<expression>??
			else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0)  || (strcmp(currentWord->content, "nil") == 0)) {	//CHANGE TO TYPE NIL
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
				
				currentWord = GetToken(currentWord, false, true);
				//printf("tusom\n");

				//EOF Prevention
				if (currentWord == NULL) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					printf("END of file\n");
					return currentWord;
				}

				//<EOL>
				if((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, "}") == 0)) {
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					//currentWord = GetToken(currentWord, true, true);
					return currentWord;
				}

			

				//<sign>
				if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type); //adding sign to root
					Node_insert(&sign1, tmp_content, NULL, tmp_type);
					
					currentWord = expression_more(currentWord, &(*sign1)); // return newline if success
					//printf("after\n");
					//EOF Prevention
					if (currentWord == NULL) {
						printf("END of files\n");
						return currentWord;
					}

					//possible }
					if (strcmp(currentWord->content, "}") == 0) {
						return currentWord;
					}

					//EOL
					if(strcmp(currentWord->type, "newline") == 0) {					
						return currentWord;
					}
					else ExitProgram(2, "Missing newline in option part expression, statement");

					//OK
					return currentWord;
				}

				else  ExitProgram(2, "Missing sign or newline in expression, section option");

			}

		}

		//if there is just let a || var a and newline
		else if(strcmp(currentWord->type, "newline") == 0) {
			//currentWord = currentWord->next; //MAYBE CHECK
			return currentWord;
		}

		else ExitProgram(2, "There is no : or = after ID in prog_con variable definition beginning\n");

	}

	else ExitProgram(2, "Missing identifier in variable definition\n");

	return currentWord;
}

//########################################
/*
<return_value> rule implementation
*/
wordStr* return_value(wordStr* currentWord, Node* parent) {
	printf("###################IN_RETURN_VALUE#####################\n");
	
	/*
	<return_value>
	*/
	if(strcmp(currentWord->type, "identifier") == 0) {
		char* tmp_content = currentWord->content;
		char* tmp_type = currentWord->type; 
		currentWord = GetToken(currentWord, false, false);
		//1. case -> EOL or } possible
        if (strcmp(currentWord->type, "newline") == 0) {
            currentWord = GetToken(currentWord, true, false);
            Node_insert(&parent, tmp_content, NULL, tmp_type);
            return currentWord;
        }
        else if (strcmp(currentWord->content, "}") == 0) {
            //printf("here\n");
            Node_insert(&parent, tmp_content, NULL, tmp_type);
            return currentWord;
        }

		//2. case -> <sign>
		else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
			//not another token, but going straight to expression_word
			//SKIPFALSE
			Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
			Node_insert(&sign1, tmp_content, NULL, tmp_type);
			currentWord = expression_more(currentWord, &(*sign1)); // will return EOL if success

			//OK
			return currentWord;
		}
		//3. case -> !
		else if(strcmp(currentWord->content, "!") == 0) {
			Node* unar1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
			Node_insert(&unar1, tmp_content, NULL, tmp_type);
			currentWord = GetToken(currentWord, false, true);
			return currentWord;
		}
		else ExitProgram(2, "Missing sign or newline in expression in return_value\n");

	}
	/*
	<expression>
	*/
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0)) {
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
	printf("###################IN_EXPRESSION#####################\n");
	/*
	81. <expression> -> <term> <sign> <term> <expression_more> NOT CHECKED
	*/
	if(strcmp(currentWord->content, "let") == 0) { //in case of let a
		Node* let1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, true, false);
		if(strcmp(currentWord->type,"identifier") == 0) {
			Node_insert(&let1, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
			return currentWord;
		}
		else ExitProgram(2, "Missing identifier near let in expression\n");
	}

	char* tmp_type = currentWord->type; // FOR CORRECT TREE STRUCTURE OF EXPRESSION, SIGN-PARENT, TERM(S) LEAFS
	char* tmp_content = currentWord->content;	
	//<term> SKIPFALSE bc idtype
	currentWord = GetToken(currentWord, true, false);

	

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		Node_insert(&sign1, tmp_content, NULL, tmp_type); //FIRST operand
		currentWord = GetToken(currentWord, true, false);
		//<term> SKIPFALSE
		currentWord = term(currentWord, &(*sign1)); //SECOND operand
		//if((result = term(currentWord))) return result;
		//<expression_more>
		//printf("idem do expmore\n");
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
	printf("###################IN_EXPRESSION_MORE#####################:\n");
	/*
	81. <expression_more> -> <sign> <term> <expression_more> EOL
	*/

	//<sign>
	//Conditions for AST
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		//not sign parent
		if((strcmp(parent->content, "+") != 0) && (strcmp(parent->content, "-") != 0) && (strcmp(parent->content, "*") != 0) && (strcmp(parent->content, "/") != 0) && (strcmp(parent->content, "==") != 0) && (strcmp(parent->content, "!=") != 0) && (strcmp(parent->content, "<") != 0) && (strcmp(parent->content, ">") != 0) && (strcmp(parent->content, "<=") != 0) && (strcmp(parent->content, ">=") != 0) && (strcmp(parent->content, "??") != 0)) {
			Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
			currentWord = GetToken(currentWord, true, false);
			currentWord = term(currentWord, &(*sign1));
			//printf("after\n");
			//EOF Prevention
			if (currentWord == NULL) {
				printf("END of files\n");
				return currentWord;
			}
			return expression_more(currentWord, &(*sign1));
		}
		else { //sign parent
			currentWord = GetToken(currentWord, false, false);
			if ((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "double") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
				char* tmp_content = currentWord->content;
				char* tmp_type = currentWord->type;
				currentWord = GetToken(currentWord, false, true);

				if (currentWord == NULL) {
					printf("END of file\n");
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}

				if ((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, ")") == 0)  || (strcmp(currentWord->content, "{") == 0)  || (strcmp(currentWord->content, "}") == 0)) {
					printf("vrac\n");
					Node_insert(&parent, tmp_content, NULL, tmp_type);
					return currentWord;
				}

				if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					Node* sign1 = Node_insert(&parent, currentWord->content, NULL, currentWord->type);
					Node_insert(&sign1, tmp_content, NULL, tmp_type);
					return expression_more(currentWord, &(*sign1));
				}


			}

			//currentWord = term(currentWord, &(*parent));
			
			//return expression_more(currentWord, &(*parent));
		}

		//<term> SKIPFALSE
		//if((result = term(currentWord))) return result;

		//<expression_more> continuing recursively
		
	}
	
	//EOL
	else if ((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, ")") == 0)  || (strcmp(currentWord->content, "{") == 0)  || (strcmp(currentWord->content, "}") == 0)) {
		//OK

		printf("GOING OUT FROM EXP MORE WITH:%s\n", currentWord->content);

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
	
	printf("###################IN_TERM#####################\n");
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
	else if(strcmp(currentWord->type, "double") == 0) {
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
	else if(strcmp(currentWord->content, "nil") == 0) {
		Node_insert(&parent, currentWord->content, NULL, currentWord->type);
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
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
		printf("som nil\n");
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
		printf("som nil\n");
		return NULL;//EOF
	}
	else if ((currentWord == NULL) && (end_approved == false)) {
		ExitProgram(2, "Unexpected EOF\n");
	}
	return currentWord;
}