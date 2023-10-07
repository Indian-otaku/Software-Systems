#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
Example grammar used:  
	E -> E + E
	E -> E * E
	E -> (E)
	E -> id
*/

// Enter the left hand side of the grammar as an array of characters starting with the starting symbol.
char grammar_lhs[] = {'E', 'E', 'E', 'E'};
// Enter the right hand side of the grammar as array of strings in correct order.
char grammar_rhs[][10] = {"E+E", "E*E", "(E)", "id"};

char grammar_length = sizeof(grammar_lhs);

char input[100] = "$";
char stack[100]; // Stack also behaves as a string. 
int stack_length = 0;

// For pushing characters into stack. 
void stack_push(char c) {
	stack[stack_length] = c;
	stack_length++;
	stack[stack_length] = '\0';
}

// For popping characters from stack. 
char stack_pop() {
	char c = stack[stack_length];
	stack[stack_length] = '\0';
	stack_length--;
	return c;
}

// Shift action.
void shift(int *i) { // i is taken as pointer because we want to increment it twice in case the token is id. 
  // If the input token is id then we push i and d as separate tokens into stack instead of a single token. 
	if (input[*i] == 'i' && input[*i+1] == 'd') {
		stack_push('i');
		stack_push('d');
		++(*i);
		printf("Shifted for id: %s\n",  stack);
	}
  // Otherwise we can push token into the stack as it is. 
	else {
		stack_push(input[*i]);
		printf("Shifted for %c: %s\n", input[*i],  stack);
	}
}

// Function that checks the top of the stack before reducing. If reducing is not possible then -1 is returned. 
int check_top_of_stack() {
	char top_of_stack[10];
	for (int i=stack_length-1; i>0; i--) {
		strncpy(top_of_stack, stack+i, stack_length-i+1);
		for (int j=0; j<grammar_length; j++) {
			if (strcmp(top_of_stack, grammar_rhs[j]) == 0){
				return j;
			}
		}
	}
	return -1;
}

// Reduce action. 
void reduce() {
	int pos = check_top_of_stack();
	if (pos != -1) {
		for (int i=0; i<strlen(grammar_rhs[pos]); i++) {
			stack_pop();
		}
		stack_push(grammar_lhs[pos]);
		printf("Stack after reducing %c->%s : %s\n", grammar_lhs[pos], grammar_rhs[pos], stack);
    
    // After reducing once we recursively check if it is possible to reduce the top of stack again until reducing is not possible. 
		reduce();
	}
}

void main() {
	char string[100];

  // We get the input string from user and concatenates $ at the end of it. 
	printf("Enter the input: ");
	scanf("%s", string);
	strcat(input, string);
	strcat(input, "$");

  // Initially we push the $ sign into the stack. 
	stack_push(input[0]);

  // Until the end of the input string we repeat the process of shift and reducing until either the grammar is accepted or rejected. 
	for (int i=1; i<strlen(input); i++) {
		reduce();
		if (input[i] == '$') {
			if (stack[0] == '$' && stack[1] == grammar_lhs[0] && stack[2] == '\0') {
				printf("The input string is accepted by the grammar\n");
			}
			else {
				printf("The input string is not accepted by the grammar\n");
			}
			exit(0);
		}
		shift(&i);
	}
}
