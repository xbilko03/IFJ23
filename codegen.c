#include <stdio.h>
#include "functions.h"
#include <string.h>

void ProcessNode(Node* c_node);
void PrintCode(char* code);
void INBUILT_WRITE(Node* c_node);

void PerformCodeGen(Node* tree)
{

	/* Cycle through AST */
	ProcessNode(tree);

	return;
}

void ProcessNode(Node* c_node)
{
	if (c_node == NULL)
		return;
	
	/* Proccess Node here */

	if (strcmp(c_node->content, "root") == 0)
	{
		/* Mandatory Commands */
		PrintCode(".IFJcode23");
		PrintCode("DEFVAR GF@writeValue");
	}
	else if (strcmp(c_node->content, "write") == 0)
	{
		/* Process children nodes (args) here and end the branch */
		INBUILT_WRITE(c_node);

		return;
	}

	/* Proccess Node here */

	//printf("node = %s\n", c_node->content);

	/* Go to the next node */
	for (int i = 0; i < c_node->numChildren; i++)
	{
		ProcessNode(c_node->children[i]);
	}
	return;
}

/* Print CODE */
void PrintCode(char* code)
{
	fprintf(stdout, "%s\n", code);
	return;
}

/* Might not need everything, but its ready if needs be */

/*
* MOVE ?var? ?symb?
* Prirazeni hodnoty do promenne zkopiruje hodnotu ?symb? do ?var?.
* Napr.MOVE LF@par GF@var provede zkopirovani hodnoty promenne var v globálním rámci do promenne par v lokalnim ramci.
*/
void MOVE(Node* c_var, Node* c_symb)
{
	return;
}

/*
* CREATEFRAME
* Vytvor novy docasny ramec
* Vytvori novy docasny ramec a zahodi pripadny obsah puvodniho docasneho ramce.
*/
void CREATEFRAME()
{
	PrintCode("CREATEFRAME");
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
	PrintCode("PUSHFRAME");
	return;
}

/*
* POPFRAME
* P?esun aktuálního rámce do do?asného
* Presun vrcholovy ramec LF ze zasobniku ramcu do TF. 
* Pokud zadny ramec v LF neni k dispozici, dojde k chybe 55.
*/
void POPFRAME()
{
	PrintCode("POPFRAME");
	return;
}

/*
* DEFVAR ?var?
* Definuj novou prom?nnou v ramci
* Definuje promennou v urcenem ramci dle ?var?. Tato promenná je zatim neinicializovana 
* a bez urceni typu, ktery bude urcen az prirazenim nejake hodnoty.
*/
void DEFVAR(Node* c_var)
{
	/* symtable required */
	return;
}

/*
* CALL ?label?
* Skok na navesti s podporou navratu
* Ulozi inkrementovanou aktualni pozici z interniho citace instrukci do zasobniku volani
* a provede skok na zadané navestí(pripadnou pripravu ramce musi zajistit jine instrukce).
*/
void CALL(Node* c_label)
{
	return;
}

/*
* RETURN 
* Navrat na pozici ulozenou instrukci CALL
* Vyjme pozici ze zasobniku volani a skoci na tuto pozici nastavenim interního citace
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
* Neni-li zásobnik prazdny, vyjme z nej hodnotu a ulozi ji do promenne?var?,
* jinak dojde k chybe 56.
*/
void POPS(Node* c_var)
{
	return;
}

/*
* CLEARS
* Vymazani obsahu celeho datového zasobniku
* Pomocna instrukce, ktera smaze cely obsah datového zasobniku,
* aby neobsahoval zapomenute hodnoty z predchozich vypoctu.
*/
void CLEARS()
{
	return;
}

/*
* ADD ?var? ?symb1? ?symb2? 
* Soucet dvou ciselnych hodnot. Secte ?symb1? a ?symb2?
* (musí být stejného ?íselného typu int nebo float) a výslednou
* hodnotu tehoz typu ulozi do promenne ?var?.
*/
void ADD(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* SUB ?var? ?symb1? ?symb2?
* Odecitani dvou ciselnych hodnot. Odecte ?symb2? od ?symb1?
* (musi byt stejneho ciselneho typu int nebo float)
* a vyslednou hodnotu tehoz typu ulozi do promenne ?var?.
*/
void SUB(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/* MUL ?var? ?symb1? ?symb2?
* Nasobeni dvou ciselných hodnot Vynásobí ?symb1? a ?symb2?
* (musí být stejného ?íselného typu int nebo float)
* a výslednou hodnotu téhož typu uloží do promenné ?var?.
*/
void MUL(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* DIV ?var? ?symb1? ?symb2?
* Delení dvou desetinných hodnot. Podelí hodnotu ze ?symb1?
* druhou hodnotou ze ?symb2? (oba musí být typu float)
* a výsledek priradí do promenné ?var? (též typu float).
* Delení nulou zpusobí chybu 57.
*/
void DIV(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* IDIV ?var? ?symb1? ?symb2?
* Delení dvou celocíselných hodnot
* Celocíselne podelí hodnotu ze ?symb1? druhou hodnotou ze ?symb2?
* (musí být oba typu int) a výsledek priradí do promenné ?var? typu int.
* Delení nulou zpusobí chybu 57.
*/
void IDIV(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* ADDS / SUBS / MULS / DIVS / IDIVS
* Zásobníkové verze instrukcí ADD, SUB, MUL, DIV a IDIV
*/
/* might not need */

/*
* LT / GT / EQ ?var? ?symb1? ?symb2? 
* Relacní operátory menší, vetší, rovno
* Instrukce vyhodnotí rela?ní operátor mezi ?symb1? a ?symb2?
* (stejného typu; int, bool, float nebo string)
* a do booleovské promenné ?var? zapíše false pri neplatnosti 
* nebo true v prípade platnosti odpovídající relace.
* retezce jsou porovnávány lexikograficky a false je menší než true.
* Pro výpocet neostrých nerovností lze použít AND / OR / NOT.
* S operandem typu nil(druhý operand je libovolného typu)
* lze porovnávat pouze instrukcí EQ, jinak chyba 53.
*/
void COMPARE(char* c_option, Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* LTS / GTS / EQS
* Zasobniková verze instrukci LT / GT / EQ
*/
/* might not need */

/*
* AND / OR / NOT ?var? ?symb1? ?symb2?
* Základní booleovské operátory
* Aplikuje konjunkci(logické A) / disjunkci(logické NEBO) 
* na operandy typu bool ?symb1? a ?symb2?
* nebo negaci na ?symb1? (NOT má pouze 2 operandy)
* a výsledek typu bool zapíše do ?var?.
*/
void LOGIC(char* c_option,Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* ANDS / ORS / NOTS
* Zásobníková verze instrukcí AND, OR a NOT
*/
/* might not need */

/* 
* INT2FLOAT ?var? ?symb? Prevod celocíselné hodnoty na desetinnou
* Prevede celocíselnou hodnotu ?symb? na desetinné císlo
* a uloží je do ?var?.
*/
void INT2FLOAT(Node* c_var, Node* c_symb)
{
	return;
}

/*
* FLOAT2INT ?var? ?symb?
* Prevod desetinné hodnoty na celocíselnou(oseknutí)
* Prevede desetinnou hodnotu ?symb?
* na celocíselnou oseknutím desetinné cásti a uloží	ji do ?var?.
*/
void FLOAT2INT(Node* c_var, Node* c_symb)
{
	return;
}

/*
* INT2CHAR ?var? ?symb?
* Prevod celého císla na znak
* císelná hodnota ?symb? je dle ASCII prevedena na znak, který tvorí 
* jednoznakový retezec prirazený do ?var?.
* Je-li ?symb? mimo interval[0; 255], dojde k chyb? 58.
*/
void INT2CHAR(Node* c_var, Node* c_symb)
{
	return;
}

/*
* STRI2INT ?var? ?symb1? ?symb2?
* Ordinální hodnota znaku.
* Do ?var? uloží ordinální hodnotu znaku(dle ASCII)
* v retezci ?symb1? na pozici ?symb2? (indexováno od nuly).
* Indexace mimo daný ?et?zec vede na chybu 58.
*/
void STRI2INT(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* INT2FLOATS / FLOAT2INTS /	INT2CHARS / STRI2INTS
* Zásobníkové verze konverzních instrukcí
*/
/* might not need */

/*
* CONCAT ?var? ?symb1? ?symb2?
* Konkatenace dvou retezcu.
* Do promenné ?var? uloží retezec vzniklý
* konkatenací dvou retezcových operandu	?symb1? a ?symb2?
* (jiné typy nejsou povoleny).
*/
void CONCAT(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* STRLEN ?var? ?symb?
* Zjisti délku retezce.
* Zjistí délku retezce v ?symb? a délka je uložena jako
* celé císlo do ?var?.
*/
void STRLEN(Node* c_var, Node* c_symb)
{
	return;
}

/*
* GETCHAR ?var? ?symb1? ?symb2?
* Vrat znak retezce.
* Do ?var? uloží retezec z jednoho znaku v retezci ?symb1?
* na pozici ?symb2? (indexováno	celým ?íslem od nuly).
* Indexace mimo daný ?et?zec vede na chybu 58.
*/
void GETCHAR(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* SETCHAR ?var? ?symb1? ?symb2?
* Zmen znak retezce.
* Zmodifikuje znak retezce uloženého v promenné ?var?
* na pozici ?symb1? (indexováno	celocíselne od nuly)
* na znak v retezci ?symb2?
* (první znak, pokud obsahuje ?symb2? více znaku).
* Výsledný retezec je opet uložen do ?var?. 
* Pri indexaci mimo retezec ?var?
* nebo v prípade prázdného retezce v ?symb2? dojde k chyb? 58.
*/
void SETCHAR(Node* c_var, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* TYPE ?var? ?symb?
* Zjisti typ daného symbolu.
* Dynamicky zjistí typ symbolu ?symb? a do ?var?
* zapíše retezec znacící tento typ
* (int, bool, float, string nebo nil).
* Je-li ?symb? neinicializovaná promenná, oznací její typ
* prázdným retezcem.
*/
void TYPE(Node* c_var, Node* c_symb)
{
	return;
}

/*
* LABEL ?label?
* Definice náveští.
* Speciální instrukce oznacující pomocí náveští ?label?
* duležitou pozici v kódu jako potenciální cíl libovolné skokové instrukce.
* Pokus o redefinici existujícího náveští je chybou 52.
*/
void LABEL(Node* c_label)
{
	return;
}

/*
* JUMP ?label? 
* Nepodmínený skok na náveští.
* Provede nepodmínený skok na zadané náveští ?label?.
*/
void JUMP(Node* c_label)
{
	return;
}

/*
* JUMPIFEQ ?label? ?symb1? ?symb2?
* Podmínený skok na náveští pri rovnosti
* Pokud jsou ?symb1? a ?symb2? stejného typu nebo 
* je nekterý operand nil(jinak chyba 53)
* a zároven se jejich hodnoty rovnají, tak provede skok 
* na náveští ?label?.
*/
void JUMPIFEQ(Node* c_label, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* JUMPIFNEQ ?label? ?symb1? ?symb2?
* Podmínený skok na náveští pri nerovnosti.
* Jsou-li ?symb1? a ?symb2? stejného typu
* nebo je n?který operand nil(jinak chyba 53),
* ale ruzné hodnoty, tak provede skok na náveští ?label?.
*/
void JUMPIFNEQ(Node* c_label, Node* c_symb1, Node* c_symb2)
{
	return;
}

/*
* JUMPIFEQS / JUMPIFNEQS ?label?
* Zásobníková verze JUMPIFEQ, JUMPIFNEQ
* Zásobníkové skokové instrukce
* mají i jeden operand mimo datový zásobník, a to náveští ?label?,
* na které se prípadne provede skok.
*/
/* might not need */

/*
* EXIT ?symb?
* Ukoncení interpretace s návratovým kódem
* Ukoncí vykonávání programu a ukoncí interpret
* s návratovým kódem ?symb?, kde ?symb? je celé císlo
* v intervalu 0 až 49 (vcetne).
* Nevalidní celocíselná hodnota ?symb? vede na chybu 57.
*/
void EXIT(Node* c_symb)
{
	return;
}

/*
* BREAK
* Výpis stavu interpretu na stderr.
* Na standardní chybový výstup(stderr) vypíše stav interpretu
* v danou chvíli(tj.behem vykonávání této instrukce). 
* Stav se mimo jiné skládá z pozice v kódu, výpisu globálního,
* aktuálního lokálního a do?asného rámce a poctu již
* vykonaných instrukcí.
*/
void BREAK()
{
	return;
}

/*
* DPRINT ?symb?
* Výpis hodnoty na stderr.
* Vypíše zadanou hodnotu ?symb? na standardní chybový výstup(stderr).
* Výpisy touto instrukcí bude možné vypnout pomocí volby interpretu.
*/
void DPRINT(Node* c_symb)
{
	return;
}

/* func readString() -> String? */
/* func readInt() -> Int? */
/* func write(term1 , term2 , ..., termx) */
void PrintFormattedStringCode(char* input,char* code)
{
	if (input == NULL)
		return;
	
	fprintf(stdout, "%s", code);

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
	fprintf(stdout, "\n");
	return;
}
bool IsConstant(char* type)
{
	if (strcmp(type, "string") ||
		strcmp(type, "integer") ||
		strcmp(type, "float") ||
		strcmp(type, "nil"))
		return true;
	else
		return false;
}
void INBUILT_WRITE(Node* c_node)
{
	for (int i = 0; i < c_node->numChildren; i++)
	{
		if (IsConstant(c_node->children[i]->type))
		{
			PrintFormattedStringCode(c_node->children[i]->content, "MOVE GF@writeValue string@");
		}
		PrintCode("WRITE GF@writeValue");
	}
}
/* func Int2Double(_ term : Int) -> Double */
/* func Double2Int(_ term : Double) */
/* func length(_ s : String) -> Int */
/* func substring(of s : String, startingAt i : Int, endingBefore j :Int) -> String? */
/* func ord(_ c : String) -> Int */
/* func chr(_ i : Int) -> String */
