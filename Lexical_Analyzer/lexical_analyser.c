#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char INPUT_FILE[] = "dummy.c";
char KEYWORDS_FILE[] = "keywords.txt";

int read_file(char file_name[], char buffer[][10]);
bool char_found(char string[], char character);
void operator_found(char string[]);
bool operator_present(char string[]);
bool if_keyword(char buffer[]);
bool if_identifier(char buffer[]);
void tokens_separator(char c_stream[]);

bool single_quote_flag = false;
bool double_quote_flag = false;
bool recursion_flag = false;
char token[100];

char keywords[50][10];
const char separators[] = "{}();";
char *dq_literal_save_ptr = NULL;
char *sq_literal_save_ptr = NULL;
int keywords_count=0;

int read_file(char file_name[], char buffer[][10]){
	FILE *fp = fopen(file_name, "r");
	int buffer_size = 0;
	
	if (fp == NULL){
		printf("%s file has some error loading...\n", file_name);
		exit(0);
	}
	
	while (!feof(fp)){
		fscanf(fp, "%s", buffer[buffer_size]);
		buffer_size++;		
	}
	
	return buffer_size;
}

bool char_found(char string[], char character){
    for (int i=0; i<strlen(string); i++){
        if (string[i] == character){
            return true;
        }
    }
    return false;
}

void operator_found(char string[]){
    char substring[50];
    int subsindex = 0;
    for (int i=0; i<strlen(string); i++){
        switch (string[i]){
        case '>':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf(">=\tGreater than or equal to operator\n");
            }else{
                printf(">\tGreater than operator\n");
            }
        break;
        case '<':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("<=\tLess than or equal to operator\n");
            }else{
                printf("<\tLess than operator\n");
            }
        break;
        case '=':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("==\tEqual to comparison operator\n");
            }else{
                printf("=\tAssignment operator\n");
            }
        break;
        case '!':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("!=\tNot equal to operator\n");
            }else{
                printf("!\tNot logical operator\n");
            }
        break;
        case '+':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("+=\tAddition assignment operator\n");
            }else{
                if (string[i+1] == '+'){
                    i++;
                    printf("++\tIncrement operator\n");
                }else{
                    printf("+\tAddition operator\n");
                }
            }
        break;
        case '-':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("-=\tSubtraction assignment operator\n");
            }else{
                if (string[i+1] == '-'){
                    i++;
                    printf("--\tDecrement operator\n");
                }else{
                    printf("-\tSubtraction operator\n");
                }
            }
        break;
        case '*':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("*=\tMultiplication assignment operator\n");
            }else{
                printf("*\tMultiplication operator\n");
            }
        break;
        case '/':
            substring[subsindex] = '\0';
            recursion_flag = true;
            tokens_separator(substring);
            recursion_flag = false;
            memset(substring, 0, 50);
            subsindex = 0;
            if (string[i+1] == '='){
                i++;
                printf("/=\tDivision assignment operator\n");
            }else{
                printf("/\tDivision operator\n");
            }
        break;
        default:
            substring[subsindex++] = string[i];
            break;
        }
        substring[subsindex] = '\0';
        recursion_flag = true;
        tokens_separator(substring);
        recursion_flag = false;
        memset(substring, 0, 50);
    }
}

bool operator_present(char string[]){
    if (char_found(string, '+') || char_found(string, '-') 
            || char_found(string, '=') || char_found(string, '/') 
            || char_found(string, '>') || char_found(string, '<') 
            || char_found(string, '*') || char_found(string, '%')){
        return true;
    }
    return false;
}

bool if_keyword(char buffer[]){
	for (int i=0; i<keywords_count; i++){
		if (strcmp(keywords[i], buffer) == 0){
			printf("%s\tKeyword\n", buffer);
			return true;
		}
	}
	return false;
}

bool if_identifier(char buffer[]){
	if (buffer[0] == '_' || isalpha(buffer[0])){
		printf("%s\tIdentifier\n", buffer);
		return true;
	}
	return false;
}

void tokens_separator(char c_stream[]){
    char *save_ptr = NULL;
    char *tokens;
    
    if (!single_quote_flag && !double_quote_flag && !double_quote_flag && !single_quote_flag && !recursion_flag){
        if (char_found(c_stream, '(')){
            printf("(\tLeft Paranthesis\n");
        }
        if (char_found(c_stream, ')')){
            printf(")\tRight Paranthesis\n");
        }
        if (char_found(c_stream, ';')){
            printf(";\tSemi colon\n");
        }
    }

    if (char_found(c_stream, '"') && !single_quote_flag){
        if (double_quote_flag){
            c_stream = strtok(c_stream, "\");");
            if (c_stream != NULL){
                printf("%s\"\tString\n", c_stream);
            }else{
                printf("\"\tString\n");
            }
            double_quote_flag = false;
            return;
        }else{
            if (c_stream[strlen(c_stream)-1] == ';'){
                c_stream = strtok_r(c_stream, "\"", &dq_literal_save_ptr);
                recursion_flag = true;
                tokens_separator(c_stream);
                recursion_flag = false;
                c_stream = strtok_r(NULL, "\"", &dq_literal_save_ptr);
                printf("\"%s\"\tString\n", c_stream);
                return;
            }
            c_stream = strtok_r(c_stream, "\"", &dq_literal_save_ptr); 
            recursion_flag = true;
            tokens_separator(c_stream);
            recursion_flag = false;
            c_stream = strtok_r(NULL, "\"", &dq_literal_save_ptr);
            printf("\"%s ", c_stream);
            double_quote_flag = true;
            return;
        }
    }else{
        if (double_quote_flag){
            printf("%s ", c_stream);
            return;
        }
    }

    if (char_found(c_stream, '\'') && !double_quote_flag){
        c_stream = strtok_r(c_stream, "'", &sq_literal_save_ptr);
        single_quote_flag = true;
    }

    tokens = strtok_r(c_stream, separators, &save_ptr); 
    while (tokens != NULL){
        if (tokens[0] == '#' && tokens[strlen(tokens)-1] == '>'){
            printf("#include\tKeyword\n");
            strncpy(token, tokens+8, strlen(tokens)-8);
            printf("%s\tIdentifier\n", token);
            return;
        }

        if (operator_present(tokens)){
            if (tokens[0] == '<' && tokens[strlen(tokens)-1] == '>'){
                printf("%s\tIdentifier\n", tokens);
                return;
            }else{
                operator_found(tokens);
            }
        }else{
            if (!if_keyword(tokens)){
                if (!if_identifier(tokens)){
                    printf("%s\tLiteral\n", tokens);
                }
            }
        }
        tokens = strtok_r(NULL, separators, &save_ptr);
    }

    if (single_quote_flag){
        c_stream = strtok_r(NULL, "'", &sq_literal_save_ptr);
        if (strlen(c_stream) > 1){
            perror("Not a valid character\n");
            exit(0);
        }
        printf("'%s'\tCharacter\n", c_stream);
        single_quote_flag = false;
    }

}

void main(){
	FILE *input = fopen(INPUT_FILE, "r");
	char c_stream[100];
	
	keywords_count = read_file(KEYWORDS_FILE, keywords);

	while (!feof(input)){
		fscanf(input, "%s", c_stream);
		tokens_separator(c_stream);
	}

	fclose(input);
}









