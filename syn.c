#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

void printTree(node* root, unsigned level);
void BindNode(node* parent, node* child, char* direction);
node* CreateNode(char* content, char* type, unsigned* level);

wordStr* prog(wordStr* currentWord);
wordStr* prog_con(wordStr* currentWord);
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
wordStr* expression(wordStr* currentWord);
wordStr* expression_more(wordStr* currentWord);
wordStr* term(wordStr* currentWord);


//node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList) {
	wordStr* currentWord = wrdList->first; //GET HEADER
	printf("%s\n", currentWord->content);
	//node* cNode;
	//cNode = CreateNode(cToken->content, "testType");
	//printf("Node = %s of type %s\n", cNode->content, cNode->type);
		

	//while(currentWord != NULL)
	
		//current_word->content
		//current_word->type
		//current_word->next
		// *********************TUTORIAL********************
		//else, func, if, let, return, var, while => keyword
		//Double, Int, String, ???nil??? => index
		// ID, FID => identifier





		//printf("%s\n",currentWord->content);
		//currentWord = currentWord->next;
		currentWord = prog(currentWord); //START OF SYNTAX
	
	return;
}



	//########################################
	/*
	1. <prog> -> HEADER <prog_con>
	*/
wordStr* prog(wordStr* currentWord) {
	printf("IN PROG\n");
	//HEADER
	if((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, ".IFJcode23") == 0)) {
		printf("<prog> -> HEADER <prog_con>\n");
		currentWord = currentWord->next;
		//<prog_con>
		currentWord = prog_con(currentWord);
		//if ((result = prog_con(currentWord))) return result;
		
	}
	else ExitProgram(2, "Missing Header");
	//DELETE currentWord = prog_con(currentWord);

	return currentWord;
}

wordStr* prog_con(wordStr* currentWord) {
	printf("IN PROG_CON\n");

	//########################################
	/*
	2. <prog_con> -> func FID (<arg>) <types> <prog_con>
	*/
	//func
	if(strcmp(currentWord->content, "func") == 0) {
		printf("<prog_con> -> func FID (<arg>) <types> {<statement>} <prog_con>");
		currentWord = currentWord->next;

		//FID
		if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing FID");
	

		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in function definition");

		//<arg>
		currentWord = arg(currentWord); 
		//after ARG

		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in function definition");
		

		//<types>
		currentWord = types(currentWord);
		//if((result = types(currentWord))) return result;

		//<prog_con> continuing recursively
		return prog_con(currentWord);
	}

	//########################################
	/*
	3. <prog_con> -> <variant> ID <option> <prog_con>
	*/
	// let || var
	if ((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
			currentWord = currentWord->next;

			//ID
			if(strcmp(currentWord->type, "identifier") == 0) {
				currentWord = currentWord->next;
			}
			else ExitProgram(2, "Missing identifier in variable definition");

			//<option>
			currentWord = option(currentWord);
			//if((result = option(currentWord))) return result;

			//<prog_con> continuing recursively
			return prog_con(currentWord);
	}

	//########################################
	/*
	4. <prog_con> -> <id_type> <prog_con>
	*/
	if ((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
		/*
		29. <id_type> -> <builtin_extra> //Including write
		*/
		if((strcmp(currentWord->content, "write") == 0) || (strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {

			/*
			31. <builtin_extra> -> write(<write_params>)
			*/
			//write
			if(strcmp(currentWord->content, "write") == 0) {
				currentWord = currentWord->next;
				//(
				if (strcmp(currentWord->content, "(") == 0) {
					currentWord = currentWord->next;
				}
				else ExitProgram(2, "Missing ( in write function calling");
				//<write_params>
				printf("%s", currentWord->content);
				currentWord = write_params(currentWord);
				//if((result = write_params(currentWord))) return result;
				printf("aktualny token po vykone funkcie write:%s\n", currentWord->content);
				
				//)
				if (strcmp(currentWord->content, ")") == 0) {
					currentWord = currentWord->next;
				}
				else ExitProgram(2, "Missing ) in write function calling");
				//OK
				return currentWord;
			}
			/*
			32. <builtin_extra> -> <builtin>
			*/
			else{
				//<builtin>
				currentWord = builtin(currentWord);
				//if((result = builtin(currentWord))) return result;
				//OK
				return currentWord;
			}



		}
		/*
		30. <id_type> -> ID <opt>
		*/
		else {
			// ID
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = currentWord->next;
			}
			//<opt>
			currentWord = opt(currentWord);
			//if((result = opt(currentWord))) return result;

			//OK
			return currentWord;
		}
		

	}

	//########################################
	/*
	5. <prog_con> -> if <expression> {<statement>} else {<statement>} <prog_con>
	*/
	// if
	if (strcmp(currentWord->content, "if") == 0) {
		currentWord = currentWord->next;

		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		// <expression>
		currentWord = expression(currentWord);
		//if((result = expression(currentWord))) return result;
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing { in if before statement");

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/

		//<statement> FOR TESTING just PROG_CON instead of statement OVERWRITE
		currentWord = prog_con(currentWord); //OVERWRITE to STATEMENT
		//if((result = prog_con(currentWord))) return result;

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing } in if after statement");

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/

		// else
		if (strcmp(currentWord->content, "else") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing keyword else in if section");

		// {
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing { in if before else statement");

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/

		//<statement> FOR TESTING just PROG_CON instead of statement OVERWRITE
		currentWord = prog_con(currentWord); // OVERWRITE for STATEMENT
		//if((result = prog_con(currentWord))) return result;

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/

		// }
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing } in if after statement");

		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing newline after if statement to border new command");*/
		//<prog_con>
		return prog_con(currentWord);



	}

	//########################################
	/*
	6. <prog_con> -> while <expression> {<statement>} <prog_con>
	*/
	//while
	if (strcmp(currentWord->content, "while") == 0) {
		// (
		if (strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		// <expression>
		currentWord = expression(currentWord);
		//if((result = expression(currentWord))) return result;
		// )
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		//{				
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing { before while statement to border new command");
		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/
		//<statement> FOR TESTING just PROG_CON instead of statement OVERWRITE
		currentWord = prog_con(currentWord); // OVERWRITE to STATEMENT
		//if((result = prog_con(currentWord))) return result;
		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/
		//}				
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing } after while statement to border new command");
		/*if newline is there OVERWRITE
		if (strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
		}*/
		
		//<prog_con>
		return prog_con(currentWord);
	}

	else {
		printf("Whether END OF FILE or FUCK UP");
		return currentWord;
	}
	
	return currentWord;
}

wordStr* opt(wordStr* currentWord) {
	//int result;
	printf("###################IN_OPT#####################");
	//########################################
	/*
	*48. <opt> -> (<params>)
	*/
	//(
	if(strcmp(currentWord->content, "(") == 0) {
		currentWord = currentWord->next;
		//<params>
		currentWord = params(currentWord);

		//)
		if (strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in rule 4 in opt (<params>)");

		//OK
		return currentWord;
	}
	/*
	49.-52. FOR NOW, WILL OVERWRITE
	*/
	else if(strcmp(currentWord->content, "=") == 0) {
		currentWord = currentWord->next;

		/*
		51. = <builtin>
		*/
		//<builtin> WITHOUT WRITE readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
			//<builtin>
			currentWord = builtin(currentWord);
			//OK
			return currentWord;
		}

		else if(strcmp(currentWord->type, "identifier") == 0) {
			
			/*
			49. <opt> -> = ID EOL
			*/
			// EOL
			if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = currentWord->next;
				//OK
				return currentWord;
			}
			/*
			50. <opt> -> = ID (<params>)
			*/
			//(
			else if(strcmp(currentWord->content, "(") == 0) {
				currentWord = currentWord->next;
				//<params>
				currentWord = params(currentWord);
				//)
				if(strcmp(currentWord->content, ")") == 0) {
					currentWord = currentWord->next;
				}
				//OK
				return currentWord;
			}

			/*
			part of 52. if first is operand is ID not expr
			*/
			// <sign> after ID
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = expression_more(currentWord); //have to return token after EOL if correct

				//EOL already checked in exp_more
				//OK
				return currentWord;
			}

			else ExitProgram(2, "Wrong token after ID token in opt");

		}

		 /*
		  another part of 52. <term> excluding ID as term
		 */
		else if ((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			currentWord = currentWord->next;

			//<expression_more>
			currentWord = expression_more(currentWord); //return token after EOL

			//OK
			return currentWord;
		}

		else ExitProgram(2, "Wrong expression after = in opt");


		return currentWord;
	}

	else ExitProgram(2, "Wrong expression after ID in ID_TYPE");

	/* EOL
	if (strcmp(currentWord->type, "newline") == 0) {
		currentWord = currentWord->next;
	}*/

	//OK
	return currentWord;
}

//########################################
/*
<builtin> rule implementation
*/
wordStr* builtin(wordStr* currentWord) {
	
	printf("###################IN_BUILTIN#####################");
	//########################################
	/*
	*37. <builtin> -> readString ()
	*/
	//readString
	if(strcmp(currentWord->content, "readString") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in readString function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in readString function");
		//OK
		return currentWord;
	}
	//readInt
	else if(strcmp(currentWord->content, "readInt") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in readInt function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in readInt function");
		//OK
		return currentWord;
	}
	//readDouble
	else if(strcmp(currentWord->content, "readDouble") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in readDouble function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in readDouble function");
		//OK
		return currentWord;
	}
	//Int2Double
	else if(strcmp(currentWord->content, "Int2Double") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in Int2Double function");
		//<chr_t>
		if((strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters chr_t in Int2Double function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in Int2Double function");
		//OK
		return currentWord;
	}
	//Double2Int
	else if(strcmp(currentWord->content, "Double2Int") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in Double2Int function");
		//<chr_t>
		if((strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters double_t in Double2Int function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in Double2Int function");
		//OK
		return currentWord;
	}
	//length
	else if(strcmp(currentWord->content, "length") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in length function");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters length_t in length function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in length function");
		//OK
		return currentWord;
	}
	//substring
	else if(strcmp(currentWord->content, "substring") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in substring function");

		//of
		if(strcmp(currentWord->content, "of") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing of in substring function");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters length_t in substring function");
		//,
		if(strcmp(currentWord->content, ",") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing , in substring function after of");

		//startingAt
		if(strcmp(currentWord->content, "startingAt") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing startingAt in substring function");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters chr_t in substring function");
		//,
		if(strcmp(currentWord->content, ",") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing , in substring function after startingAt");

		//endingBefore
		if(strcmp(currentWord->content, "endingBefore") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing endingBefore in substring function");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters chr_t in substring function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in length function");
		//OK
		return currentWord;
	}
	//ord
	else if(strcmp(currentWord->content, "ord") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in ord function");
		//<length_t>
		if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters length_t in ord function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in ord function");
		//OK
		return currentWord;
	}
	//chr
	else if(strcmp(currentWord->content, "chr") == 0) {
		currentWord = currentWord->next;
		//(
		if(strcmp(currentWord->content, "(") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ( in chr function");
		//<chr_t>
		if((strcmp(currentWord->type, "integer") == 0) || (strcmp(currentWord->type, "identifier") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing parameters chr_t in chr function");
		//)
		if(strcmp(currentWord->content, ")") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ) in chr function");
		//OK
		return currentWord;
	}
	else ExitProgram(2, "Wrong builtin name");


	
	return currentWord;
}

//########################################
/*
<arg> rule implementation
*/
wordStr* arg(wordStr* currentWord) {
	//int result;
	printf("###################IN_ARG#####################");
	//########################################
	/*
	*8. <arg> -> <param_name> ID : <type_spec> <args_more>
	*/
	//<param_name> NOTE: INCLUDING _
	if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {

		char* tmp_id = currentWord->content; // param_name cannot be same as ID name
		currentWord = currentWord->next;
		
		//ID
		if ((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, tmp_id) != 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ID in arg or id has same name as param_name");
		
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing : in arg");
		
		//<type_spec>
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//<args_more>
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
	else ExitProgram(2, "Missing param_name in arg");

	return currentWord;
}

//########################################
/*
<args_more> rule implementation
*/
wordStr* args_more(wordStr* currentWord) {
	//int result;
	printf("###################IN_ARGS_MORE#####################");
	/*
	10. <args_more> -> , PARAM_NAME ID : <type_spec> <args_more>
	*/
	//,
	if (strcmp(currentWord->content, ",") == 0) {
		currentWord = currentWord->next;

		char* tmp_id = currentWord->content; // for comparing param_name with id

		//PARAM_NAME
		if ((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->content, "_") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing param_name");
		
		//ID
		if ((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, tmp_id) != 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing ID in args_more or id has same name as param_name");

		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing , in args_more");

		//<type_spec>
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
	else ExitProgram(2, "Missing , in args_more");

	return currentWord;
}

wordStr* types(wordStr* currentWord) {
	//int result;
	printf("###################IN_TYPES#####################");
	/*
	12. <types> -> -> <type_spec> {statement_with_return} 
	*/
	// ->
	if (strcmp(currentWord->content, "->") == 0) {
		currentWord = currentWord->next;
		//<type_spec>
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//{
		if (strcmp(currentWord->content, "{") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing { in function definition");

		//<statement_with_return> OVERWRITE FROM PROGCON TO STATEMENTWITHRETURN
		currentWord = prog_con(currentWord); //OVERWRITE to STATEMENT
		//if((result = prog_con(currentWord))) return result;
		//after <statement_with_return>

		//}
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing } in function definition");

		//OK
		return currentWord;
	}

	/*
	13. <types> -> {<statement_with_return>}
	*/
	//{
	else if(strcmp(currentWord->content, "{") == 0) {
		currentWord = currentWord->next;

		//<statement_with_return> OVERWRITE FROM PROG_CON TO STATEMENTWITHRETURN
		currentWord = prog_con(currentWord);
		//if((result = prog_con(currentWord))) return result;
		//after <statement_with_return>

		//}
		if (strcmp(currentWord->content, "}") == 0) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing } in types rule, func definition");

		//OK
		return currentWord;
	}

	//NOT OKAY
	else ExitProgram(2, "missing -> or { in function definition");

	return currentWord;
}

//########################################
/*
<params> rule implementation CHANGE RULES
*/
//<params>
wordStr* params(wordStr* currentWord) {
	//int result;
	printf("###################IN_PARAMS#####################");
	//ID
	if (strcmp(currentWord->type, "identifier") == 0) {
		currentWord = currentWord->next;

		//whether : || <params_more>
		//:
		if (strcmp(currentWord->content, ":") == 0) {
			currentWord = currentWord->next;

			//<term>
			currentWord = term(currentWord);
			//if((result = term(currentWord))) return result;

			//<params_more>
			currentWord = params_more(currentWord);
			//if((result = params_more(currentWord))) return result;


		}
		//<params_more>
		else {
			currentWord = params_more(currentWord);
			//if((result = params_more(currentWord))) return result;
		}

		//OK
		return currentWord;
	}
	//<term> without ID
	else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0)) {
		currentWord = currentWord->next;
		//<params_more>
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
	else ExitProgram(2, "Wrong parameter in called function");


	return currentWord;
}

//########################################
/*
<params_more> rule implementation CHANGE RULES
*/
wordStr* params_more(wordStr* currentWord) {
	//int result;
	printf("###################IN_PARAMS_MORE#####################");

	if(strcmp(currentWord->content, ",") == 0) {
		//ID
		if (strcmp(currentWord->type, "identifier") == 0) {
			currentWord = currentWord->next;

			//whether : || <params_more>
			//:
			if (strcmp(currentWord->content, ":") == 0) {
				currentWord = currentWord->next;

				//<term>
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
		else if((strcmp(currentWord->type, "string") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0)) {
			currentWord = currentWord->next;
			//<params_more>
			return params_more(currentWord);
			//OK
			return currentWord;
		}
		//NOT OK
		else ExitProgram(2, "Wrong parameter after , in params_more");
		
	}
	/*
	58. <params_more> -> eps
	*/
	else if(strcmp(currentWord->content, ")") == 0) {
		return currentWord;
	}
	else ExitProgram(2, "Missing ',' or wrong parameter in params_more");

	return currentWord;
}

//########################################
/*
<type_spec> rule implementation
*/
wordStr* type_spec(wordStr* currentWord) {
	
	printf("###################IN_TYPE_SPEC#####################");
	//14.-19. <type_spec>
	if (strcmp(currentWord->content, "Double")) {
		currentWord = currentWord->next;
	}
	else if(strcmp(currentWord->content, "Int")) {
		currentWord = currentWord->next;
	}
	else if(strcmp(currentWord->content, "String")) {
		currentWord = currentWord->next;
	}
	else if(strcmp(currentWord->content, "Double?")) {
		currentWord = currentWord->next;
	}
	else if(strcmp(currentWord->content, "Int?")) {
		currentWord = currentWord->next;
	}
	else if(strcmp(currentWord->content, "String?")) {
		currentWord = currentWord->next;
	}
	//missing TYPE_SPEC
	else ExitProgram(2, "Missing type_spec");

	//OK
	return currentWord;
}

//########################################
/*
<write_params> rule implementation
*/
wordStr* write_params(wordStr* currentWord) {
	//int result;
	printf("###################IN_WRITE_PARAMS#####################");
	/*
	33. <write_params> -> <term> <write_params_more>
	*/
	//<term>
	if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
		currentWord = currentWord->next;
		printf("tu som:%s\n", currentWord->content);
		//<write_params_more>
		currentWord = write_params_more(currentWord);
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
	else ExitProgram(2, "Missing write params or ) in write function calling");	

	return currentWord;
}

//########################################
/*
<write_params_more> rule implementation
*/
wordStr* write_params_more(wordStr* currentWord) {
	
	printf("###################IN_WRITE_PARAMS_MORE#####################");
	/*
	35. <write_params_more> -> , <term> <write_params_more>
	*/
	//,
	if(strcmp(currentWord->content, ",") == 0) {
		currentWord = currentWord->next;

		//<term>
		if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
			currentWord = currentWord->next;
		}
		else ExitProgram(2, "Missing term in write_params_more part");
		//<write_params_more>
		return write_params_more(currentWord);
	}
	/*
	36. <write_params_more> -> eps
	*/
	//)
	else if(strcmp(currentWord->content, ")") == 0) {
		//OK
		printf("tu som:%s", currentWord->content);
		currentWord = currentWord->next;
		printf("mam sa vratit s:%s\n", currentWord->content);
		return currentWord;
	}
	else ExitProgram(2, "Wrong parameter in write_params_more part");

	return currentWord;
}

//########################################
/*
<option> rule implementation
*/
wordStr* option(wordStr* currentWord) {
	//int result;
	printf("###################IN_OPTION#####################");
	/*
	22. <option> -> : <type_spec> <assigns>
	*/
	//:
	if(strcmp(currentWord->content, ":") == 0) {
		currentWord = currentWord->next;

		//<type_spec>
		currentWord = type_spec(currentWord);
		//if((result = type_spec(currentWord))) return result;

		//<assigns>
		//24. <assigns> -> =
		//=
		if(strcmp(currentWord->content, "=") == 0) {
			currentWord = currentWord->next;
		}
		//25. <assigns> -> eps EOL
		//EOL
		else if(strcmp(currentWord->type, "newline") == 0) {
			currentWord = currentWord->next;
			//OK
			return currentWord;
		}
		else ExitProgram(2, "Missing = in variable definition");

		//whether <builtin> OR ID<opt> OR <expression>

		//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
			currentWord = currentWord->next;

			//into <builtin>
			currentWord = builtin(currentWord);
			//if((result = builtin(currentWord))) return result;

			//OK
			return currentWord;
		}	
		//ID
		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = currentWord->next;

			//(
			if(strcmp(currentWord->content, "(") == 0) {
				currentWord = currentWord->next;

				//<params>
				currentWord = params(currentWord);
				//if((result = params(currentWord))) return result;
				//)
				if (strcmp(currentWord->content, ")")) {
					currentWord = currentWord->next;
				}
				else ExitProgram(2, "Missing ) in variable definition near called function params");
				//OK
				return currentWord;
			}
			//ID in the beginning of expression <expression>
			//<sign>
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = currentWord->next;

				//<term>
				currentWord = term(currentWord);
				//if((result = term(currentWord))) return result;

				//<expression_more>
				currentWord = expression_more(currentWord);
				//if((result = expression_more(currentWord))) return result;

				//OK
				return currentWord;
			}
			// EOL
			else if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = currentWord->next;
				//OK
				return currentWord;
			}
			else ExitProgram(2, "Missing ( or sign after ID in typed definition of variable");
		
			return currentWord;
		}


	}
	/*
	23. <option> -> <assign>
	*/
	else if(strcmp(currentWord->content, "=") == 0) {
		currentWord = currentWord->next;
		//<assign> -> = <expression>
		//<assign> -> = <builtin>
		//<assign> -> = ID <preopt>
		//=

		//whether <builtin> OR ID<opt> OR <expression>

		//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
		if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
			//currentWord = currentWord->next;

			//into <builtin>
			currentWord = builtin(currentWord);
			//if((result = builtin(currentWord))) return result;

			//OK
			return currentWord;
		}	
		//ID
		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = currentWord->next;
			/*
			46. <preopt> -> (<params>)
			*/
			//(
			if(strcmp(currentWord->content, "(") == 0) {
				currentWord = currentWord->next;

				//<params>
				currentWord = params(currentWord);
				//if((result = params(currentWord))) return result;
				//)
				if (strcmp(currentWord->content, ")")) {
					currentWord = currentWord->next;
				}
				else ExitProgram(2, "Missing ) in variable definition near called function params");
				//OK
				return currentWord;
			}
			//ID in the beginning of expression <expression>
			//<sign>
			else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
				currentWord = currentWord->next;

				//<term>
				currentWord = term(currentWord);
				//if((result = term(currentWord))) return result;

				//<expression_more>
				currentWord = expression_more(currentWord);
				//if((result = expression_more(currentWord))) return result;

				//OK
				return currentWord;
			}
			/*
			47. <preopt> -> eps EOL
			*/
			//EOL
			else if(strcmp(currentWord->type, "newline") == 0) {
				currentWord = currentWord->next;
				return currentWord;
				//MAYBE RETURN CURRENTWORD?
			}
			else ExitProgram(2, "Missing ID or function or builtin func after = in option");
		}
	}

	//if there is jsut let a || var a
	else if(strcmp(currentWord->type, "newline") == 0) {
		currentWord = currentWord->next; //MAYBE CHECK
		return currentWord;
	}

	else ExitProgram(2, "There is no : or = after ID in prog_con variable definition beginning");

	return currentWord;
}

//########################################
/*
<expression> rule implementation
*/
wordStr* expression(wordStr* currentWord) {
	//int result;
	printf("###################IN_EXPRESSION#####################");
	/*
	81. <expression> -> <term> <sign> <term> <expression_more>
	*/
	if((strcmp(currentWord->content, "let") == 0) || (strcmp(currentWord->content, "var") == 0)) {
		currentWord = currentWord->next;
	}

	//<term>
	currentWord = term(currentWord);
	//if((result = term(currentWord))) return result;

	/*can be EOL
	if(strcmp(currentWord->type, "newline") == 0) {	//OVERWRITE
		currentWord = currentWord->next;
		return 0;
	}*/

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		currentWord = currentWord->next;
	}
	else ExitProgram(2, "Missing sign in expression");
	//<term>
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
	printf("###################IN_EXPRESSION_MORE#####################");
	/*
	81. <expression_more> -> <sign> <term> <expression_more> EOL
	*/

	//<sign>
	if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
		currentWord = currentWord->next;

		//<term>
		currentWord = term(currentWord);
		//if((result = term(currentWord))) return result;

		//<expression_more> continuing recursively
		return expression_more(currentWord);
	}
	//EOL
	else if (strcmp(currentWord->content, "newline") == 0) {
		currentWord = currentWord->next;
		//OK
		return currentWord;
	}
	else ExitProgram(2, "Missing newline or sign in expression (expression_more)");

	//OK
	return currentWord;
}

//########################################
/*
<term> rule implementation
*/
wordStr* term(wordStr* currentWord) {
	
	printf("###################IN_TERM#####################");
	//"STRINGLIT" || INTLIT || DOUBLELIT || ID
	if(strcmp(currentWord->type, "string") == 0) {
		currentWord = currentWord->next;
		return currentWord;
	}
	else if(strcmp(currentWord->type, "int") == 0) {
		currentWord = currentWord->next;
		return currentWord;
	}
	else if(strcmp(currentWord->type, "float") == 0) {
		currentWord = currentWord->next;
		return currentWord;
	}
	else if(strcmp(currentWord->type, "identifier") == 0) {
		currentWord = currentWord->next;
		return currentWord;
	}
	else ExitProgram(2, "Wrong term");

	return currentWord;
}




/*
NODE CREATENODE FUNC
*/
void printTree(node* root, unsigned level) {
	node* currentNode = root;

	for (int i = 0; i < level; i++)
		printf(i == level - 1 ? "|-" : "  ");

	if(strcmp(currentNode->type,"string") == 0)
		printf("'TESTSTRING' of type '%s'\n", currentNode->type);
	else
		printf("'%s' of type '%s'\n", currentNode->content, currentNode->type);
	if (currentNode->left != NULL)
		printTree(currentNode->left, (level + 1));
	if (currentNode->right != NULL)
		printTree(currentNode->right, (level + 1) );
}

node* CreateNode(char* content, char* type, unsigned* level) {
	node* newNode = malloc(sizeof(node));
	newNode->type = malloc(sizeof(char) * (strlen(type) + 1));
	newNode->content = malloc(sizeof(char) * (strlen(content) + 1));
	newNode->level = malloc(sizeof(unsigned));
	memcpy(newNode->level, level, sizeof(unsigned));
	
	strcpy(newNode->type, type);
	strcpy(newNode->content, content);
	newNode->right = NULL;
	newNode->left = NULL;

	return newNode;
}

void BindNode(node* parent, node* child, char* direction) {
	child->parent = parent;

	if (strcmp(direction, "left") == 0) {
		parent->left = child;
	}
	else if (strcmp(direction, "right") == 0) {
		parent->right = child;
	}
}