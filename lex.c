#include "lex.h"

void AddToken(MachineStates state, wordStr **Tokens, char *word)
{
    if((*Tokens)->type != NULL)
    {
        (*Tokens)->next = malloc(sizeof(wordStr));
        (*Tokens) = (*Tokens)->next;
        (*Tokens)->next = NULL;
    }
    if(state == String_end)
    {
        (*Tokens)->type = "string";
        (*Tokens)->content = EscapeSequence(word);
    }
    else if(state == Compare_end || state == Operator || state == Operator3 || state == Operator4 )
    {
        (*Tokens)->type = "operator";
        (*Tokens)->content = word;
    }
    else if(state == Open_bracket)
    {
        (*Tokens)->type = "openBracket";
        (*Tokens)->content = word;
    }
    else if(state == Close_bracket)
    {
        (*Tokens)->type = "closeBracket";
        (*Tokens)->content = word;
    }
    else if(state == Open_block)
    {
        (*Tokens)->type = "openBlock";
        (*Tokens)->content = word;
    }
    else if(state == Close_block)
    {
        (*Tokens)->type = "closeBlock";
        (*Tokens)->content = word;
    }
    else if(state == Colon)
    {
        (*Tokens)->type = "colon";
        (*Tokens)->content = word;
    }
    else if(state == Comma)
    {
        (*Tokens)->type = "comma";
        (*Tokens)->content = word;
    }
    else if(state == Assign)
    {
        (*Tokens)->type = "assign";
        (*Tokens)->content = word;
    }
    else if(state == Compare || state == Compare2)
    {
        (*Tokens)->type = "compare";
        (*Tokens)->content = word;
    }
    else if(state == ExclamationMark)
    {
        (*Tokens)->type = "exclamationMark";
        (*Tokens)->content = word;
    }
    else if(state == Identifier)
    {
        if(Keyword(word))
        {
            (*Tokens)->type = "keyword";
            (*Tokens)->content = word;
        }
        else if(Type(word))
        {
            (*Tokens)->type = "identifier(type)";
            (*Tokens)->content = word;
        }
        else
        {
            (*Tokens)->type = "identifier";
            (*Tokens)->content = word;
        }
    }
    else if(state == Identifier_type)
    {
        if(Type(word))
        {
            (*Tokens)->type = "identifier(type)";
            (*Tokens)->content = word;
        }
        else
        {
            exit(1);
            //TODO: Print error unknown type
        }
    }
    else if (state == Integer)
    {
        long int i = strtol(word, NULL, 10);
        if ((i == LONG_MAX || i == LONG_MIN) && errno == ERANGE)
        {
            exit(1);
        }
        (*Tokens)->type = "integer";
        (*Tokens)->content = word;
    }
    else if (state == Double || state == Double_exp_op2)
    {
        strtod(word, NULL);
        if(errno == ERANGE)
        {
            exit(1);
        }
        (*Tokens)->type = "double";
        (*Tokens)->content = word;
    }
    else if (state == Arrow)
    {
        (*Tokens)->type = "arrow";
        (*Tokens)->content = word;
    }
    else if (state == Newline)
    {
        (*Tokens)->type = "newline";
        (*Tokens)->content = "newline";
    }
    else if (state == Identifier_name)
    {
        (*Tokens)->type = "identifier(name)";
        (*Tokens)->content = word;
    }
}

void PrintWordList(wordStr *wordList)
{
    while (wordList != NULL)
    {
        //printf("%s %s\n", wordList->type, wordList->content);
        wordList = wordList->next;
    }
}

bool Keyword(char* word)
{
    if (strcmp(word, "else") == 0 || strcmp(word, "func") == 0 || strcmp(word, "if") == 0 
    || strcmp(word, "let") == 0 || strcmp(word, "nil") == 0 || strcmp(word, "return") == 0
    || strcmp(word, "var") == 0 || strcmp(word, "while") == 0)
        return true;
    return false;
}

bool Type(char* word)
{
    if (strcmp(word, "Double") == 0 || strcmp(word, "Double?") == 0 || strcmp(word, "Int") == 0 
    || strcmp(word,"Int?") == 0 || strcmp(word, "String") == 0 || strcmp(word, "String?") == 0)
        return true;
    return false;
}

char* EscapeSequence(char *word)
{
    int lenght = strlen(word);
    char *tmp = malloc(sizeof(char) * lenght);
    int tmpIndex = 0;
    for (int i = 1; i < lenght-1; i++)
    {
        if (word[i] == '\\')
        {
            i++;
            if (word[i] == 'n')
            {
                tmp[tmpIndex] = '\n';
                tmpIndex += 1;
            }
            else if (word[i] == 't')
            {
                tmp[tmpIndex] = '\t';
                tmpIndex += 1;
            }
            else if (word[i] == '"')
            {
                tmp[tmpIndex] = '"';
                tmpIndex += 1;
            }
            else if (word[i] == '\\')
            {
                tmp[tmpIndex] = '\\';
                tmpIndex += 1;
            }
            else if (word[i] == 'u')
            {
                tmp[tmpIndex] = HexEscape(word, &i, &lenght);
                tmpIndex += 1;
            }
            else
            {
                // TODO: Print error
                exit(1);
            }
        }
        else
        {
            tmp[tmpIndex] = word[i];
            tmpIndex += 1;
        }
    }
    free(word);
    tmp = realloc(tmp, sizeof(char) * tmpIndex);
    return tmp;
}

char HexEscape(char *word, int *i, int *lenght)
{
    char *hex = malloc(sizeof(char) * 9);
    int hexIndex = 0;
    *i += 1;
    if (word[*i] == '{')
    {
        for (*i += 1; *i < *lenght; *i += 1)
        {
            if (isxdigit(word[*i]))
            {
                if (hexIndex == 7)
                {
                    // TODO: print error escape sequence
                    exit(1);
                }
                hex[hexIndex] = word[*i];
                hexIndex += 1;
            }
            else if (word[*i] == '}')
            {
                if (hexIndex == 0)
                {
                    // TODO: print error escape sequence
                    exit(1);
                }
                hex[hexIndex] = '\0';
                break;
            }
            else
            {
                // TODO: print error escape sequence
                    exit(1);
            }
        }
        int tmp = strtol(hex, NULL, 16);

        if (tmp <= 255)
        {
            char tmp2 = tmp;
            return tmp2;
        }
        else
        {
            //TODO: print error escape sequence
            exit(1);
        }
    }
    else
    {
        // TODO: print error escape sequence
        exit(1);
    }
    exit(1);
}

void Tokenizer(wordStr *LastToken)
{
    MachineStates currentState = Start;
    MachineStates nextState = Start;
    size_t bufferSize = 32;
    size_t bufferLenght = 0;
    char *buffer;
    while (true)
    {
        char input = getchar();
        nextState = StateMachine(input, currentState);
        if (currentState == Start)
        {
            if (nextState == Start)
                continue;
            buffer = (char *)malloc(bufferSize);
        }
        if (nextState == Error)
        {
            // TODO: Print lex error
            exit(1);
        }
        else if (nextState == End)
        {
            buffer[bufferLenght] = '\0';
            if (currentState != Comment_one_line && currentState != Comment_multi_line_end2)
                AddToken(currentState, &LastToken, buffer);
            else
                free(buffer);
            nextState = Start;
            bufferSize = 32;
            bufferLenght = 0;
            ungetc(input, stdin);
        }
        else if (nextState == Comment_one_line)
        {
            while (input != '\n' && input != EOF)
            {
                input = getchar();
            }
            ungetc(input, stdin);
        }
        else if (nextState == Endoffile)
        {
            break;
        }
        else
        {
            if (bufferSize == bufferLenght)
            {
                char *temp = realloc(buffer, bufferSize * 2);
                if (temp == NULL)
                {
                    // TODO: Print memory error
                    exit(99);
                }
                bufferSize *= 2;
                buffer = temp;
            }
            buffer[bufferLenght] = input;
            bufferLenght++;
        }
        currentState = nextState;
    }
}

MachineStates StateMachine(char input, MachineStates currentState)
{
    switch (currentState)
    {
    case Start:
        if (input == '"')
            return String_start;
        if (input == '/')
            return Operator3;
        if (input == '?')
            return Compare_start;
        if (input == ')')
            return Close_bracket;
        if (input == '(')
            return Open_bracket;
        if (input == '{')
            return Open_block;
        if (input == '}')
            return Close_block;
        if (input == ':')
            return Colon;
        if (input == ',')
            return Comma;
        if (input == '*' || input == '+')
            return Operator;
        if (input == '=')
            return Assign;
        if (input == '<' || input == '>')
            return Compare;
        if (input == '!')
            return ExclamationMark;
        if (isalpha(input))
            return Identifier;
        if (input == '_')
            return Identifier_name;
        if (isdigit(input))
            return Integer;
        if (input == '-')
            return Operator4;
        if (input == '\n')
            return Newline;
        if (input == EOF)
            return Endoffile;
        if (isspace(input))
            return Start;
        return Error;
    case String_start:
        if (input > 31 && input != '"' && input != '\\')
            return String_start;
        if (input == '\\')
            return String_escape;
        if (input == '"')
            return String_end;
        return Error;
    case String_escape:
        return String_start;
    case String_end:
        return End;
    case Operator3:
        if (input == '/')
            return Comment_one_line;
        if (input == '*')
            return Comment_multi_line_start;
        return End;
    case Comment_one_line:
        return End;
    case Comment_multi_line_start:
        if (input == '*')
            return Comment_multi_line_end1;
        return Comment_multi_line_start;
    case Comment_multi_line_end1:
        if (input == '/')
            return Comment_multi_line_end2;
        return Comment_multi_line_start;
    case Comment_multi_line_end2:
        return End;
    case Compare_start:
        if (input == '?')
            return Compare_end;
        return Error;
    case Compare_end:
        return End;
    case Close_bracket:
        return End;
    case Open_bracket:
        return End;
    case Open_block:
        return End;
    case Close_block:
        return End;
    case Colon:
        return End;
    case Comma:
        return End;
    case Operator:
        return End;
    case Assign:
        if (input == '=')
            return Compare2;
        return End;
    case Compare:
        if (input == '=')
            return Compare2;
        return End;
    case ExclamationMark:
        if (input == '=')
            return Compare2;
        return End;
    case Compare2:
        return End;
    case Identifier:
        if (isdigit(input) || isalpha(input) || input == '_')
            return Identifier;
        if (input == '?')
            return Identifier_type;
        return End;
    case Identifier_name:
        if (isdigit(input) || isalpha(input) || input == '_')
            return Identifier;
        return End;
    case Identifier_type:
        return End;
    case Integer:
        if (isdigit(input))
            return Integer;
        if (input == '.')
            return Double_start;
        if (input == 'E' || input == 'e')
            return Double_exp;
        return End;
    case Double_start:
        if (isdigit(input))
            return Double;
        return Error;
    case Double:
        if (input == 'E' || input == 'e')
            return Double_exp;
        return End;
    case Double_exp:
        if (input == '+' || input == '-')
            return Double_exp_op;
        if (isdigit(input))
            return Double_exp;
        return Error;
    case Double_exp_op:
        if (isdigit(input))
            return Double_exp_op2;
        return Error;
    case Double_exp_op2:
        if (isdigit(input))
            return Double_exp_op2;
        return End;
    case Operator4:
        if (input == '>')
            return Arrow;
        return End;
    case Arrow:
        return End;
    case Error:
        return Error;
    case End:
        return Error;
    case Newline:
        return End;
    case Endoffile:
        return Error;
    }
    return Error;
}