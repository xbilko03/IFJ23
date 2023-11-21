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
		PrintCode(".IFJcode23");
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

/* Destroy CODE */

/*
* MOVE ?var? ?symb?
* Prirazeni hodnoty do promenne zkopiruje hodnotu ?symb? do ?var?.
* Napr.MOVE LF@par GF@var provede zkopirovani hodnoty promenne var v glob�ln�m r�mci do promenne par v lokalnim ramci.
*/

/*
* CREATEFRAME
* Vytvor novy docasny ramec
* Vytvori novy docasny ramec a zahodi pripadny obsah puvodniho docasneho ramce.
*/

/*
* PUSHFRAME
* Presun docasneho ramce na zasobnik ramcu
* Presun TF na zasobnik ramcu.Ramec bude k dispozici pres LF a prekryje puvodni
* ramce na zasobniku ramcu.TF bude po provedeni instrukce nedefinovan a je treba jej
* pred dalsim pouzitim vytvorit pomoci CREATEFRAME. Pokus o pristup k nedefinovanemu ramci vede na chybu 55.
*/

/*
* POPFRAME
* P?esun aktu�ln�ho r�mce do do?asn�ho
* Presun vrcholovy ramec LF ze zasobniku ramcu do TF. 
* Pokud zadny ramec v LF neni k dispozici, dojde k chybe 55.
*/

/*
* DEFVAR
* ?var? Definuj novou prom?nnou v ramci
* Definuje promennou v urcenem ramci dle ?var?. Tato promenn� je zatim neinicializovana 
* a bez urceni typu, ktery bude urcen az prirazenim nejake hodnoty.
*/

/*
* CALL ?label?
* Skok na navesti s podporou navratu
* Ulozi inkrementovanou aktualni pozici z interniho citace instrukci do zasobniku volani
* a provede skok na zadan� navest�(pripadnou pripravu ramce musi zajistit jine instrukce).
*/

/*
* RETURN 
* Navrat na pozici ulozenou instrukci CALL
* Vyjme pozici ze zasobniku volani a skoci na tuto pozici nastavenim intern�ho citace
* instrukci(uklid lokalnich ramcu musi zajistit jine instrukce).Provedeni instrukce pri
* prazdnem zasobniku volani vede na chybu 56.
*/

/*
* PUSHS ?symb?
* Vlou hodnotu na vrchol datoveho zasobniku
* Ulozi hodnotu ?symb? na datovy zasobnik.
*/

/* POPS ?var?
* Vyjmi hodnotu z vrcholu datoveho zasobniku
* Neni-li z�sobnik prazdny, vyjme z nej hodnotu a ulozi ji do promenne?var?,
* jinak dojde k chybe 56.
*/

/*
* CLEARS
* Vymazani obsahu celeho datov�ho zasobniku
* Pomocna instrukce, ktera smaze cely obsah datov�ho zasobniku,
* aby neobsahoval zapomenute hodnoty z predchozich vypoctu.
*/

/*
* ADD ?var? ?symb1? ?symb2? 
* Soucet dvou ciselnych hodnot. Secte ?symb1? a ?symb2?
* (mus� b�t stejn�ho ?�seln�ho typu int nebo float) a v�slednou
* hodnotu tehoz typu ulozi do promenne ?var?.
*/

/*
* SUB ?var? ?symb1? ?symb2?
* Odecitani dvou ciselnych hodnot. Odecte ?symb2? od ?symb1?
* (musi byt stejneho ciselneho typu int nebo float)
* a vyslednou hodnotu tehoz typu ulozi do promenne ?var?.
*/

/* MUL ?var? ?symb1? ?symb2?
* Nasobeni dvou ciseln�ch hodnot Vyn�sob� ?symb1? a ?symb2?
* (mus� b�t stejn�ho ?�seln�ho typu int nebo float)
* a v�slednou hodnotu t�ho� typu ulo�� do promenn� ?var?.
*/

/*
* DIV ?var? ?symb1? ?symb2?
* Delen� dvou desetinn�ch hodnot. Podel� hodnotu ze ?symb1?
* druhou hodnotou ze ?symb2? (oba mus� b�t typu float)
* a v�sledek prirad� do promenn� ?var? (t� typu float).
* Delen� nulou zpusob� chybu 57.
*/

/*
* IDIV ?var? ?symb1? ?symb2?
* Delen� dvou celoc�seln�ch hodnot
* Celoc�selne podel� hodnotu ze ?symb1? druhou hodnotou ze ?symb2?
* (mus� b�t oba typu int) a v�sledek prirad� do promenn� ?var? typu int.
* Delen� nulou zpusob� chybu 57.
*/

/*
* ADDS / SUBS / MULS / DIVS / IDIVS
* Z�sobn�kov� verze instrukc� ADD, SUB, MUL, DIV a IDIV
*/

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

/*
* LTS / GTS / EQS
* Zasobnikov� verze instrukci LT / GT / EQ
*/

/*
* AND / OR / NOT ?var? ?symb1? ?symb2?
* Z�kladn� booleovsk� oper�tory
* Aplikuje konjunkci(logick� A) / disjunkci(logick� NEBO) 
* na operandy typu bool ?symb1? a ?symb2?
* nebo negaci na ?symb1? (NOT m� pouze 2 operandy)
* a v�sledek typu bool zap�e do ?var?.
*/

/*
* ANDS / ORS / NOTS
* Z�sobn�kov� verze instrukc� AND, OR a NOT
*/

/* 
* INT2FLOAT
* ?var? ?symb? Prevod celoc�seln� hodnoty na desetinnou
* Prevede celoc�selnou hodnotu ?symb? na desetinn� c�slo
* a ulo�� je do ?var?.
*/

/*
* FLOAT2INT ?var? ?symb?
* Prevod desetinn� hodnoty na celoc�selnou(oseknut�)
* Prevede desetinnou hodnotu ?symb?
* na celoc�selnou oseknut�m desetinn� c�sti a ulo��	ji do ?var?.
*/

/*
* INT2CHAR ?var? ?symb?
* Prevod cel�ho c�sla na znak
* c�seln� hodnota ?symb? je dle ASCII prevedena na znak, kter� tvor� 
* jednoznakov� retezec prirazen� do ?var?.
* Je-li ?symb? mimo interval[0; 255], dojde k chyb? 58.
*/

/*
* STRI2INT ?var? ?symb1? ?symb2?
* Ordin�ln� hodnota znaku.
* Do ?var? ulo�� ordin�ln� hodnotu znaku(dle ASCII)
* v retezci ?symb1? na pozici ?symb2? (indexov�no od nuly).
* Indexace mimo dan� ?et?zec vede na chybu 58.
*/

/*
* INT2FLOATS / FLOAT2INTS /	INT2CHARS / STRI2INTS
* Z�sobn�kov� verze konverzn�ch instrukc�
*/

/*
* CONCAT ?var? ?symb1? ?symb2?
* Konkatenace dvou retezcu.
* Do promenn� ?var? ulo�� retezec vznikl�
* konkatenac� dvou retezcov�ch operandu	?symb1? a ?symb2?
* (jin� typy nejsou povoleny).
*/

/*
* STRLEN ?var? ?symb?
* Zjisti d�lku retezce.
* Zjist� d�lku retezce v ?symb? a d�lka je ulo�ena jako
* cel� c�slo do ?var?.
*/

/*
* GETCHAR ?var? ?symb1? ?symb2?
* Vrat znak retezce.
* Do ?var? ulo�� retezec z jednoho znaku v retezci ?symb1?
* na pozici ?symb2? (indexov�no	cel�m ?�slem od nuly).
* Indexace mimo dan� ?et?zec vede na chybu 58.
*/

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

/*
* TYPE ?var? ?symb?
* Zjisti typ dan�ho symbolu.
* Dynamicky zjist� typ symbolu ?symb? a do ?var?
* zap�e retezec znac�c� tento typ
* (int, bool, float, string nebo nil).
* Je-li ?symb? neinicializovan� promenn�, oznac� jej� typ
* pr�zdn�m retezcem.
*/

/*
* LABEL ?label?
* Definice n�ve�t�.
* Speci�ln� instrukce oznacuj�c� pomoc� n�ve�t� ?label?
* dule�itou pozici v k�du jako potenci�ln� c�l libovoln� skokov� instrukce.
* Pokus o redefinici existuj�c�ho n�ve�t� je chybou 52.
*/

/*
* JUMP ?label? 
* Nepodm�nen� skok na n�ve�t�.
* Provede nepodm�nen� skok na zadan� n�ve�t� ?label?.
*/

/*
* JUMPIFEQ ?label? ?symb1? ?symb2?
* Podm�nen� skok na n�ve�t� pri rovnosti
* Pokud jsou ?symb1? a ?symb2? stejn�ho typu nebo 
* je nekter� operand nil(jinak chyba 53)
* a z�roven se jejich hodnoty rovnaj�, tak provede skok 
* na n�ve�t� ?label?.
*/

/*
* JUMPIFNEQ ?label? ?symb1? ?symb2?
* Podm�nen� skok na n�ve�t� pri nerovnosti.
* Jsou-li ?symb1? a ?symb2? stejn�ho typu
* nebo je n?kter� operand nil(jinak chyba 53),
* ale ruzn� hodnoty, tak provede skok na n�ve�t� ?label?.
*/

/*
* JUMPIFEQS / JUMPIFNEQS ?label?
* Z�sobn�kov� verze JUMPIFEQ, JUMPIFNEQ
* Z�sobn�kov� skokov� instrukce
* maj� i jeden operand mimo datov� z�sobn�k, a to n�ve�t� ?label?,
* na kter� se pr�padne provede skok.
*/

/*
* EXIT ?symb?
* Ukoncen� interpretace s n�vratov�m k�dem
* Ukonc� vykon�v�n� programu a ukonc� interpret
* s n�vratov�m k�dem ?symb?, kde ?symb? je cel� c�slo
* v intervalu 0 a� 49 (vcetne).
* Nevalidn� celoc�seln� hodnota ?symb? vede na chybu 57.
*/

/*
* BREAK
* V�pis stavu interpretu na stderr.
* Na standardn� chybov� v�stup(stderr) vyp�e stav interpretu
* v danou chv�li(tj.behem vykon�v�n� t�to instrukce). 
* Stav se mimo jin� skl�d� z pozice v k�du, v�pisu glob�ln�ho,
* aktu�ln�ho lok�ln�ho a do?asn�ho r�mce a poctu ji�
* vykonan�ch instrukc�.
*/

/*
* DPRINT ?symb?
* V�pis hodnoty na stderr.
* Vyp�e zadanou hodnotu ?symb? na standardn� chybov� v�stup(stderr).
* V�pisy touto instrukc� bude mo�n� vypnout pomoc� volby interpretu.
*/

/* func readString() -> String? */
/* func readInt() -> Int? */
/* func write(term1 , term2 , ..., termx) */
char* ConvertString(char* input)
{
	char* result = NULL;	
	for (int i = 0;i < strlen(input);i++)
	{
		if (result[i] == '\n')
		{

		}
	}


	return input;
}
void INBUILT_WRITE(Node* c_node)
{
	PrintCode("CREATEFRAME");
	PrintCode("PUSHFRAME");
	PrintCode("DEFVAR LF@%value");
	for (int i = 0; i < c_node->numChildren; i++)
	{
		PrintCode("MOVE LF@value string@"); //add string strcat(c_outputBuff, c_node->children[i]->content)

		PrintCode("WRITE LF@value");
	}
	
	PrintCode("POPFRAME");
}

/* func Int2Double(_ term : Int) -> Double */
/* func Double2Int(_ term : Double) */
/* func length(_ s : String) -> Int */
/* func substring(of s : String, startingAt i : Int, endingBefore j :Int) -> String? */
/* func ord(_ c : String) -> Int */
/* func chr(_ i : Int) -> String */
