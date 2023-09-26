typedef struct{
	char* content;
	struct word *next;
}word;
typedef struct {
	struct word *firstWord;
} wordList;