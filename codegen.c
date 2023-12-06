#include "codegen.h"

TRP* trpList[256];
unsigned int trpIndex = 0;

unsigned int labelID = 0;
unsigned int labelIndex = 0;
unsigned int labelList[50];

void PerformCodeGen(Node* tree)
{
	/* Cycle through AST */
	ProcessNode(tree);

	return;
}
typedef struct
{
	TRP* symTable;
	unsigned int ID;
}symStr;

symStr symTables[256];
unsigned int symIndex = 0;
unsigned int symID = 0;
int functionActiveFlag = 0;

unsigned int FindTableIndex(char* symbol)
{
	for (int i = symIndex - 1; i > -1; i--)
	{
		if (TableFindItem(symTables[i].symTable, symbol) != NULL)
			return symTables[i].ID;
	}
	return 404;
}
void ADD(Node* c_symb);
void SUB(Node* c_symb);
void IDIV(Node* c_symb);
void MUL(Node* c_symb);
void INBUILT_LENGTH(Node* c_node);
void INBUILT_INT2DOUBLE(Node* c_node);
void INBUILT_READSTRING(Node* c_node);
void INBUILT_READINT(Node* c_node);
void INBUILT_READDOUBLE(Node* c_node);
void INBUILT_WRITE(Node* c_node);
void INBUILT_DOUBLE2INT(Node* c_node);
void INBUILT_SUBSTRING(Node* c_node);
void INBUILT_ORD(Node* c_node);
void INBUILT_CHR(Node* c_node);

/* Print to stdout */
void PrintCode(char* code)
{
	fprintf(stdout, "%s", code);
	return;
}
void PrintString(char* input, char* code)
{
	if (input == NULL)
		return;

	PrintCode(code);

	for (int i = 0;i < strlen(input); i++)
	{
		if (input[i] == '\n')
		{
			fprintf(stdout, "\\010");
		}
		else if (input[i] == ' ')
		{
			fprintf(stdout, "\\032");
		}
		else if (input[i] == '\\')
		{
			fprintf(stdout, "\\092");
		}
		else if (input[i] == '#')
		{
			fprintf(stdout, "\\035");
		}
		else
		{
			fprintf(stdout, "%c", input[i]);
		}
	}
	return;
}
void PrintSymbol(Node* c_symb)
{
	//printf("received=%s of type %s\n", c_symb->content,c_symb->type);
	if (strcmp(c_symb->type, "string") == 0)
	{
		PrintString(c_symb->content,"string@");
	}
	else if (strcmp(c_symb->type, "integer") == 0)
	{
		PrintCode("INT@");
		PrintCode(c_symb->content);
	}
	else if (strcmp(c_symb->type, "double") == 0)
	{
		PrintCode("FLOAT@0x");
		PrintCode(c_symb->content);
		PrintCode("p+0");
	}
	else if (strcmp(c_symb->type, "identifier") == 0
		|| strcmp(c_symb->type, "let declaration") == 0
		|| strcmp(c_symb->type, "var declaration") == 0)
	{
		PrintCode("GF@");
		PrintCode(c_symb->content);
		if(functionActiveFlag == 1)
			PrintCode("_func");
		else
		{
			unsigned int index = FindTableIndex(c_symb->content);
			if (index != 404)
			{
				fprintf(stdout, "%d", index);
			}
		}

	}
	else if (strcmp(c_symb->type, "paramid") == 0)
	{
		PrintCode("GF@");
		PrintCode(c_symb->content);
		PrintCode("_func");
	}
	else if (strcmp(c_symb->content, "nil") == 0 && strcmp(c_symb->type, "keyword") == 0)
	{
		PrintCode("NIL@nil");
	}
	else if (strcmp(c_symb->type, "func identfier") == 0)
	{
		PrintCode(c_symb->content);
	}
	else if(strcmp(c_symb->type, "paramname") == 0)
	{
		PrintCode("GF@");
		PrintCode(c_symb->content);
		PrintCode("_func");
	}
	else
	{
		if (strcmp(c_symb->type, "function") == 0)
			PrintCode("INT@0");
	}

	return;
}
void PrintType(Node* c_symb)
{
	if (strcmp(c_symb->type, "integer") == 0)
		PrintCode("int");
	else if (strcmp(c_symb->type, "string") == 0)
		PrintCode("string");
	else if (strcmp(c_symb->type, "double") == 0)
		PrintCode("double");
	return;
}
void ExpressionSum(Node* c_symb)
{	
	if (strcmp(c_symb->type, "operator") == 0)
	{
		/* left side */

		if (strcmp(c_symb->parent->content, "+") == 0)
			ADD(c_symb->children[0]);
		else if (strcmp(c_symb->parent->content, "-") == 0)
			SUB(c_symb->children[0]);
		else if (strcmp(c_symb->parent->content, "*") == 0)
			MUL(c_symb->children[0]);
		else if (strcmp(c_symb->parent->content, "/") == 0)
			IDIV(c_symb->children[0]);

		/* right side */
		if (strcmp(c_symb->parent->type, "assign") == 0 || strcmp(c_symb->parent->type, "keyword") == 0)
		{
			ADD(c_symb->children[0]);
			ExpressionSum(c_symb->children[1]);
		}
		else if(strcmp(c_symb->children[1]->type,"operator") == 0)
			ExpressionSum(c_symb->children[1]);
		else if (strcmp(c_symb->content, "+") == 0)
			ADD(c_symb->children[1]);
		else if (strcmp(c_symb->content, "-") == 0)
			SUB(c_symb->children[1]);
		else if (strcmp(c_symb->content, "*") == 0)
			MUL(c_symb->children[1]);
		else if (strcmp(c_symb->content, "/") == 0)
			IDIV(c_symb->children[1]);
	}
	else
	{
		/* left side */
		if (strcmp(c_symb->parent->type, "assign") == 0 )
		{
			if (c_symb->children > 0)
			{
				if (strcmp(c_symb->children[0]->type,"paramname") == 0)
					return;
			}
			PrintCode("MOVE GF@expressionSum ");
			PrintSymbol(c_symb);
			PrintCode("\n");
		}
		else if (strcmp(c_symb->parent->content, "+") == 0)
			ADD(c_symb);
		else if (strcmp(c_symb->parent->content, "-") == 0)
			SUB(c_symb);
		else if (strcmp(c_symb->parent->content, "*") == 0)
			MUL(c_symb);
		else if (strcmp(c_symb->parent->content, "/") == 0)
			IDIV(c_symb);
	}
}
void ConditionJump(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	else if (c_symb->children[0] == NULL)
		return;
	if (strcmp(c_symb->content, "if") == 0)
	{
		Node* c_node = c_symb->children[0];
		if (strcmp(c_node->type, "keyword") == 0 && strcmp(c_node->content, "let") == 0)
		{
			c_node = c_node->children[0];
			PrintCode("EQ GF@compareValue ");
			PrintSymbol(c_node);
			PrintCode(" ");
			PrintCode("NIL@nil");
			PrintCode("\n");
		}
		else if (strcmp(c_node->type, "compare") == 0)
		{
			if (strcmp(c_node->content, ">=") == 0)
				PrintCode("LT GF@compareValue ");
			else if (strcmp(c_node->content, "<=") == 0)
				PrintCode("GT GF@compareValue ");
			else if (strcmp(c_node->content, "==") == 0 || strcmp(c_node->content, "!=") == 0)
				PrintCode("EQ GF@compareValue ");
			else if (strcmp(c_node->content, "<") == 0)
			{
				PrintCode("EQ GF@compareValue ");
				PrintSymbol(c_symb->children[0]->children[0]);
				PrintCode(" ");
				PrintSymbol(c_symb->children[0]->children[1]);
				PrintCode("\n");
				PrintCode("JUMPIFEQ ");
				fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
				PrintCode("GF@compareValue BOOL@true\n");
				PrintCode("GT GF@compareValue ");
			}
			else if (strcmp(c_node->content, ">") == 0)
			{
				PrintCode("EQ GF@compareValue ");
				PrintSymbol(c_symb->children[0]->children[0]);
				PrintCode(" ");
				PrintSymbol(c_symb->children[0]->children[1]);
				PrintCode("\n");
				PrintCode("JUMPIFEQ ");
				fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
				PrintCode("GF@compareValue BOOL@true\n");
				PrintCode("LT GF@compareValue ");
			}
			PrintSymbol(c_symb->children[0]->children[0]);
			PrintCode(" ");
			PrintSymbol(c_symb->children[0]->children[1]);
			PrintCode("\n");
		}
		if(strcmp(c_node->content, "!=") == 0)
			PrintCode("JUMPIFNEQ ");
		else
			PrintCode("JUMPIFEQ ");
		fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
		PrintCode("GF@compareValue BOOL@true\n");
	}
	else if (strcmp(c_symb->content, "while") == 0)
	{
		Node* c_node = c_symb->children[0];
		if (strcmp(c_node->type, "keyword") == 0 && strcmp(c_node->content, "let") == 0)
		{
			c_node = c_node->children[0];
			PrintCode("EQ GF@compareValue ");
			PrintSymbol(c_node);
			PrintCode(" ");
			PrintCode("NIL@nil");
			PrintCode("\n");
		}
		else if (strcmp(c_node->type, "compare") == 0)
		{
			if (strcmp(c_node->content, ">=") == 0)
				PrintCode("LT GF@compareValue ");
			else if (strcmp(c_node->content, "<=") == 0)
				PrintCode("GT GF@compareValue ");
			else if (strcmp(c_node->content, "==") == 0 || strcmp(c_node->content, "!=") == 0)
				PrintCode("EQ GF@compareValue ");
			else if (strcmp(c_node->content, "<") == 0)
			{
				PrintCode("EQ GF@compareValue ");
				PrintSymbol(c_symb->children[0]->children[0]);
				PrintCode(" ");
				PrintSymbol(c_symb->children[0]->children[1]);
				PrintCode("\n");
				PrintCode("JUMPIFEQ ");
				fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
				PrintCode("GF@compareValue BOOL@true\n");
				PrintCode("GT GF@compareValue ");
			}
			else if (strcmp(c_node->content, ">") == 0)
			{
				PrintCode("EQ GF@compareValue ");
				PrintSymbol(c_symb->children[0]->children[0]);
				PrintCode(" ");
				PrintSymbol(c_symb->children[0]->children[1]);
				PrintCode("\n");
				PrintCode("JUMPIFEQ ");
				fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
				PrintCode("GF@compareValue BOOL@true\n");
				PrintCode("LT GF@compareValue ");
			}
			PrintSymbol(c_symb->children[0]->children[0]);
			PrintCode(" ");
			PrintSymbol(c_symb->children[0]->children[1]);
			PrintCode("\n");
		}
		if (strcmp(c_node->content, "!=") == 0)
			PrintCode("JUMPIFNEQ ");
		else
			PrintCode("JUMPIFEQ ");
		fprintf(stdout, "LABEL%dend ", labelList[labelIndex - 1]);
		PrintCode("GF@compareValue BOOL@true\n");
	}
}
void FunctionDeclaration(Node* c_symb)
{
	if (c_symb->numChildren < 1)
		return;

	Node* c_node = c_symb->children[0];
	for (int i = 0; i < c_node->numChildren; i++)
	{
		DEFVAR(c_node->children[i]->children[0]);
		DEFVAR(c_node->children[i]);
	}
	return;
}

/* Process AST */
void ProcessNode(Node* c_node)
{
	if (c_node == NULL)
		return;

	//printf("__NODE %s of type %s\n", c_node->content, c_node->type);

	/* Proccess Node here */
	if (strcmp(c_node->content, "root") == 0)
	{
		/* Mandatory Commands */
		PrintCode(".IFJcode23\n");
		PrintCode("DEFVAR GF@writeValue\n");
		PrintCode("DEFVAR GF@expressionSum\n");
		PrintCode("DEFVAR GF@compareValue\n");
		PrintCode("DEFVAR GF@funcReturn\n");
		/* Add global TRP */
		symTables[symIndex].symTable = c_node->TRP;
		symTables[symIndex++].ID = symID++;
	}
	else if (strcmp(c_node->type, "keyword") == 0)
	{
		if (strcmp(c_node->content, "let") == 0 || strcmp(c_node->content, "var") == 0)
		{
			if (strcmp(c_node->parent->content, "if") == 0 && strcmp(c_node->parent->type, "keyword") == 0)
			{
				/* let a | var a */
				/* empty */
			}
			else
				DEFVAR(c_node->children[0]);
			return;
		}
		else if (strcmp(c_node->content, "if") == 0)
		{
			PrintCode("LABEL LABEL");
			labelList[labelIndex] = labelID++;
			fprintf(stdout,"%d", labelList[labelIndex]);
			labelIndex++;
			PrintCode("\n");
			ConditionJump(c_node);
		}
		else if (strcmp(c_node->content, "else") == 0)
		{
			PrintCode("JUMP LABEL");
			fprintf(stdout, "%d", labelList[labelIndex - 1]);
			PrintCode("skip\n");

			PrintCode("LABEL LABEL");
			fprintf(stdout, "%d", labelList[labelIndex - 1]);
			PrintCode("end\n");
		}
		else if (strcmp(c_node->content, "while") == 0)
		{
			PrintCode("LABEL LABEL");
			labelList[labelIndex] = labelID++;
			fprintf(stdout, "%d", labelList[labelIndex]);
			labelIndex++;
			PrintCode("\n");
			ConditionJump(c_node);
		}
		else if (strcmp(c_node->content, "func") == 0)
		{
			/* define variables */
			FunctionDeclaration(c_node);

			PrintCode("JUMP func");
			PrintSymbol(c_node->children[0]);
			PrintCode("skip\n");

			PrintCode("LABEL func");
			PrintSymbol(c_node->children[0]);
			PrintCode("\n");

			/* process body */
			functionActiveFlag = 1;

			/* move values to local variables */
			for (int i = 0; i < c_node->children[0]->numChildren; i++)
			{
				PrintCode("MOVE ");
				PrintSymbol(c_node->children[0]->children[i]->children[0]);
				PrintCode(" ");
				PrintCode("GF@");
				PrintCode(c_node->children[0]->children[i]->content);
				PrintCode("_func");
				PrintCode("\n");
			}

			ProcessNode(c_node->children[2]);
			functionActiveFlag = 0;

			/* return from func */
			PrintCode("RETURN\n");
			/* skip function */
			PrintCode("LABEL func");
			PrintSymbol(c_node->children[0]);
			PrintCode("skip\n");
			return;
		}
		else if (strcmp(c_node->content, "return") == 0)
		{
			if (c_node->numChildren > 0)
			{
				PrintCode("MOVE GF@funcReturn ");
				PrintSymbol(c_node->children[0]);
				PrintCode("\n");
			}
			return;
		}
	}
	else if (strcmp(c_node->type, "body") == 0 )
	{
		/* Add local table (if it exists) */
		if (c_node->TRP == NULL)
			return;
		symTables[symIndex].symTable = c_node->TRP;
		symTables[symIndex++].ID = symID++;
	}
	else if (strcmp(c_node->type, "function") == 0)
	{
		if (strcmp(c_node->content, "write") == 0)
			INBUILT_WRITE(c_node);
		else if (strcmp(c_node->content, "Int2Double") == 0)
			INBUILT_INT2DOUBLE(c_node);
		else if (strcmp(c_node->content, "Double2Int") == 0)
			INBUILT_DOUBLE2INT(c_node);
		else if (strcmp(c_node->content, "readString") == 0)
			INBUILT_READSTRING(c_node);
		else if (strcmp(c_node->content, "readInt") == 0)
			INBUILT_READINT(c_node);
		else if (strcmp(c_node->content, "readDouble") == 0)
			INBUILT_READDOUBLE(c_node);
		else if (strcmp(c_node->content, "length") == 0)
			INBUILT_LENGTH(c_node);
		else if (strcmp(c_node->content, "substring") == 0)
			INBUILT_SUBSTRING(c_node);
		else if (strcmp(c_node->content, "ord") == 0)
			INBUILT_ORD(c_node);
		else if (strcmp(c_node->content, "chr") == 0)
			INBUILT_CHR(c_node);
	}
	else if (strcmp(c_node->type, "assign") == 0)
	{
		PrintCode("MOVE GF@expressionSum INT@0\n");
		ExpressionSum(c_node->children[1]);
		PrintCode("MOVE ");
		PrintSymbol(c_node->children[0]);
		PrintCode(" GF@expressionSum\n");	
	}
	else if (strcmp(c_node->type, "identifier") == 0)
	{
		/* call function */
		if (c_node->numChildren != 0 && strcmp(c_node->children[0]->type, "paramname") == 0)
		{
			/* params func */

			/* assign params to params inside of the function */
			for (int i = 0; i < c_node->numChildren; i++)
			{
				PrintCode("MOVE GF@");
				PrintCode(c_node->children[i]->content);
				PrintCode("_func");
				PrintCode(" ");
				if(c_node->children[i]->numChildren > 0)
					PrintSymbol(c_node->children[i]->children[0]);
				else
					PrintSymbol(c_node->children[i]);
				PrintCode("\n");
			}

			/* call func */
			PrintCode("CALL func");
			PrintCode(c_node->content);
			PrintCode("\n");

			/* return assign */
			if (strcmp(c_node->parent->type,"assign") == 0)
			{
				PrintCode("MOVE GF@expressionSum GF@funcReturn\n");
				PrintCode("MOVE ");
				PrintSymbol(c_node->parent->children[0]);
				PrintCode(" GF@expressionSum");
				PrintCode("\n");
			}
			return;
		}
	}
	
	/* Go to the next node */
	for (int i = 0; i < c_node->numChildren; i++)
	{
		ProcessNode(c_node->children[i]);
	}

	/* if the body ends, destroy local table */
	if (strcmp(c_node->type, "body") == 0)
	{
		if (c_node->TRP == NULL)
			return;
		symIndex--;
	}
	if (strcmp(c_node->content, "else") == 0)
	{
		PrintCode("LABEL LABEL");
		labelList[labelIndex] = labelID;
		fprintf(stdout, "%d", labelList[labelIndex - 1]);
		labelIndex--;
		PrintCode("skip\n");
	}
	if (strcmp(c_node->content, "while") == 0)
	{
		PrintCode("JUMP LABEL");
		labelList[labelIndex] = labelID;
		fprintf(stdout, "%d", labelList[labelIndex - 1]);
		PrintCode("\n");

		PrintCode("LABEL LABEL");
		labelList[labelIndex] = labelID;
		fprintf(stdout, "%d", labelList[labelIndex - 1]);
		labelIndex--;
		PrintCode("end\n");
	}
	return;
}
void MOVE(Node* c_var, Node* c_symb)
{
	if (c_var == NULL || c_symb == NULL)
		return;

	/* MOVE GF@a1 */
	PrintCode("MOVE ");
	PrintSymbol(c_var);

	PrintCode(" ");

	/* MOVE GF@a1 INT@69 */
	PrintSymbol(c_symb);
	PrintCode("\n");
	return;
}

/*
* DEFVAR ?var?
* Definuj novou prom?nnou v ramci
* Definuje promennou v urcenem ramci dle ?var?. Tato promenn� je zatim neinicializovana 
* a bez urceni typu, ktery bude urcen az prirazenim nejake hodnoty.
*/
void DEFVAR(Node* c_var)
{
	if (c_var == NULL)
		return;

	PrintCode("DEFVAR ");
	PrintSymbol(c_var);
	PrintCode("\n");

	/* assign something to this defined variable */
	if (c_var->parent->numChildren > 1 && strcmp(c_var->parent->type,"func identfier") != 0)
	{
		/* let a : Int = 5 */
		if (strcmp(c_var->parent->children[1]->type, "operator") == 0)
		{
			PrintCode("MOVE GF@expressionSum INT@0\n");

			ExpressionSum(c_var->parent->children[1]);

			PrintCode("MOVE ");
			PrintSymbol(c_var);
			PrintCode(" GF@expressionSum\n");
		}
		else
		{
			MOVE(c_var, c_var->parent->children[1]);
		}
	}
	else
	{
		PrintCode("MOVE ");
		PrintSymbol(c_var);
		PrintCode(" NIL@nil\n");
	}
	return;
}

void ADD(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	PrintCode("ADD GF@expressionSum GF@expressionSum ");
	PrintSymbol(c_symb);
	PrintCode("\n");
}

void SUB(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	PrintCode("SUB GF@expressionSum GF@expressionSum ");
	PrintSymbol(c_symb);
	PrintCode("\n");
}

void MUL(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	PrintCode("MUL GF@expressionSum GF@expressionSum ");
	PrintSymbol(c_symb);
	PrintCode("\n");
}

void IDIV(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	PrintCode("IDIV GF@expressionSum GF@expressionSum ");
	PrintSymbol(c_symb);
	PrintCode("\n");
}
/* func readString() -> String? */
void INBUILT_READSTRING(Node* c_node)
{
	if (c_node->numChildren != 0)
		return;

	PrintCode("READ ");
	PrintCode("GF@expressionSum ");
	PrintCode("string");
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func readInt() -> Int? */
void INBUILT_READINT(Node* c_node)
{
	if (c_node->numChildren != 0)
		return;

	PrintCode("READ ");
	PrintCode("GF@expressionSum ");
	PrintCode("int");
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func readInt() -> Int? */
void INBUILT_READDOUBLE(Node* c_node)
{
	if (c_node->numChildren != 0)
		return;

	PrintCode("READ ");
	PrintCode("GF@expressionSum ");
	PrintCode("float");
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func write(term1 , term2 , ..., termx) */
void INBUILT_WRITE(Node* c_node)
{
	for (int i = 0; i < c_node->numChildren; i++)
	{
		PrintCode("MOVE GF@writeValue ");
		PrintSymbol(c_node->children[i]);
		PrintCode("\nWRITE GF@writeValue\n");
	}
}
/* func Int2Double(_ term : Int) -> Double */
void INBUILT_INT2DOUBLE(Node* c_node)
{
	if (c_node->numChildren == 0)
		return;

	PrintCode("INT2FLOAT ");
	PrintCode("GF@expressionSum ");
	PrintSymbol(c_node->children[0]);
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func Double2Int(_ term : Double) */
void INBUILT_DOUBLE2INT(Node* c_node)
{
	if (c_node->numChildren == 0)
		return;

	PrintCode("FLOAT2INT ");
	PrintCode("GF@expressionSum ");
	PrintSymbol(c_node->children[0]);
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func length(_ s : String) -> Int */
void INBUILT_LENGTH(Node* c_node)
{
	if (c_node->numChildren == 0)
		return;

	PrintCode("STRLEN ");
	PrintCode("GF@expressionSum ");
	PrintSymbol(c_node->children[0]);
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func substring(of s : String, startingAt i : Int, endingBefore j :Int) -> String? */
void INBUILT_SUBSTRING(Node* c_node)
{
	if (c_node->numChildren == 0)
		return;
	/* empty */
}
/* func ord(_ c : String) -> Int */
void INBUILT_ORD(Node* c_node)
{
	if (c_node->numChildren == 0)
		return;

	PrintCode("STRI2INT ");
	PrintCode("GF@expressionSum ");
	PrintSymbol(c_node->children[0]);
	PrintCode(" INT@0");
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}
/* func chr(_ i : Int) -> String */
void INBUILT_CHR(Node* c_node)
{

	if (c_node->numChildren == 0)
		return;

	PrintCode("INT2CHAR ");
	PrintCode("GF@expressionSum ");
	PrintSymbol(c_node->children[0]);
	PrintCode("\n");

	if (strcmp(c_node->parent->type, "assign") == 0)
	{
		PrintCode("MOVE ");
		PrintSymbol(c_node->parent->children[0]);
		PrintCode(" ");
		PrintCode("GF@expressionSum");
		PrintCode("\n");
	}
}