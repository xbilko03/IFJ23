#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define maxCommandTokenCount 255

//In use
void printTree(node* root, unsigned level);
void BindNode(node* parent, node* child, char* direction);
node* CreateNode(char* content, char* type, unsigned* level);
wordStr* prog_con(wordStr* currentWord, unsigned* level, node** currentNode);
wordStr* args(wordStr* currentWord, char* direction, unsigned* level, node** currentNode);
wordStr* func(wordStr* currentWord, char* id, char* direction, unsigned* level, node** currentNode);
wordStr* expression(wordStr* currentWord, wordStr* lastWord, char* direction, unsigned* level, node** currentNode);
wordStr* option(wordStr* currentWord, unsigned* level);
wordStr* funcdecl(wordStr* currentWord, char* id, char* direction, unsigned* level, node** currentNode);
wordStr* GetToken(wordStr* currentWord, char* ignoreNewLines);

void PerformSyntax(wordListStr* wrdList, node** AST) {
	
	wordStr* currentWord = wrdList->first;
	
	unsigned level = 1;

	node* program = CreateNode("root", "program", &level);
	node* currentNode = program;

	currentWord = GetToken(currentWord, "yes");
	currentWord = prog_con(currentWord, &level, &currentNode);

	//printTree(program, 1);

	*AST = program;

	return;
}

void printTree(node* root, unsigned level)
{
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

//GRAMMAR

wordStr* prog_con(wordStr* currentWord, unsigned* level, node** currentNode)
{
	//<prog_con> <EOF>
	if (currentWord == NULL)
		return NULL; //EOF
		
	//<prog_con> id
	if (strcmp(currentWord->type, "identifier") == 0)
	{
		char* id = currentWord->content;
		wordStr* lastWord = currentWord;
		currentWord = GetToken(currentWord, "yes");
		if(currentWord == NULL)
			exit(2);

		//<prog_con> func_id(<args>)
		if (strcmp(currentWord->content, "(") == 0)
		{
			currentWord = func(currentWord, id, "right", level, currentNode);

			//<prog_con> func_id(<args>) <prog_con>
			currentWord = prog_con(currentWord, level, currentNode);
			return currentWord;
		}
		//<prog_con> id =
		else if (strcmp(currentWord->content, "=") == 0) 
		{
			node* newNode = CreateNode(id, "identifier", level);
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;			 

			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			
			//<prog_con> id = id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				
				if (strcmp(currentWord->content, "*") == 0 || strcmp(currentWord->content, "-") == 0)
				{
					char* direction = "left";
					currentWord = expression(currentWord, lastWord, direction, level, currentNode);

					//<prog_con> id = <expression> <prog_con>
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
			}
			//<prog_con> id = <integer>
			else if (strcmp(currentWord->type, "integer") == 0)
			{
				char* intValue = currentWord->content;

				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);

				//<prog_con> id = <expression>
				if (strcmp(currentWord->content, "*") == 0 || strcmp(currentWord->content, "-") == 0)
				{
					char* direction = "left";
					currentWord = expression(currentWord, lastWord, direction, level, currentNode);

					//<prog_con> id = <expression> <prog_con>
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
				//<prog_con> id = <integer>
				else
				{
					node* newNode = CreateNode(intValue, "integer", level);
					BindNode(*currentNode, newNode, "left");
					*currentNode = newNode;

					*currentNode = (*currentNode)->parent;
				}
				currentWord = prog_con(currentWord, level, currentNode);
				return currentWord;
			}
		}		
	}
	//<prog_con> <keyword>
	else if (strcmp(currentWord->type, "keyword") == 0)
	{
		//<prog_con> else
		if (strcmp(currentWord->content, "else") == 0)
		{
			node* newNode = CreateNode(currentWord->content, "branching", level);
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;			 

			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			

			//<prog_con> else <prog_con>
			currentWord = prog_con(currentWord, level, currentNode);
			return currentWord;
		}
		//<prog_con> let
		else if (strcmp(currentWord->content, "let") == 0)
		{
			
			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			//<prog_con> let id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				node* newNode = CreateNode(currentWord->content, "constant declaration", level);
				if((*currentNode)->parent == NULL)
					BindNode(*currentNode, newNode, "left");
				else
				if ((*currentNode)->parent->left == NULL)
					BindNode(*currentNode, newNode, "left");
				else
					BindNode(*currentNode, newNode, "right");
				*currentNode = newNode;				 

				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				

				//<prog_con> let id <option>
				if (strcmp(currentWord->content, ":") == 0)
				{
					currentWord = option(currentWord, level);

					//<prog_con> let id <option> <typeIdentifier>
					if (strcmp(currentWord->type, "identifier(type)") == 0)
					{
						node* newNode = CreateNode(currentWord->content, "constant declaration", level);
						BindNode(*currentNode, newNode, "left");
						*currentNode = newNode;

						currentWord = GetToken(currentWord, "yes");
						if(currentWord == NULL)
							exit(2);
						

						//<prog_con> let id <option> <typeIdentifier> =
						if (strcmp(currentWord->content, "=") == 0)
						{
							currentWord = GetToken(currentWord, "yes");
							if(currentWord == NULL)
								exit(2);
							

							//<prog_con> let id <option> <typeIdentifier> = id
							if (strcmp(currentWord->type, "identifier") == 0)
							{
								char* id = currentWord->content;
								currentWord = GetToken(currentWord, "yes");
								
								if(currentWord == NULL)
									exit(2);
								//<prog_con> let id <option> <typeIdentifier> = id(<args>)
								if (strcmp(currentWord->content, "(") == 0)
								{
									currentWord = func(currentWord, id, "left", level, currentNode);

									//<prog_con> let id <option> <typeIdentifier> = id(<args>) <prog_con>
									*currentNode = (*currentNode)->parent;
									*currentNode = (*currentNode)->parent;
									currentWord = prog_con(currentWord, level, currentNode);
									return currentWord;
								}
							}
						}
					}
				}
				//<prog_con> let id =
				else if (strcmp(currentWord->content, "=") == 0)
				{
					currentWord = GetToken(currentWord, "yes");
					if(currentWord == NULL)
						exit(2);
					
					
					//<prog_con> var id = id
					if (strcmp(currentWord->type, "identifier") == 0)
					{
						char* id = currentWord->content;
						currentWord = GetToken(currentWord, "yes");
						if(currentWord == NULL)
							exit(2);
						
						if (strcmp(currentWord->content, "(") == 0)
						{
							currentWord = func(currentWord, id, "left", level, currentNode);

							*currentNode = (*currentNode)->parent;
							//<prog_con> var id = id <prog_con>

							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}

		}
		//<prog_con> var
		else if (strcmp(currentWord->content, "var") == 0)
		{
			
			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			//<prog_con> var id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				char* id = currentWord->content;

				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);

				//<prog_con> var id <option>
				if (strcmp(currentWord->content, ":") == 0)
				{
					node* newNode = CreateNode(id, "variable declaration", level);
					BindNode(*currentNode, newNode, "right");
					*currentNode = newNode;
					 

					currentWord = option(currentWord, level);

					//<prog_con> var id <option> <typeIdentifier>
					if (strcmp(currentWord->type, "identifier(type)") == 0)
					{
						node* newNode = CreateNode(currentWord->content, "variable type", level);
						BindNode(*currentNode, newNode, "left");
						*currentNode = newNode;

						currentWord = GetToken(currentWord, "yes");
						if(currentWord == NULL)
							exit(2);
						

						//<prog_con> var id <option> <typeIdentifier> =
						if (strcmp(currentWord->content, "=") == 0)
						{
							currentWord = GetToken(currentWord, "yes");
							if(currentWord == NULL)
								exit(2);

							//<prog_con> var id <option> <typeIdentifier> = id
							if (strcmp(currentWord->type, "identifier") == 0)
							{
								char* id = currentWord->content;
								currentWord = GetToken(currentWord, "yes");
								if(currentWord == NULL)
									exit(2);
								
								//<prog_con> var id <option> <typeIdentifier> = id(<args>)
								if (strcmp(currentWord->content, "(") == 0)
								{
									currentWord = func(currentWord, id, "right", level, currentNode);

									*currentNode = (*currentNode)->parent;

									//<prog_con> var id <option> <typeIdentifier> = id(<args>) <prog_con>
									currentWord = prog_con(currentWord, level, currentNode);
									return currentWord;
								}
							}
							//<prog_con> var id <option> <typeIdentifier> = <integer>
							else if (strcmp(currentWord->type, "integer") == 0)
							{
								node* newNode = CreateNode(currentWord->content, "integer", level);
								BindNode(*currentNode, newNode, "left");
								*currentNode = newNode;

								currentWord = GetToken(currentWord, "yes");
								if(currentWord == NULL)
									exit(2);
								

								*currentNode = (*currentNode)->parent;
								*currentNode = (*currentNode)->parent;

								currentWord = prog_con(currentWord, level, currentNode);
								return currentWord;
							}

						}
						//<prog_con> var id <option> <typeIdentifier> <prog_con>
						else
						{
							*currentNode = (*currentNode)->parent;

							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
					
					
				}
				//<prog_con> var id =
				else if (strcmp(currentWord->content, "=") == 0)
				{
					node* newNode = CreateNode(id, "variable declaration", level);
					BindNode(*currentNode, newNode, "right");
					*currentNode = newNode;
					 

					currentWord = GetToken(currentWord, "yes");
					if(currentWord == NULL)
						exit(2);
					

					//<prog_con> var id <option> = id
					if (strcmp(currentWord->type, "identifier") == 0)
					{
						char* id = currentWord->content;
						currentWord = GetToken(currentWord, "yes");
						if(currentWord == NULL)
							exit(2);
						
						if (strcmp(currentWord->content, "(") == 0)
						{
							currentWord = func(currentWord, id, "left", level, currentNode);

							*currentNode = (*currentNode)->parent;
							//<prog_con> var id <option> = id <prog_con>

							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}

		}
		//<prog_con> while
		else if (strcmp(currentWord->content, "while") == 0) 
		{
			node* newNode = CreateNode(currentWord->content, "loop", level);
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;
			 
			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			

			//<prog_con> while (
			if (strcmp(currentWord->content, "(") == 0)
			{
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				
				//<prog_con> while (id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					wordStr* lastWord = currentWord;
					
					currentWord = GetToken(currentWord, "yes");
					if(currentWord == NULL)
						exit(2);

					//<prog_con> while (<expression>
					if (strcmp(currentWord->content, ">") == 0)
					{
						char* direction = "left";
						currentWord = expression(currentWord, lastWord, direction, level, currentNode);
						//<prog_con> while (<expression>)
						if (strcmp(currentWord->content, ")") == 0)
						{
							currentWord = GetToken(currentWord, "yes");
							if(currentWord == NULL)
								exit(2);

							//<prog_con> while (<expression>) <prog_con>
							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
			}
		}
		//<prog_con> if
		else if(strcmp(currentWord->content, "if") == 0)
		{
			node* newNode = CreateNode(currentWord->content, "branching", level);
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;

			
			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);

			//<prog_con> if let
			if (strcmp(currentWord->content, "let") == 0)
			{
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				
				//<prog_con> if let id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					node* newNode = CreateNode(currentWord->content, "constant declaration", level);
					BindNode(*currentNode, newNode, "left");
					*currentNode = newNode;
					
					currentWord = GetToken(currentWord, "yes");
					if(currentWord == NULL)
						exit(2);
					

					//<prog_con> if let id <prog_con>
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
				exit(2);
			}

			//<prog_con> if (
			if (strcmp(currentWord->content, "(") == 0)
			{
				
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				//<prog_con> if (id
				if (strcmp(currentWord->type, "identifier") == 0)
				{
					wordStr* lastWord = currentWord;
					
					currentWord = GetToken(currentWord, "yes");
					if(currentWord == NULL)
						exit(2);
					
					//<prog_con> if (<expression>
					if (strcmp(currentWord->content, "<") == 0)
					{
						char* direction = "left";
						currentWord = expression(currentWord, lastWord, direction, level, currentNode);
						//<prog_con> if (<expression>)
						if (strcmp(currentWord->content, ")") == 0)
						{
							
							currentWord = GetToken(currentWord, "yes");
							if(currentWord == NULL)
								exit(2);

							currentWord = prog_con(currentWord, level, currentNode);
							return currentWord;
						}
					}
				}
				exit(2);
			}
		}
		//<prog_con> func
		else if (strcmp(currentWord->content, "func") == 0)
		{
			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			
			//<prog_con> func id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				char* id = currentWord->content;
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				
				//<prog_con> func id(
				if (strcmp(currentWord->content, "(") == 0)
				{
					//<prog_con> func id(<args>) -> <functionType>
					currentWord = funcdecl(currentWord, id, "right", level, currentNode);

					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
			}
		}
		//<prog_con> return
		else if (strcmp(currentWord->content, "return") == 0)
		{
			node* newNode = CreateNode(currentWord->content, "branching", level);
			BindNode(*currentNode, newNode, "right");
			*currentNode = newNode;

			currentWord = GetToken(currentWord, "yes");
			if(currentWord == NULL)
				exit(2);
			
			//<prog_con> return id
			if (strcmp(currentWord->type, "identifier") == 0)
			{
				wordStr* lastWord = currentWord;
				currentWord = GetToken(currentWord, "yes");
				if(currentWord == NULL)
					exit(2);
				
				//<prog_con> return id <expression>
				if (strcmp(currentWord->content, "*") == 0 || strcmp(currentWord->content, "-") == 0)
				{
					currentWord = expression(currentWord, lastWord, "left", level, currentNode);

					//<prog_con> return id <expression> <prog_con>
					currentWord = prog_con(currentWord, level, currentNode);
					return currentWord;
				}
				else if(strcmp(currentWord->content, "!") == 0)
				{
					//SKIP '!'
					currentWord = GetToken(currentWord, "yes");
					
				}
			}
		}
		exit(2);
	}
	//<prog_con> { <prog_con>
	if (strcmp(currentWord->content, "{") == 0)
	{
		currentWord = GetToken(currentWord, "yes");
		if(currentWord == NULL)
			exit(2);

		(*level)++;

		//<prog_con> { <prog_con>
		currentWord = prog_con(currentWord, level, currentNode);
		return currentWord;
	}
	//<prog_con> } <prog_con>
	else if (strcmp(currentWord->content, "}") == 0)
	{
		currentWord = GetToken(currentWord, "yes");

		//End of File detect?
		
		if (currentWord == NULL)
			return NULL;
		else if(currentWord == NULL)
			exit(2);
		//<prog_con> { <prog_con>
				
		unsigned targetLevel = *(*currentNode)->level;
		unsigned clevel;
		do
		{
			clevel = *(*currentNode)->level;
			*currentNode = (*currentNode)->parent;
			if(*currentNode == NULL)
				return NULL;
			if (strcmp((*currentNode)->type, "functionType") == 0)
			{
				do
				{
					*currentNode = (*currentNode)->parent;
					if (strcmp((*currentNode)->type, "root") == 0)
						exit(2);//neco nen� v pohode

				} while (strcmp((*currentNode)->type, "functionDeclaration"));
				break;
			}
		} while (clevel >= targetLevel);


		if (strcmp((*currentNode)->content, "if") == 0)
		{
			if ((*currentNode)->right != NULL)
			{
				if (strcmp((*currentNode)->right->content, "else") == 0)
				{
					*currentNode = (*currentNode)->right;
				}
			}
		}

		(*level)--;
				
		currentWord = prog_con(currentWord, level, currentNode);

		return currentWord;
	}
	else
		exit(2);

	return currentWord; //return to prog_con
}
wordStr* func(wordStr* currentWord, char* id, char* direction, unsigned* level, node** currentNode)
{
	node* newNode = CreateNode(id, "function", level);
	BindNode(*currentNode, newNode, direction);
	*currentNode = newNode;


	currentWord = GetToken(currentWord, "yes");
	if(currentWord == NULL)
			exit(2);

	currentWord = args(currentWord, "left", level, &newNode);

	
	if (strcmp(currentWord->content, ")") == 0)
	{
		currentWord = GetToken(currentWord, "yes");
		if(currentWord == NULL)
			return NULL;
		

		return currentWord;
	}
	else
		exit(2);

}
wordStr* funcdecl(wordStr* currentWord, char* id, char* direction, unsigned* level, node** currentNode)
{
	node* newNode = CreateNode(id, "functionDeclaration", level);
	BindNode(*currentNode, newNode, direction);
	*currentNode = newNode;

	currentWord = GetToken(currentWord, "yes");
	if(currentWord == NULL)
			exit(2);

	currentWord = args(currentWord, "left", level, &newNode);

	
	if (strcmp(currentWord->content, ")") == 0)
	{
		currentWord = GetToken(currentWord, "yes");
		
		currentWord = GetToken(currentWord, "yes"); //ignore ->
		
	}
	if (strcmp(currentWord->content, "Int") == 0)
	{
		*currentNode = (*currentNode)->left;

		while ((*currentNode)->right != NULL)
			*currentNode = (*currentNode)->right;
		
		node* newNode = CreateNode(currentWord->content, "function declaration", level);
		BindNode(*currentNode, newNode, "right");
		*currentNode = newNode;
	}
	currentWord = GetToken(currentWord, "yes");
	if(currentWord == NULL)
		exit(2);


	return currentWord;
}
wordStr* args(wordStr* currentWord, char* direction, unsigned* level, node** currentNode) {
	
	node* newNode;

	if (strcmp(currentWord->type, "string") == 0) //is OK
	{
		newNode = CreateNode(currentWord->content, "string", level);
		BindNode(*currentNode, newNode, direction);
		*currentNode = newNode;
		currentWord = GetToken(currentWord, "yes");
	}
	else if (strcmp(currentWord->type, "identifier") == 0) //is OK
	{
		if (strcmp(currentWord->content, "of") == 0 || strcmp(currentWord->content, "by") == 0 || strcmp(currentWord->content, "_") == 0) //is OK
		{
			currentWord = GetToken(currentWord, "yes");

			if (strcmp(currentWord->content, ":") == 0)
			{
				currentWord = GetToken(currentWord, "yes");

				if(strcmp(currentWord->type,"identifier") == 0)
					newNode = CreateNode(currentWord->content, "identifier", level);
				else if (strcmp(currentWord->type, "integer") == 0)
					newNode = CreateNode(currentWord->content, "integer", level);

				BindNode(*currentNode, newNode, direction);
				*currentNode = newNode;

				currentWord = GetToken(currentWord, "yes");
			}
			else
			{
				newNode = CreateNode(currentWord->content, "functionType", level);
				BindNode(*currentNode, newNode, direction);
				*currentNode = newNode;
				currentWord = GetToken(currentWord, "yes");
				currentWord = GetToken(currentWord, "yes");
				currentWord = GetToken(currentWord, "yes");
			}
		}
		else
		{
			newNode = CreateNode(currentWord->content, "identifier", level);
			BindNode(*currentNode, newNode, "left");
			*currentNode = newNode;
			currentWord = GetToken(currentWord, "yes");
		}
	}
	else if (strcmp(currentWord->type, "integer") == 0) //is OK
	{
		newNode = CreateNode(currentWord->content, "integer", level);
		BindNode(*currentNode, newNode, "left");
		*currentNode = newNode;
		currentWord = GetToken(currentWord, "yes");
	}
	else if (strcmp(currentWord->content, ",") == 0) //is OK
	{
		//empty
	}
	else
	{
		return currentWord; //No params
	}

	if (strcmp(currentWord->content, ",") == 0) //is OK
	{
		currentWord = GetToken(currentWord, "yes");
		currentWord = args(currentWord, "right", level, &newNode);
		return currentWord;
	}
	else
	{
		newNode = newNode->parent;
		*currentNode = newNode;
	}

	return currentWord;
}
wordStr* option(wordStr* currentWord, unsigned* level) {
	currentWord = GetToken(currentWord, "yes");
	

	return currentWord;
}
wordStr* expression(wordStr* currentWord, wordStr* lastWord, char* direction, unsigned* level, node** currentNode)
{
	node* newNode = CreateNode(currentWord->content, "expression", level);
	BindNode(*currentNode, newNode, direction);
	*currentNode = newNode;		 

	newNode = CreateNode(lastWord->content, "expression", level);
	BindNode(*currentNode, newNode, "left");
	*currentNode = newNode;

	currentWord = GetToken(currentWord, "yes");
	
	if (strcmp(currentWord->type, "integer") == 0)
	{
		node* newNode = CreateNode(currentWord->content, "integer", level);
		BindNode(*currentNode, newNode, "right");

		currentWord = GetToken(currentWord, "yes");
		
	}
	else if (strcmp(currentWord->type, "identifier") == 0)
	{
		node* newNode = CreateNode(currentWord->content, "identifier", level);
		BindNode(*currentNode, newNode, "right");

		currentWord = GetToken(currentWord, "yes");
		
	}
	*currentNode = (*currentNode)->parent;
	return currentWord;
}

//NODES//

node* CreateNode(char* content, char* type, unsigned* level)
{
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
void BindNode(node* parent, node* child, char* direction) 
{
	child->parent = parent;

	if (strcmp(direction, "left") == 0) {
		parent->left = child;
	}
	else if (strcmp(direction, "right") == 0)
	{
		parent->right = child;
	}
}

//OTHER//

wordStr* GetToken(wordStr* currentWord, char* ignoreNewLines)
{
	currentWord = currentWord->next;
	if(strcmp(ignoreNewLines,"yes") == 0)
		{
			while (currentWord != NULL && strcmp(currentWord->content, "newline") == 0) //Skip all newlines before first valid token
			{
				currentWord = currentWord->next;
			}
		}
	if (currentWord == NULL)
		return NULL;//EOF

	return currentWord;
}