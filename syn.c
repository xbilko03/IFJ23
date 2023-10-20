#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>

#define maxCommandTokenCount 255

node* CreateNode(char* content, char* type);

void PerformSyntax(wordListStr* wrdList) {
	wordStr* currentWord = wrdList->first; //GET HEADER
	//node* cNode;
	//cNode = CreateNode(cToken->content, "testType");
	//printf("Node = %s of type %s\n", cNode->content, cNode->type);
	//########################################
	/*
	1. <prog> -> HEADER <prog_con>
	*/
	int prog(wordStr* currentWord) {
		int result;
		printf("IN PROG\n");
		//HEADER
		if((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->content, ".IFJcode23") == 0)) {
			printf("<prog> -> HEADER <prog_con>\n");
			currentWord = currentWord->next;
			//<prog_con>
			if ((result = prog_con(currentWord))) return result;
			
		}
		else ExitProgram(2, "Missing Header");
		

		return 0;
	}

	int prog_con(wordStr* currentWord) {
		int result;
		printf("IN PROG_CON\n");

		//########################################
		/*
		2. <prog_con> -> func FID (<arg>) <types> <prog_con>
		*/
		//func
		if((currentWord->type == keyword) && (strcmp(currentWord->content, "func") == 0)) {
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
			if((result = arg(currentWord))) return result;
			//after ARG

			//)
			if(strcmp(currentWord->content, ")") == 0) {
				currentWord = currentWord->next;
			}
			else ExitProgram(2, "Missing ) in function definition");
			

			//<types>
			if((result = types(currentWord->type))) return result;

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
				if((result = option(currentWord->type))) return result;

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
					if((result = write_params(currentWord->type))) return result;
					//)
					if (strcmp(currentWord->content, ")") == 0) {
						currentWord = currentWord->next;
					}
					else ExitProgram(2, "Missing ) in write function calling");
					//OK
					return 0;
				}
				/*
				32. <builtin_extra> -> <builtin>
				*/
				else{
					//<builtin>
					if((result = builtin(currentWord->type))) return result;
					//OK
					return 0;
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

			}
			

		}
		
		
		return 0;
	}

	//########################################
	/*
	<builtin> rule implementation
	*/
	int builtin(wordStr* currentWord) {
		int result;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
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
			return 0;
		}
		else ExitProgram(2, "Wrong builtin name");


		

	}

	//########################################
	/*
	<arg> rule implementation
	*/
	int arg(wordStr* currentWord) {
		int result;
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
			if((result = type_spec(currentWord))) return result;

			//<args_more>
			if((result = args_more(currentWord))) return result;

			//OK
			return 0;
		}
		//########################################
		/*
		*9. <arg> -> eps
		*/
		// eps
		else if(strcmp(currentWord->content, ")")) {
			return 0;
		}
		else ExitProgram(2, "Missing param_name in arg");

		
	}

	//########################################
	/*
	<args_more> rule implementation
	*/
	int args_more(wordStr* currentWord) {
		int result;
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
			if((result = type_spec(currentWord))) return result;

			//<args_more>
			return args_more(currentWord);
		}

		//11. <args_more> -> eps
		//)
		else if(strcmp(currentWord->content, ")") == 0) {
			return 0;
		}
		//NOT OKAY
		else ExitProgram(2, "Missing , in args_more");

	
	}

	int types(wordStr* currentWord) {
		int result;
		printf("###################IN_TYPES#####################");
		/*
		12. <types> -> -> <type_spec> {statement_with_return} 
		*/
		// ->
		if (strcmp(currentWord->content, "->") == 0) {
			currentWord = currentWord->next;
			//<type_spec>
			if((result = type_spec(currentWord))) return result;

			//{
			if (strcmp(currentWord->content, "{") == 0) {
				currentWord = currentWord->next;
			}
			else ExitProgram(2, "Missing { in function definition");

			//<statement_with_return>
			if((result = statement_with_return(currentWord))) return result;
			//after <statement_with_return>

			//}
			if (strcmp(currentWord->content, "}") == 0) {
				currentWord = currentWord->next;
			}
			else ExitProgram(2, "Missing } in function definition");

			//OK
			return 0;
		}

		/*
		13. <types> -> {<statement_with_return>}
		*/
		//{
		else if(strcmp(currentWord->content, "{") == 0) {
			currentWord = currentWord->next;

			//<statement_with_return>
			if((result = statement_with_return(currentWord))) return result;
			//after <statement_with_return>

			//}
			if (strcmp(currentWord->content, "}") == 0) {
				currentWord = currentWord->next;
			}

			//OK
			return 0;
		}

		//NOT OKAY
		else ExitProgram(2, "missing -> or { in function definition");
	}

	//########################################
	/*
	<type_spec> rule implementation
	*/
	int type_spec(wordStr* currentWord) {
		int result;
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
		return 0;
	}

	//########################################
	/*
	<write_params> rule implementation
	*/
	int write_params(wordStr* currentWord) {
		int result;
		printf("###################IN_WRITE_PARAMS#####################");
		/*
		33. <write_params> -> <term> <write_params_more>
		*/
		//<term>
		if((strcmp(currentWord->type, "identifier") == 0) || (strcmp(currentWord->type, "int") == 0) || (strcmp(currentWord->type, "float") == 0) || (strcmp(currentWord->type, "string") == 0)) {
			currentWord = currentWord->next;
			//<write_params_more>
			if((result = write_params_more(currentWord->type))) return result;
			//OK
			return 0;
		}
		/*
		34. <write_params> -> eps
		*/
		else if(strcmp(currentWord->content, ")")) {
			//OK
			return 0;
		}
		else ExitProgram(2, "Missing write params or ) in write function calling");	

	}

	//########################################
	/*
	<write_params_more> rule implementation
	*/
	int write_params_more(wordStr* currentWord) {
		int result;
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
			return 0;
		}
		else ExitProgram(2, "Wrong parameter in write_params_more part");

		return 0;
	}

	//########################################
	/*
	<option> rule implementation
	*/
	int option(wordStr* currentWord) {
		int result;
		printf("###################IN_OPTION#####################");
		/*
		22. <option> -> : <type_spec> <assigns>
		*/
		//:
		if (strcmp(currentWord->content, ":")) {
			currentWord = currentWord->next;

			//<type_spec>
			if((result = type_spec(currentWord))) return result;

			//<assigns> -> =
			//=
			if (strcmp(currentWord->content, "=")) {
				currentWord = currentWord->next;
			}
			else ExitProgram(2, "Missing = in variable definition");

			//whether <builtin> OR ID<opt> OR <expression>

			//<builtin> readString || readInt || readDouble || Int2Double || Double2Int || length || substring || ord || chr
			if((strcmp(currentWord->content, "readString") == 0) || (strcmp(currentWord->content, "readInt") == 0) || (strcmp(currentWord->content, "readDouble") == 0) || (strcmp(currentWord->content, "Int2Double") == 0) || (strcmp(currentWord->content, "Double2Int") == 0) || (strcmp(currentWord->content, "length") == 0) || (strcmp(currentWord->content, "substring") == 0) || (strcmp(currentWord->content, "ord") == 0) || (strcmp(currentWord->content, "chr") == 0)) {
				currentWord = currentWord->next;

				//into <builtin>
				if((result = builtin(currentWord))) return result;

				//OK
				return 0;
			}	
			//ID
			else if(strcmp(currentWord->type, "identifier") == 0) {
				currentWord = currentWord->next;

				//(
				if(strcmp(currentWord->content, "(") == 0) {
					currentWord = currentWord->next;

					//<params>
					if((result = params(currentWord))) return result;
					//)
					if (strcmp(currentWord->content, ")")) {
						currentWord = currentWord->next;
					}
					else ExitProgram(2, "Missing ) in variable definition near called function params");
					//OK
					return 0;
				}
				//ID in the beginning of expression <expression>
				//<sign>
				else if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
					currentWord = currentWord->next;

					//<term>
					if((result = term(currentWord))) return result;

					//<expression_more>
					if((result = expression_more(currentWord))) return result;

					//OK
					return 0;
				}
				// EOL
				else if(strcmp(currentWord->type, "newline") == 0) {
					currentWord = currentWord->next;
					//OK
					return 0;
				}
				else ExitProgram(2, "Missing ( or sign after ID in typed definition of variable");
			
			}


		}

	}

	//########################################
	/*
	<expression> rule implementation
	*/
	int expression(wordStr* currentWord) {
		int result;
		printf("###################IN_EXPRESSION#####################");
		/*
		81. <expression> -> <term> <sign> <term> <expression_more>
		*/
		//<term>
		if((result = term(currentWord))) return result;
		//<sign>
		if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
			currentWord = currentWord->next;
		}
		//<term>
		if((result = term(currentWord))) return result;

		//<expression_more>
		if((result = expression_more(currentWord))) return result;

		//OK
		return 0;
	}

	//########################################
	/*
	<expression_more> rule implementation
	*/
	int expression_more(wordStr* currentWord) {
		int result;
		printf("###################IN_EXPRESSION_MORE#####################");
		/*
		81. <expression_more> -> <sign> <term> <expression_more>
		*/

		//<sign>
		if((strcmp(currentWord->content, "+") == 0) || (strcmp(currentWord->content, "-") == 0) || (strcmp(currentWord->content, "*") == 0) || (strcmp(currentWord->content, "/") == 0) || (strcmp(currentWord->content, "==") == 0) || (strcmp(currentWord->content, "!=") == 0) || (strcmp(currentWord->content, "<") == 0) || (strcmp(currentWord->content, ">") == 0) || (strcmp(currentWord->content, "<=") == 0) || (strcmp(currentWord->content, ">=") == 0) || (strcmp(currentWord->content, "??") == 0)) {
			currentWord = currentWord->next;

			//<term>
			if((result = term(currentWord))) return result;

			//<expression_more> continuing recursively
			return expression_more(currentWord);
		}
		else if (strcmp(currentWord->content, "newline") == 0) {
			currentWord = currentWord->next;
			//OK
			return 0;
		}
		else ExitProgram(2, "Missing newline or sign in expression (expression_more)");

		//OK
		return 0;
	}

	//########################################
	/*
	<term> rule implementation
	*/
	int term(wordStr* currentWord) {
		int result;
		printf("###################IN_TERM#####################");
		//"STRINGLIT" || INTLIT || DOUBLELIT || ID
		if(strcmp(currentWord->type, "string") == 0) {
			currentWord = currentWord->next;
			return 0;
		}
		else if(strcmp(currentWord->type, "int") == 0) {
			currentWord = currentWord->next;
			return 0;
		}
		else if(strcmp(currentWord->type, "float") == 0) {
			currentWord = currentWord->next;
			return 0;
		}
		else if(strcmp(currentWord->type, "identifier") == 0) {
			currentWord = currentWord->next;
			return 0;
		}
		else ExitProgram(2, "Wrong term");
	}

	

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
	
	return;
}


/*
NODE CREATENODE FUNC
*/
node* CreateNode(char* content, char* type) {
	node* newNode = malloc(sizeof(node));
	newNode->type = malloc(sizeof(char) * (strlen(type) + 1));
	newNode->content = malloc(sizeof(char) * (strlen(content) + 1));
	strcpy(newNode->type, type);
	strcpy(newNode->content, content);
	newNode->right = NULL;
	newNode->left = NULL;

	return newNode;
}

void BindNode(node* parent, node* child, char* direction) {
	if (strcmp(direction, "left") == 0) {
		parent->left = child;
	}
	else if (strcmp(direction, "right") == 0) {
		parent->right = child;
	}
}