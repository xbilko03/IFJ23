#include <stdio.h>
#include "functions.h"
#include <string.h>
#include <stdbool.h>

void PerformSyntax(wordListStr* wrdList) {
	wordStr* currentWord = wrdList->first; //GET HEADER

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
			if ((result = prog_con(&currentWord))) return result;
			
		}
		else {
			ExitProgram(2, "Missing Header");
		}

		return 0;
	}

	int prog_con(wordStr* currentWord) {
		int result;
		printf("IN PROG_CON\n");

		//########################################
		/*
		2. <prog_con> -> func FID (<arg>) <types> {<statement_with_return>} <prog_con>
		*/
		//func
		if((currentWord->type == keyword) && (strcmp(currentWord->content, "func") == 0)) {
			printf("<prog_con> -> func FID (<arg>) <types> {<statement>} <prog_con>");
			currentWord = currentWord->next;

			//FID
			if (strcmp(currentWord->type, "identifier") == 0) {
				currentWord = currentWord->next;
			}
			else {
				ExitProgram(2, "Missing FID");
			}

			if ((result = arg(currentWord))) return result;
			//after ARG

		}
		
		return 0;
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
		//<param_name>
		if (strcmp(currentWord->type, "identifier") == 0) {

			char* tmp_id = currentWord->content; // param_name cannot be same as ID name
			currentWord = currentWord->next;
			
			//ID
			if ((strcmp(currentWord->type, "identifier") == 0) && (strcmp(currentWord->type, tmp_id) != 0)) {
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
		}
		//########################################
		/*
		*9. <arg> -> eps
		*/
		// eps
		else if(strcmp(currentWord->content, ")")) {
			currentWord = currentWord->next;
		}

		return 0;
	}

	//########################################
	/*
	<args_more> rule implementation
	*/
	int args_more(wordStr* currentWord) {
		int result;
		printf("###################IN_ARGS_MORE#####################");
		//10. <args_more> -> , <param_name> ID : <type_spec> <args_more>
		return 0;
	}

	//########################################
	/*
	<type_spec> rule implementation
	TODO: doplnit typy do podmienok
	*/
	int type_spec(wordstr* currentWord) {
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

		return 0;
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