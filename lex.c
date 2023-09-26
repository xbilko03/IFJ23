#include <stdio.h>
#include "functions.h"
#include <ctype.h>
#include <string.h>

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

void PerformLex()
{
	char inputWord[MaxWordSize];
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
		if(isalpha(c) || c == '_')
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
		printf("keyword: %s\n",word);

	else if(
	   strcmp(word, "Double") == 0
	|| strcmp(word, "Int") == 0
	|| strcmp(word, "String") == 0)
		IdentifierType(word, index);

	else if(strcmp(word, "nil") == 0)
		printf("nil: %s\n",word);
	
	else
		printf("identifier: %s\n",word);
	
	return;
}
void Sign(char word[MaxWordSize], char c)
{
	word[0] = c;
	word[1] = '\0';
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
	while((c = getc(stdin)))
	{
		if(c == EOF)
			ExitProgram(1,"lex.c: string not terminated\n");
		else if(c == '"')
		{
			String(word, index);
			return;
		}
		else
			word[index++] = c;
	}
	return;
}
void String(char word[MaxWordSize], int index)
{
	word[index] = '\0';
	printf("string: %s\n",word);
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
	printf("identifier (type) = %s\n",word);
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
	printf("integer: %s\n",word);

	return;
}