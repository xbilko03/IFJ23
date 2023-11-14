#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

wordStr* GetToken(wordStr* currentWord, bool ignoreNewLines, bool end_approved);
wordStr* GetFirstToken(wordListStr* wrdList, bool ignoreNewLines, bool end_approved);

wordStr* prog(wordStr* currentWord);
wordStr* prog_con(wordStr* currentWord);
wordStr* statement(wordStr* currentWord);
wordStr* opt(wordStr* currentWord);
wordStr* builtin(wordStr* currentWord);
wordStr* arg(wordStr* currentWord);
wordStr* args_more(wordStr* currentWord);
wordStr* types(wordStr* currentWord);
wordStr* params(wordStr* currentWord);
wordStr* params_more(wordStr* currentWord);
wordStr* type_spec(wordStr* currentWord);
wordStr* write_params(wordStr* currentWord);
wordStr* write_params_more(wordStr* currentWord);
wordStr* option(wordStr* currentWord);
wordStr* return_value(wordStr* currentWord);
wordStr* expression(wordStr* currentWord);
wordStr* expression_more(wordStr* currentWord);
wordStr* term(wordStr* currentWord);


//node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList) {
	wordStr* currentWord = GetFirstToken(wrdList, true, true); //GET HEADER

	//EOF Prevention
	if (currentWord == NULL) {
		printf("END of file\n");
		return;
	}

	printf("%s\n", currentWord->content);
	// *********************TUTORIAL********************
	//node* cNode;
	//cNode = CreateNode(cToken->content, "testType");
	//printf("Node = %s of type %s\n", cNode->content, cNode->type);	

	currentWord = prog_con(currentWord); //START OF SYNTAX
	
	return;
}


	//########################################
	/*
	65-70. <statement>
	*/
wordStr* statement(wordStr* currentWord) {
	printf("IN STATEMENT:%s\n", currentWord->content);

	//########################################
	/*
	65. <statement> -> <variant> ID <option> EOL <statement>
	*/
	// let || var
	if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		currentWord = GetToken(currentWord, true, false);

		//ID
		if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing identifier in variable definition in statement\n");

		//<option> SKIPFALSE
		currentWord = option(currentWord);

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after statement variant ID definition in statement\n");

		//<statement> continuing recursively
		return statement(currentWord);
	}

	//########################################
	/*
	66. <statement> -> <id_type> EOL <statement>
	*/
	else if ((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
		printf("###################IN_STATEMENT_IDTYPE#####################\n");
		/*
		29. <id_type> -> <builtin_extra> //Including write
		*/
		if((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			/*
			31. <builtin_extra> -> write(<write_params>)
			*/
			//write
			if(strcmp(currentWord->content, "write") == 0) {
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");
				//<write_params>
				printf("%s", currentWord->content);
				
				//SKIPTRUE
				currentWord = write_params(currentWord); //should return ) if success
				//if((result = write_params(currentWord))) return result;
				printf("aktualny token po vykone funkcie write:%s\n", currentWord->content);
				
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
				return statement(currentWord);
			}
			/*
			32. <builtin_extra> -> <builtin>
			*/
			else{
				//<builtin> SKIPFALSE
				currentWord = builtin(currentWord);

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing EOL after id_type (builtin case) in statement section\n");

				//OK
				return statement(currentWord);
			}



		}
		/*
		30. <id_type> -> ID <opt>
		*/
		else {
			// ID
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = GetToken(currentWord, true, false);
			}
			//<opt> SKIPFALSE
			currentWord = opt(currentWord);
			//if((result = opt(currentWord))) return result;
			printf("IDOPT\n");
			//EOL
			if(strcmp(currentWord->type, "newline") == 0) {
					printf("token pred:%s\n", currentWord->content);
					currentWord = GetToken(currentWord, true, false);
					printf("token po:%s\n", currentWord->content);
			}
			else ExitProgram(2, "Missing EOL after id_type (ID <opt>) in statement section\n");


			//OK
			return statement(currentWord);
		}
		

	}

	//########################################
	/*
	67. <statement> -> if <expression> {<statement>} else {<statement>} <statement>
	*/
	// if
	else if (strcmp(currentWord->content, "if") == 0) {
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord);
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
		currentWord = statement(currentWord);
		//if((result = prog_con(currentWord))) return result;

		printf("token:%s\n", currentWord->content);

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");

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
		currentWord = statement(currentWord);

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
		return statement(currentWord);



	}

	//########################################
	/*
	68. <statement> -> while <expression> {<statement>} EOL <statement>
	*/
	//while
	else if (strcmp(currentWord->content, "while") == 0) {
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord);
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
		currentWord = statement(currentWord);
		
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
		return statement(currentWord);
	}
	//########################################
	/*
	69. <statement> -> return <return_value> EOL <statement>
	*/
	else if(strcmp(currentWord->content, "return") == 0) {
		currentWord = GetToken(currentWord, false, false); // CAUTION

		//<return_value> SKIPFALSE
		currentWord = return_value(currentWord); // return EOL if success

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after return in statement\n");

		//<statement>
		return statement(currentWord);
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

wordStr* prog_con(wordStr* currentWord) {
	printf("IN PROG_CON\n");
	printf("currentinprogcon:%s\n", currentWord->content);
	//########################################
	/*
	1. <prog_con> -> func FID (<arg>) <types> EOL <prog_con>
	*/
	//func
	if(strcmp(currentWord->content, "func") == 0) {
		printf("<prog_con> -> func FID (<arg>) <types> {<statement>} <prog_con>");
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
		currentWord = arg(currentWord); 
		//after ARG

		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ) in function definition\n");

		//<types> SKIPFALSE
		currentWord = types(currentWord);
		//if((result = types(currentWord))) return result;

		//EOF prevention
		if (currentWord == NULL) {
			printf("END of file\n");
			return currentWord;
		}

		printf("tokenFUNC:%s\n", currentWord->content);

		//EOL
		if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after function definition in prog_con section\n");

		//<prog_con> continuing recursively
		return prog_con(currentWord);
	}

	//########################################
	/*
	2. <prog_con> -> <variant> ID <option> EOL <prog_con>
	*/
	// let || var
	else if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		currentWord = GetToken(currentWord, true, false);

		//ID
		if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = GetToken(currentWord, false, false);
		}
		else ExitProgram(2, "Missing identifier in variable definition\n");

		//<option> SKIPFALSE
		currentWord = option(currentWord); //returns newline

		//EOF Prevention
		if (currentWord == NULL) {
			printf("END of file\n");
			return currentWord;
		}

		//EOL
		if(strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing EOL after variable definition in prog_con section\n");

		//<prog_con> continuing recursively
		return prog_con(currentWord);
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
				currentWord = GetToken(currentWord, true, false);
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing ( in write function calling\n");

				//<write_params> SKIPTRUE
				printf("%s", currentWord->content);
				currentWord = write_params(currentWord);
				//if((result = write_params(currentWord))) return result;
				printf("aktualny token po vykone funkcie write:%s\n", currentWord->content);
				
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

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing EOL after id_type in statement section\n");

				//OK
				return prog_con(currentWord);
			}
			/*
			32. <builtin_extra> -> <builtin>
			*/
			else{
				//<builtin> SKIPFALSE
				currentWord = builtin(currentWord);

				//EOF Prevention
				if (currentWord == NULL) {
					printf("END of file\n");
					return currentWord;
				}

				//EOL
				if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
				}
				else ExitProgram(2, "Missing EOL after id_type (builtin case) in statement section\n");
				//if((result = builtin(currentWord))) return result;
				//OK
				return prog_con(currentWord);
			}



		}
		/*
		30. <id_type> -> ID <opt>
		*/
		else {
			// ID
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = GetToken(currentWord, true, false);
			}
			//<opt> SKIPFALSE
			currentWord = opt(currentWord);
			//if((result = opt(currentWord))) return result;

			//EOF Prevention
			if (currentWord == NULL) {
				printf("END of file\n");
				return currentWord;
			}

			//EOL
			if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = GetToken(currentWord, true, false);
			}
			else ExitProgram(2, "Missing EOL after id_type (ID <opt>) in statement section\n");

			//OK
			return prog_con(currentWord);
		}
		

	}

	//########################################
	/*
	4. <prog_con> -> if <expression> {<statement>} else {<statement>} <prog_con>
	*/
	// if
	else if (strcmp(currentWord->content, "if") == 0) {
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord);
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
		currentWord = statement(currentWord);
		//if((result = prog_con(currentWord))) return result;

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing } in if after statement\n");

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
		currentWord = statement(currentWord);
		//if((result = prog_con(currentWord))) return result;

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			printf("aktual tokenhere:%s\n", currentWord->type);
			currentWord = GetToken(currentWord, false, true);
			//printf("aktual tokenhere:%s\n", currentWord->type);
		}
		else ExitProgram(2, "Missing } in if after statement\n");

		//EOF Prevention
		if (currentWord == NULL) {
			printf("END of file\n");
			return currentWord;
		}
		
		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
			printf("aktual token:%s\n", currentWord->type);
		}

		else ExitProgram(2, "Missing newline after if statement to border new command\n");
		//<prog_con>
		return prog_con(currentWord);



	}

	//########################################
	/*
	5. <prog_con> -> while <expression> {<statement>} <prog_con>
	*/
	//while
	else if (strcmp(currentWord->content, "while") == 0) {
		currentWord = GetToken(currentWord, true, false);

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		// <expression> SKIPTRUE
		currentWord = expression(currentWord);
		
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
		currentWord = statement(currentWord);
		//if((result = prog_con(currentWord))) return result;

		//}				
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = GetToken(currentWord, false, true);
		}
		else ExitProgram(2, "Missing } after while statement to border new command\n");

		//EOF Prevention
		if (currentWord == NULL) {
			printf("END of file\n");
			return currentWord;
		}

		//EOL
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		
		//<prog_con>
		return prog_con(currentWord);
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

wordStr* opt(wordStr* currentWord) {
	printf("###################IN_OPT#####################\n");
	//########################################
	/*
	*48. <opt> -> (<params>)
	*/
	//(
	if(strcmp(currentWord->content, "(") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<params> SKIPTRUE
		currentWord = params(currentWord);//returns )

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
			currentWord = builtin(currentWord);
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
				currentWord = params(currentWord);
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
				currentWord = expression_more(currentWord); //have to return token after EOL if correct

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
			currentWord = GetToken(currentWord, true, false);

			//<expression_more> SKIPFALSE
			currentWord = expression_more(currentWord); //return token after EOL

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
wordStr* builtin(wordStr* currentWord) {
	
	printf("###################IN_BUILTIN#####################:%s\n", currentWord->content);
	//########################################
	/*
	*37. <builtin> -> readString ()
	*/
	//readString
	if(strcmp(currentWord->content, "readString") == 0) {
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
		printf("in readINT\n");
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in Int2Double function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in Double2Int function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in length function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in substring function\n");

		//of
		if(strcmp(currentWord->content, "of") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing of in substring function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing parameters length_t in substring function\n");
		//,
		if(strcmp(currentWord->content, ",") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing , in substring function after of\n");

		//startingAt
		if(strcmp(currentWord->content, "startingAt") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing startingAt in substring function\n");
		//<chr_t>
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
		if(strcmp(currentWord->content, "endingBefore") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing endingBefore in substring function\n");
		//<chr_t>
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in ord function\n");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
		currentWord = GetToken(currentWord, true, false);
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing ( in chr function\n");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
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
wordStr* arg(wordStr* currentWord) {
	//int result;
	printf("###################IN_ARG#####################\n");
	//########################################
	/*
	*8. <arg> -> <param_name> ID : <type_spec> <args_more>
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
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//<args_more> SKIPTRUE
		currentWord = args_more(currentWord);
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
wordStr* args_more(wordStr* currentWord) {
	//int result;
	printf("###################IN_ARGS_MORE#####################\n");
	/*
	10. <args_more> -> , PARAM_NAME ID : <type_spec> <args_more>
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
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//<args_more>
		return args_more(currentWord);
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

wordStr* types(wordStr* currentWord) {
	//int result;
	printf("###################IN_TYPES#####################\n");
	/*
	12. <types> -> -> <type_spec> {statement_with_return} 
	*/
	// ->
	if (strcmp(currentWord->content, "->") == 0) {
		currentWord = GetToken(currentWord, true, false);
		//<type_spec> SKIPTRUE
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//{
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing { in function definition\n");

		//<statement_with_return> SKIPTRUE
		currentWord = statement(currentWord); //OVERWRITE to STATEMENT
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
	13. <types> -> {<statement_with_return>}
	*/
	//{
	else if(strcmp(currentWord->content, "{") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<statement_with_return> SKIPTRUE
		currentWord = statement(currentWord);
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
<params> rule implementation CHANGE RULES
*/
//<params>
wordStr* params(wordStr* currentWord) {
	printf("###################IN_PARAMS#####################\n");
	//ID
	if (strcmp(currentWord->type, "identifier") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//whether : || <params_more>
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = GetToken(currentWord, true, false);

			//<term>
			currentWord = term(currentWord);
			//if((result = term(currentWord))) return result;

			//<params_more> SKIPTRUE
			currentWord = params_more(currentWord);
			//if((result = params_more(currentWord))) return result;


		}
		//<params_more>
		else {
			//<params_more> SKIPTRUE
			currentWord = params_more(currentWord);
			//if((result = params_more(currentWord))) return result;
		}

		//OK
		return currentWord;
	}
	//<term> without ID
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
		currentWord = GetToken(currentWord, true, false);
		//<params_more> SKIPTRUE
		currentWord = params_more(currentWord);
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
<params_more> rule implementation CHANGE RULES
*/
wordStr* params_more(wordStr* currentWord) {
	//int result;
	printf("###################IN_PARAMS_MORE#####################\n");

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
				currentWord = term(currentWord);
				//if((result = term(currentWord))) return result;

				//<params_more>
				return params_more(currentWord);


			}
			//<params_more>
			else {
				return params_more(currentWord);
			}

			//OK
			return currentWord;
		}
		//<term> without ID
		else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			currentWord = GetToken(currentWord, true, false);
			//<params_more> SKIPTRUE
			return params_more(currentWord);
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
wordStr* type_spec(wordStr* currentWord) {
	
	printf("###################IN_TYPE_SPEC#####################\n");
	//14.-19. <type_spec>
	if (strcmp(currentWord->content, "Double")) {
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int")) {
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String")) {
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Double?")) {
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "Int?")) {
		currentWord = GetToken(currentWord, false, true);
	}
	else if(strcmp(currentWord->content, "String?")) {
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
wordStr* write_params(wordStr* currentWord) {
	//int result;
	printf("###################IN_WRITE_PARAMS#####################\n");
	/*
	33. <write_params> -> <term> <write_params_more>
	*/
	//<term>
	if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
		currentWord = GetToken(currentWord, true, false);
		printf("tu som:%s\n", currentWord->content);
		//<write_params_more> SKIPTRUE
		currentWord = write_params_more(currentWord); // should return )
		//if((result = write_params_more(currentWord))) return result;
		printf("vraciam sa s:%s\n", currentWord->content);
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
wordStr* write_params_more(wordStr* currentWord) {
	
	printf("###################IN_WRITE_PARAMS_MORE#####################\n");
	/*
	35. <write_params_more> -> , <term> <write_params_more>
	*/
	//,
	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<term>
		if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
			currentWord = GetToken(currentWord, true, false);
		}
		else ExitProgram(2, "Missing term in write_params_more part\n");
		//<write_params_more>
		return write_params_more(currentWord);
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
<option> rule implementation
*/
wordStr* option(wordStr* currentWord) {
	//int result;
	printf("###################IN_OPTION#####################\n");
	/*
	22. <option> -> : <type_spec> <assigns>
	*/
	//:
	if(strcmp(currentWord->content, ":") == 0) {
		currentWord = GetToken(currentWord, true, false);

		//<type_spec> SKIPFALSE bc after typespec can be EOL
		currentWord = type_spec(currentWord);
		
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
			currentWord = builtin(currentWord); //return token after )
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
				currentWord = params(currentWord);
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
				currentWord = term(currentWord);

				//<expression_more> SKIPFALSE
				currentWord = expression_more(currentWord); // return newline if success
				
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
			currentWord = GetToken(currentWord, false, true);
			
			//EOF Prevention
			if (currentWord == NULL) {
				printf("END of file\n");
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
				currentWord = term(currentWord);

				//<expression_more> SKIPFALSE
				currentWord = expression_more(currentWord); // return newline if success
				
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
			currentWord = builtin(currentWord);
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
				currentWord = params(currentWord);
				
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
				currentWord = term(currentWord);

				//<expression_more>
				currentWord = expression_more(currentWord);

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
wordStr* return_value(wordStr* currentWord) {
	printf("###################IN_RETURN_VALUE#####################\n");
	
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
			currentWord = expression_more(currentWord); // will return EOL if success

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
		currentWord = expression(currentWord);
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
wordStr* expression(wordStr* currentWord) {
	//int result;
	printf("###################IN_EXPRESSION#####################\n");
	/*
	81. <expression> -> <term> <sign> <term> <expression_more>
	*/

	//<term> SKIPFALSE bc idtype
	currentWord = term(currentWord);

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		currentWord = GetToken(currentWord, true, false);
	}
	// for example "let a" can be expression too
	else {
		return currentWord;
	}
	//<term> SKIPFALSE
	currentWord = term(currentWord);
	//if((result = term(currentWord))) return result;

	//<expression_more>
	currentWord = expression_more(currentWord);
	//if((result = expression_more(currentWord))) return result; //getting token after EOL from expression_more

	//OK
	return currentWord;
}

//########################################
/*
<expression_more> rule implementation
*/
wordStr* expression_more(wordStr* currentWord) {
	//int result;
	printf("###################IN_EXPRESSION_MORE#####################\n");
	/*
	81. <expression_more> -> <sign> <term> <expression_more> EOL
	*/

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		currentWord = GetToken(currentWord, true, false);

		//<term> SKIPFALSE
		currentWord = term(currentWord);
		//if((result = term(currentWord))) return result;

		//<expression_more> continuing recursively
		return expression_more(currentWord);
	}
	//EOL
	else if ((strcmp(currentWord->type, "newline") == 0) || (strcmp(currentWord->content, ")") == 0)  || (strcmp(currentWord->content, "{") == 0)) {
		//OK
		printf("tusom\n");
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
wordStr* term(wordStr* currentWord) {
	
	printf("###################IN_TERM#####################\n");
	//"STRINGLIT" || INTLIT || DOUBLELIT || ID
	if(strcmp(currentWord->type, "string") == 0) {
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "integer") == 0) {
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "float") == 0) {
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if(strcmp(currentWord->type, "identifier") == 0) {
		currentWord = GetToken(currentWord, false, true);
		return currentWord;
	}
	else if((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		currentWord = GetToken(currentWord, true, false);

		if(strcmp(currentWord->type, "identifier") == 0) {
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
