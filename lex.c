#include <stdio.h>
#include "functions.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MaxWordSize 255

void CommentStart();
void CommentContentSingle();
void CommentContentMultiLine();
void CommentEndMultiLine();
void Identifier(char word[MaxWordSize], int index, char c);
void IdentifierType(char word[MaxWordSize], int index);
void Sign(char word[MaxWordSize], char c);
void StringOpen(char word[MaxWordSize], int index);
void StringContent(char word[MaxWordSize], int index, char c);
void String(char word[MaxWordSize], int index);
void PositiveInteger(char word[MaxWordSize], int index, char c);
void SaveWordToList(char word[MaxWordSize], char* type);
int EscapeSequence(char word[MaxWordSize], int *index);
int HexEscape(char word[MaxWordSize], int *index);

wordListStr* wordList;

void PerformLex(wordListStr* wrdList)
{
	wordList = wrdList;
	char inputWord[MaxWordSize] = "";
	int index;
	char c;

	while((c = getc(stdin)) != EOF)
	{
		index = 0;

		if(c == '/')
			CommentStart();
		else if(isalpha(c) || c == '_')
			Identifier(inputWord, index, c);
		else if(c == '+' ||
				c == '-' ||
				c == '*' ||
				c == '/' ||
				c == '(' ||
				c == ')' ||
				c == '{' ||
				c == '}' ||
				c == ':' ||
				c == '=' ||
				c == '<' ||
				c == '>' ||
				c == ',' ||
				c == '!')
			Sign(inputWord, c);
		else if(c == '"')
			StringOpen(inputWord, index);
		else if(isdigit(c))
			PositiveInteger(inputWord, index, c);
		else if(c == EOF)
			return;
		else if(isspace(c)) {}
		else
		{
			fprintf(stderr,"character : '%c' : ", c);
			ExitProgram(1, "lex.c: unknown character\n");
		}
	}
	return;
}
void CommentStart()
{
	char c = getc(stdin);
	if(c == '*')
		CommentContentMultiLine();
	else if(c == '/')
		CommentContentSingle();
	else
		ExitProgram(1, "lex.c: comment syntax token not correctly initialized\n");
	return;
}
void CommentContentSingle()
{
	char c;
	while((c = getc(stdin)))
	{
		if(c == '\n' || c == EOF)
			return;
	}
	return;
}
void CommentContentMultiLine()
{
	char c;
	while((c = getc(stdin)))
	{
		if(c == '*')
		{
			CommentEndMultiLine();
			return;
		}
		else if(c == EOF)
			return;
	}
	return;
}
void CommentEndMultiLine()
{
	char c;
	c = getc(stdin);
	if(c == '/')
		return;
	else if(c == EOF)
		ExitProgram(1, "lex.c: multi-line comment was not terminated correctly\n");
	else
		CommentContentMultiLine();
	
	return;
}
void Identifier(char word[MaxWordSize], int index, char c)
{
	word[index++] = c;
	
	while((c = getc(stdin)))
	{
		if(isalpha(c) || c == '_' || isdigit(c))
			word[index++] = c;
		else if(c == EOF)
			return;
		else
		{
			ungetc(c, stdin);
			break;
		}
	}
	word[index++] = '\0';
	
	if(
	   strcmp(word, "else") == 0
	|| strcmp(word, "func") == 0
	|| strcmp(word, "if") == 0
	|| strcmp(word, "let") == 0
	|| strcmp(word, "return") == 0
	|| strcmp(word, "var") == 0
	|| strcmp(word, "while") == 0)
		SaveWordToList(word,"keyword");

	else if(
	   strcmp(word, "Double") == 0
	|| strcmp(word, "Int") == 0
	|| strcmp(word, "String") == 0)
		IdentifierType(word, index);

	else if(strcmp(word, "nil") == 0)
		SaveWordToList(word,"nil");
	
	else
		SaveWordToList(word,"identifier");
	
	return;
}
void Sign(char word[MaxWordSize], char c)
{
	word[0] = c;
	word[1] = '\0';
	SaveWordToList(word,"sign");
	return;
}
void StringOpen(char word[MaxWordSize], int index)
{
	char c = getc(stdin);
	if(c == '"')
	{
		word[0] = '"';
		word[1] = '"';
		word[2] = '\0';
		return;
	}
	else if( c > 31)
		StringContent(word, index, c);
	else
		ExitProgram(1,"lex.c: value inside of a string is not valid\n");
	return;
}
void StringContent(char word[MaxWordSize], int index, char c)
{
	word[index++] = c;
	int esacapeReturn = 0;
	while(esacapeReturn == 0 && (c = getc(stdin)))
	{
		if(c == EOF)
			ExitProgram(1,"lex.c: string not terminated\n");
		else if(c == '"')
		{
			String(word, index);
			return;
		}
		else if(c == 92)
		{
			esacapeReturn = EscapeSequence(word, &index);
		}
		else
			word[index++] = c;
	}
	return;
}
int EscapeSequence(char word[MaxWordSize], int *index)
{
	char c = getc(stdin);
	if(c == 'n')
	{
		word[*index] = '\n';
		*index = *index + 1;
	}
	else if(c == 't')
	{
		word[*index] = '\t';
		*index = *index + 1;
	}
	else if(c == '"')
	{
		word[*index] = '"';
		*index = *index + 1;
	}
	else if(c == '\\')
	{
		word[*index] = '\\';
		*index = *index + 1;
	}
	else if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
	{
		word[*index] = '\\';
		*index = *index + 1;
	}
	else if(c == 'u')
	{
		int value = HexEscape(word, index);
		if(value == -1)
			return 0;
		else if(value == -2)
		{
			String(word, *index);
			return -1;
		}
		else if(value == -3)
		{
			EscapeSequence(word, index);
			return 0;
		}
		else
		{
			word[*index] = value;
			*index = *index + 1;
		}
			

	}
	return 0;
}
void String(char word[MaxWordSize], int index)
{
	word[index] = '\0';
	SaveWordToList(word,"string");
}
void IdentifierType(char word[MaxWordSize], int index)
{
	char c = getc(stdin);
	if(c == '?')
	{
		word[--index] = c;
		word[++index] = '\0';
	}
	else
		ungetc(c, stdin);
	SaveWordToList(word,"identifier(type)");
	return;
}
void PositiveInteger(char word[MaxWordSize], int index, char c)
{
	word[index++] = c;
	while((c = getc(stdin)))
	{
		if(c == EOF || isspace(c))
			break;
		else if(isdigit(c))
		{
			word[index++] = c;
		}
		else
		{
			ungetc(c, stdin);
			break;
		}
	}
	SaveWordToList(word,"integer");

	return;
}
int HexEscape(char word[MaxWordSize], int *index)
{
	char buffer[MaxWordSize] = "\\u";
	int bufferIndex = 3;
	char hex[MaxWordSize]= "";
	int hexIndex = 0;
	char c = getc(stdin);
	if (c == '{')
	{
		strcat(buffer, "{");
		while((c = getc(stdin)))
		{
			//check if hex number
			if(c >= 48 && c <= 57 || c >= 65 && c <= 70 || c >= 97 && c <= 102)
			{
				buffer[bufferIndex++] = c;
				hex[hexIndex++] = c;
			}
			//check for white space
			else if(c == '\t' || c == '\n' || c == '\r' || c == ' ')
			{
				buffer[bufferIndex++] = c;
				strcat(word, buffer);
				*index = strlen(word);
				return -1;
			}
			else if(c == '}')
			{
				int len = strlen(hex);
				// NOTE: Unsure if this should throw error or just save as string
				if (len == 0 || len > 8)
				{
					buffer[bufferIndex++] = c;
					strcat(word, buffer);
					*index = strlen(word);
					return -1;
				}
				return (int)strtol(hex, NULL, 16);
			}
			else if(c == '"')
			{
				buffer[bufferIndex++] = c;
				strcat(word, buffer);
				*index = strlen(word);
				return -2;
			}
			else if(c == '\\')
			{
				strcat(word, buffer);
				*index = strlen(word);
				return -3;
			}
			else 
			{
				buffer[bufferIndex++] = c;
				strcat(word, buffer);
				*index = strlen(word);
				return -1;
			}
		}
	}
	else if(c == '"')
	{
		strcat(word, buffer);
		*index = strlen(word);
		return -2;
	}
	else
	{
		buffer[bufferIndex++] = c;
		strcat(word, buffer);
		*index = strlen(word);
		return -1;
	}
}

void SaveWordToList(char word[MaxWordSize], char* type)
{
	//Create new Word
	wordStr* newWord;
	newWord = malloc(sizeof(wordStr));
	newWord->content = malloc(sizeof(char) * (strlen(word) + 1));
	newWord->type = malloc(sizeof(char) * (strlen(type) + 1));
	newWord->next = NULL;
	strcpy(newWord->content, word);
	strcpy(newWord->type, type);


	//Save to List
	if(wordList->first == NULL)
	{
		wordList->first = newWord;
		wordList->last = newWord;
	}
	else
	{
		wordList->last->next = newWord;
		wordList->last = newWord;
	}
}