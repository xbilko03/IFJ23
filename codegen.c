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
unsigned int paramIndex = 0;
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
		if(strcmp(c_node->content, "write") == 0)
			INBUILT_WRITE(c_node);
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
				PrintSymbol(c_node->children[i]->children[0]);
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
* CREATEFRAME
* Vytvor novy docasny ramec
* Vytvori novy docasny ramec a zahodi pripadny obsah puvodniho docasneho ramce.
*/
void CREATEFRAME()
{
	PrintCode("CREATEFRAME\n");
	return;
}

/*
* PUSHFRAME
* Presun docasneho ramce na zasobnik ramcu
* Presun TF na zasobnik ramcu.Ramec bude k dispozici pres LF a prekryje puvodni
* ramce na zasobniku ramcu.TF bude po provedeni instrukce nedefinovan a je treba jej
* pred dalsim pouzitim vytvorit pomoci CREATEFRAME. Pokus o pristup k nedefinovanemu ramci vede na chybu 55.
*/
void PUSHFRAME()
{
	PrintCode("PUSHFRAME\n");
	return;
}

/*
* POPFRAME
* P?esun aktu�ln�ho r�mce do do?asn�ho
* Presun vrcholovy ramec LF ze zasobniku ramcu do TF. 
* Pokud zadny ramec v LF neni k dispozici, dojde k chybe 55.
*/
void POPFRAME()
{
	PrintCode("POPFRAME\n");
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

/*
* CALL ?label?
* Skok na navesti s podporou navratu
* Ulozi inkrementovanou aktualni pozici z interniho citace instrukci do zasobniku volani
* a provede skok na zadan� navest�(pripadnou pripravu ramce musi zajistit jine instrukce).
*/
void CALL(Node* c_label)
{
	return;
}

/*
* RETURN 
* Navrat na pozici ulozenou instrukci CALL
* Vyjme pozici ze zasobniku volani a skoci na tuto pozici nastavenim intern�ho citace
* instrukci(uklid lokalnich ramcu musi zajistit jine instrukce).Provedeni instrukce pri
* prazdnem zasobniku volani vede na chybu 56.
*/
void RETURN()
{
	return;
}

/*
* PUSHS ?symb?
* Vlou hodnotu na vrchol datoveho zasobniku
* Ulozi hodnotu ?symb? na datovy zasobnik.
*/
void PUSHS(Node* c_symb)
{
	return;
}

/* POPS ?var?
* Vyjmi hodnotu z vrcholu datoveho zasobniku
* Neni-li z�sobnik prazdny, vyjme z nej hodnotu a ulozi ji do promenne?var?,
* jinak dojde k chybe 56.
*/
void POPS(Node* c_var)
{
	return;
}

/*
* CLEARS
* Vymazani obsahu celeho datov�ho zasobniku
* Pomocna instrukce, ktera smaze cely obsah datov�ho zasobniku,
* aby neobsahoval zapomenute hodnoty z predchozich vypoctu.
*/
void CLEARS()
{
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

/*
* DIV ?var? ?symb1? ?symb2?
* Delen� dvou desetinn�ch hodnot. Podel� hodnotu ze ?symb1?
* druhou hodnotou ze ?symb2? (oba mus� b�t typu float)
* a v�sledek prirad� do promenn� ?var? (t� typu float).
* Delen� nulou zpusob� chybu 57.
*/
void DIV(Node* c_symb)
{
	return;
}

void IDIV(Node* c_symb)
{
	if (c_symb == NULL)
		return;
	PrintCode("IDIV GF@expressionSum GF@expressionSum ");
	PrintSymbol(c_symb);
	PrintCode("\n");
}

/*
* ADDS / SUBS / MULS / DIVS / IDIVS
* Z�sobn�kov� verze instrukc� ADD, SUB, MUL, DIV a IDIV
*/
/* might not need */

/*
* LT / GT / EQ ?var? ?symb1? ?symb2? 
* Relacn� oper�tory men��, vet��, rovno
* Instrukce vyhodnot� rela?n� oper�tor mezi ?symb1? a ?symb2?
* (stejn�ho typu; int, bool, float nebo string)
* a do booleovsk� promenn� ?var? zap�e false pri neplatnosti 
* nebo true v pr�pade platnosti odpov�daj�c� relace.
* retezce jsou porovn�v�ny lexikograficky a false je men�� ne� true.
* Pro v�pocet neostr�ch nerovnost� lze pou��t AND / OR / NOT.
* S operandem typu nil(druh� operand je libovoln�ho typu)
* lze porovn�vat pouze instrukc� EQ, jinak chyba 53.
*/
void COMPARE(char* c_option, Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* LTS / GTS / EQS
* Zasobnikov� verze instrukci LT / GT / EQ
*/
/* might not need */

/*
* AND / OR / NOT ?var? ?symb1? ?symb2?
* Z�kladn� booleovsk� oper�tory
* Aplikuje konjunkci(logick� A) / disjunkci(logick� NEBO) 
* na operandy typu bool ?symb1? a ?symb2?
* nebo negaci na ?symb1? (NOT m� pouze 2 operandy)
* a v�sledek typu bool zap�e do ?var?.
*/
void LOGIC(char* c_option,Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* ANDS / ORS / NOTS
* Z�sobn�kov� verze instrukc� AND, OR a NOT
*/
/* might not need */

/* 
* INT2FLOAT ?var? ?symb? Prevod celoc�seln� hodnoty na desetinnou
* Prevede celoc�selnou hodnotu ?symb? na desetinn� c�slo
* a ulo�� je do ?var?.
*/
void INT2FLOAT(Node* c_var, Node* c_symb)
{
	return;
}

/*
* FLOAT2INT ?var? ?symb?
* Prevod desetinn� hodnoty na celoc�selnou(oseknut�)
* Prevede desetinnou hodnotu ?symb?
* na celoc�selnou oseknut�m desetinn� c�sti a ulo��	ji do ?var?.
*/
void FLOAT2INT(Node* c_var, Node* c_symb)
{
	return;
}

/*
* INT2CHAR ?var? ?symb?
* Prevod cel�ho c�sla na znak
* c�seln� hodnota ?symb? je dle ASCII prevedena na znak, kter� tvor� 
* jednoznakov� retezec prirazen� do ?var?.
* Je-li ?symb? mimo interval[0; 255], dojde k chyb? 58.
*/
void INT2CHAR(Node* c_var, Node* c_symb)
{
	return;
}

/*
* STRI2INT ?var? ?symb1? ?symb2?
* Ordin�ln� hodnota znaku.
* Do ?var? ulo�� ordin�ln� hodnotu znaku(dle ASCII)
* v retezci ?symb1? na pozici ?symb2? (indexov�no od nuly).
* Indexace mimo dan� ?et?zec vede na chybu 58.
*/
void STRI2INT(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* INT2FLOATS / FLOAT2INTS /	INT2CHARS / STRI2INTS
* Z�sobn�kov� verze konverzn�ch instrukc�
*/
/* might not need */

/*
* CONCAT ?var? ?symb1? ?symb2?
* Konkatenace dvou retezcu.
* Do promenn� ?var? ulo�� retezec vznikl�
* konkatenac� dvou retezcov�ch operandu	?symb1? a ?symb2?
* (jin� typy nejsou povoleny).
*/
void CONCAT(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* STRLEN ?var? ?symb?
* Zjisti d�lku retezce.
* Zjist� d�lku retezce v ?symb? a d�lka je ulo�ena jako
* cel� c�slo do ?var?.
*/
void STRLEN(Node* c_var, Node* c_symb)
{
	return;
}

/*
* GETCHAR ?var? ?symb1? ?symb2?
* Vrat znak retezce.
* Do ?var? ulo�� retezec z jednoho znaku v retezci ?symb1?
* na pozici ?symb2? (indexov�no	cel�m ?�slem od nuly).
* Indexace mimo dan� ?et?zec vede na chybu 58.
*/
void GETCHAR(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* SETCHAR ?var? ?symb1? ?symb2?
* Zmen znak retezce.
* Zmodifikuje znak retezce ulo�en�ho v promenn� ?var?
* na pozici ?symb1? (indexov�no	celoc�selne od nuly)
* na znak v retezci ?symb2?
* (prvn� znak, pokud obsahuje ?symb2? v�ce znaku).
* V�sledn� retezec je opet ulo�en do ?var?. 
* Pri indexaci mimo retezec ?var?
* nebo v pr�pade pr�zdn�ho retezce v ?symb2? dojde k chyb? 58.
*/
void SETCHAR(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* TYPE ?var? ?symb?
* Zjisti typ dan�ho symbolu.
* Dynamicky zjist� typ symbolu ?symb? a do ?var?
* zap�e retezec znac�c� tento typ
* (int, bool, float, string nebo nil).
* Je-li ?symb? neinicializovan� promenn�, oznac� jej� typ
* pr�zdn�m retezcem.
*/
void TYPE(Node* c_var, Node* c_symb)
{
	return;
}

/*
* LABEL ?label?
* Definice n�ve�t�.
* Speci�ln� instrukce oznacuj�c� pomoc� n�ve�t� ?label?
* dule�itou pozici v k�du jako potenci�ln� c�l libovoln� skokov� instrukce.
* Pokus o redefinici existuj�c�ho n�ve�t� je chybou 52.
*/
void LABEL(Node* c_label, char* type)
{
	return;
}

/*
* JUMP ?label? 
* Nepodm�nen� skok na n�ve�t�.
* Provede nepodm�nen� skok na zadan� n�ve�t� ?label?.
*/
void JUMP(Node* c_label)
{
	return;
}

/*
* JUMPIFEQ ?label? ?symb1? ?symb2?
* Podm�nen� skok na n�ve�t� pri rovnosti
* Pokud jsou ?symb1? a ?symb2? stejn�ho typu nebo 
* je nekter� operand nil(jinak chyba 53)
* a z�roven se jejich hodnoty rovnaj�, tak provede skok 
* na n�ve�t� ?label?.
*/
void JUMPIFEQ(Node* c_label, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* JUMPIFNEQ ?label? ?symb1? ?symb2?
* Podm�nen� skok na n�ve�t� pri nerovnosti.
* Jsou-li ?symb1? a ?symb2? stejn�ho typu
* nebo je n?kter� operand nil(jinak chyba 53),
* ale ruzn� hodnoty, tak provede skok na n�ve�t� ?label?.
*/
void JUMPIFNEQ(Node* c_label, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* JUMPIFEQS / JUMPIFNEQS ?label?
* Z�sobn�kov� verze JUMPIFEQ, JUMPIFNEQ
* Z�sobn�kov� skokov� instrukce
* maj� i jeden operand mimo datov� z�sobn�k, a to n�ve�t� ?label?,
* na kter� se pr�padne provede skok.
*/
/* might not need */

/*
* EXIT ?symb?
* Ukoncen� interpretace s n�vratov�m k�dem
* Ukonc� vykon�v�n� programu a ukonc� interpret
* s n�vratov�m k�dem ?symb?, kde ?symb? je cel� c�slo
* v intervalu 0 a� 49 (vcetne).
* Nevalidn� celoc�seln� hodnota ?symb? vede na chybu 57.
*/
void EXIT(Node* c_symb)
{
	return;
}

/*
* BREAK
* V�pis stavu interpretu na stderr.
* Na standardn� chybov� v�stup(stderr) vyp�e stav interpretu
* v danou chv�li(tj.behem vykon�v�n� t�to instrukce). 
* Stav se mimo jin� skl�d� z pozice v k�du, v�pisu glob�ln�ho,
* aktu�ln�ho lok�ln�ho a do?asn�ho r�mce a poctu ji�
* vykonan�ch instrukc�.
*/
void BREAK()
{
	return;
}

/*
* DPRINT ?symb?
* V�pis hodnoty na stderr.
* Vyp�e zadanou hodnotu ?symb? na standardn� chybov� v�stup(stderr).
* V�pisy touto instrukc� bude mo�n� vypnout pomoc� volby interpretu.
*/
void DPRINT(Node* c_symb)
{
	return;
}

/* func readString() -> String? */
/* func readInt() -> Int? */
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
/* func Double2Int(_ term : Double) */
/* func length(_ s : String) -> Int */
/* func substring(of s : String, startingAt i : Int, endingBefore j :Int) -> String? */
/* func ord(_ c : String) -> Int */
/* func chr(_ i : Int) -> String */
