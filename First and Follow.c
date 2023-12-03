/*
Program to find the first and follow given any grammar:

Initial grammar:
E -> TE'
E' -> +TE'
E' -> ?
T -> FT'
T' -> *FT'
T' -> ?
F -> id
F -> (E)

Replace above grammar lhs having prime with a new non-terminal symbol and id with i:
E -> TA
T -> ?
A -> +TA
A -> ?
T -> FB
B -> *FB
B -> ?
F -> i
F -> (E)

Expected results:
FIRST(E) = (i?+
FIRST(A) = ?+
FIRST(T) = (i?
FIRST(B) = ?*
FIRST(F) = (i
FOLLOW(E) = $)
FOLLOW(A) = $)
FOLLOW(T) = +$)
FOLLOW(B) = +$)
FOLLOW(F) = *+$)
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Store the grammar to find the first and follow
char grammar_lhs[] = {'E', 'T', 'A', 'A', 'T', 'B', 'B', 'F', 'F'};
char grammar_rhs[][10] = {"TA", "?", "+TA", "?", "FB", "*FB", "?", "i", "(E)"};

// char grammar_lhs[] = {'A', 'B', 'C', 'D', 'D'};
// char grammar_rhs[][10] = {"BCD", "?", "?", "i", "d"};

// grammar_count is the no of productions
int grammar_count = sizeof(grammar_lhs) / sizeof(grammar_lhs[0]);

// Array of all non_terminals in reverse order (of given grammar)
char non_terminals[10];
int non_terminals_count = 0;

// Matrices used to store first and follow of all non terminals in the grammar
char first_matrix[10][20];
char follow_matrix[10][20];

void first_all();
void first(char symbol, char first_list[], int *first_list_index);
void first_index(int grammar_index, char first_list[], int *first_list_index);

void follow_all();
void follow(char symbol, char follow_list[], int *follow_list_index);
void find_follow(int grammar_index, int grammar_rhs_index, int grammar_rhs_size, char follow_list[], int *follow_list_index);

int get_non_terminal_index(char non_terminal);
void remove_epsilon(char arr[], int *arr_len);
void join_sets(char set1[], int *set1_len, char set2[], int set2_len);
void set_enter(char set[], int *set_len, int ele);

bool is_terminal(char terminal);
bool if_epsilon_in(char arr[], int arr_len);

void main()
{
    first_all();
    for (int i = non_terminals_count - 1; i >= 0; i--)
    {
        printf("FIRST(%c) = %s\n", non_terminals[i], first_matrix[i]);
    }
    printf("\n");

    follow_all();
    for (int i = non_terminals_count - 1; i >= 0; i--)
    {
        printf("FOLLOW(%c) = %s\n", non_terminals[i], follow_matrix[i]);
    }
    printf("\n");
}

// Function that finds the first of all the non-terminals in the grammar and stores it in first_matrix
void first_all()
{
    char first_list[10];
    int first_list_index = 0;

    // Clear the first_matrix
    for (int i = 0; i < 10; i++)
    {
        memset(first_matrix[i], '\0', 20);
    }

    // Find all the non terminals present in the grammar and store them in non_terminals global array
    for (int i = 0; i < grammar_count; i++)
    {
        set_enter(non_terminals, &non_terminals_count, grammar_lhs[grammar_count - i - 1]);
    }

    // Fill the first_matrix
    for (int i = 0; i < non_terminals_count; i++)
    {
        first(non_terminals[i], first_list, &first_list_index);
        for (int j = 0; j < first_list_index; j++)
        {
            first_matrix[i][j] = first_list[j];
        }
        memset(first_list, '\0', sizeof(first_list));
        first_list_index = 0;
    }
}

// Function that finds the first of a given symbol
void first(char symbol, char first_list[], int *first_list_index)
{
    // If the symbol is a terminal then first of that is the terminal itself
    if (is_terminal(symbol) || symbol == '?')
    {
        first_list[*first_list_index] = symbol;
        (*first_list_index)++;
    }

    // If its non-terminal then we have to first of all the production whose LHS is the symbol
    else
    {
        for (int grammar_index = grammar_count - 1; grammar_index >= 0; grammar_index--)
        {
            if (grammar_lhs[grammar_index] == symbol)
            {
                first_index(grammar_index, first_list, first_list_index);
            }
        }
    }
}

// Function that finds the first given the grammar_index (production index)
void first_index(int grammar_index, char first_list[], int *first_list_index)
{
    char temp_first_list[10];
    int temp_first_list_index = 0;
    int grammar_rhs_size = strlen(grammar_rhs[grammar_index]);
    int grammar_rhs_index = 0;
    int non_terminal_index = 0;

    // If first symbol of production's RHS is terminal then return that as the first of that production
    if (is_terminal(grammar_rhs[grammar_index][0]))
    {
        first_list[*first_list_index] = grammar_rhs[grammar_index][0];
        (*first_list_index)++;
        first_list[*first_list_index] = '\0';
    }

    // Otherwise iterate through the RHS of the production until we reach a terminal or the end of the production
    else
    {
        while (grammar_rhs_index < grammar_rhs_size)
        {
            non_terminal_index = get_non_terminal_index(grammar_rhs[grammar_index][grammar_rhs_index]);

            if (non_terminal_index == -1)
            {
                printf("Non terminal without production found in grammar's RHS\n");
                exit(0);
            }

            // if non-terminal is found in RHS then take its first and store in temp_first_list_index
            join_sets(temp_first_list, &temp_first_list_index, first_matrix[non_terminal_index], strlen(first_matrix[non_terminal_index]));

            // if epsilon not present in its first then we can take union of first_list with temp_first_list to obtain the result
            if (!if_epsilon_in(temp_first_list, temp_first_list_index) || grammar_rhs_index == grammar_rhs_size - 1)
            {
                join_sets(first_list, first_list_index, temp_first_list, temp_first_list_index);
                break;
            }

            // Remove the epsilon that's present and then take union of first_list with temp_first_list
            remove_epsilon(temp_first_list, &temp_first_list_index);
            join_sets(first_list, first_list_index, temp_first_list, temp_first_list_index);

            // Clear the temp_first_list and repeat
            memset(temp_first_list, '\0', temp_first_list_index);
            temp_first_list_index = 0;
            grammar_rhs_index++;
        }
    }
}

// Function that finds the follow of all the non-terminals in the grammar and stores it in follow_matrix
void follow_all()
{
    char follow_list[10];
    int follow_list_index = 0;

    // Clear the follow_matrix
    for (int i = 0; i < 10; i++)
    {
        memset(follow_matrix[i], '\0', 20);
    }

    // Fill the follow_matrix
    for (int i = non_terminals_count - 1; i >= 0; i--)
    {
        follow(non_terminals[i], follow_list, &follow_list_index);
        for (int j = 0; j < follow_list_index; j++)
        {
            follow_matrix[i][j] = follow_list[j];
        }
        memset(follow_list, '\0', sizeof(follow_list));
        follow_list_index = 0;
    }
}

// Function that finds the follow given a non-terminal
void follow(char symbol, char follow_list[], int *follow_list_index)
{
    int grammar_rhs_size = 0;

    // If the symbol is starting symbol then we append '$' to its follow
    if (symbol == grammar_lhs[0])
    {
        follow_list[*follow_list_index] = '$';
        (*follow_list_index)++;
    }

    // For all instances of symbol in RHS of productions, we find its follow
    for (int grammar_index = 0; grammar_index < grammar_count; grammar_index++)
    {
        grammar_rhs_size = strlen(grammar_rhs[grammar_index]);

        for (int grammar_rhs_index = 0; grammar_rhs_index < grammar_rhs_size; grammar_rhs_index++)
        {
            if (grammar_rhs[grammar_index][grammar_rhs_index] == symbol)
            {
                find_follow(grammar_index, grammar_rhs_index, grammar_rhs_size, follow_list, follow_list_index);
            }
        }
    }
}

// Function that finds the follow of a non-terminal given its location on RHS of production
void find_follow(int grammar_index, int grammar_rhs_index, int grammar_rhs_size, char follow_list[], int *follow_list_index)
{
    int gr_index = grammar_rhs_index + 1;
    int non_terminal_index;
    char temp_follow_list[10];
    int temp_follow_list_index = 0;

    // If the next symbol is end of the production then we append the follow of that production's LHS non-terminal
    if (gr_index == grammar_rhs_size)
    {
        non_terminal_index = get_non_terminal_index(grammar_lhs[grammar_index]);
        join_sets(follow_list, follow_list_index, follow_matrix[non_terminal_index], strlen(follow_matrix[non_terminal_index]));
    }
    // If the next symbol is terminal then that is the follow of the given non-terminal
    else if (is_terminal(grammar_rhs[grammar_index][gr_index]))
    {
        follow_list[*follow_list_index] = grammar_rhs[grammar_index][gr_index];
        (*follow_list_index)++;
    }
    // When the next symbol is a non-terminal we take its first
    else
    {
        non_terminal_index = get_non_terminal_index(grammar_rhs[grammar_index][gr_index]);

        // If epsilon is present in first of next non-terminal then we remove it, append it and then append follow of that non-terminal to follow_list
        if (if_epsilon_in(first_matrix[non_terminal_index], strlen(first_matrix[non_terminal_index])))
        {
            join_sets(follow_list, follow_list_index, first_matrix[non_terminal_index], strlen(first_matrix[non_terminal_index]));
            remove_epsilon(follow_list, follow_list_index);

            find_follow(grammar_index, gr_index, grammar_rhs_size, temp_follow_list, &temp_follow_list_index);
            join_sets(follow_list, follow_list_index, temp_follow_list, temp_follow_list_index);
        }
        // If epsilon is not present then the first of next non-terminal is appended to follow_list
        else
        {
            join_sets(follow_list, follow_list_index, first_matrix[non_terminal_index], strlen(first_matrix[non_terminal_index]));
        }
    }
}

// Function to get the index of given non-terminal in the non_terminals list
// Returns -1 if non-terminal given is not present in non_terminals list
int get_non_terminal_index(char non_terminal)
{
    for (int i = 0; i < non_terminals_count; i++)
    {
        if (non_terminals[i] == non_terminal)
        {
            return i;
        }
    }
    return -1;
}

bool is_terminal(char terminal)
{
    if (terminal == '+' || terminal == '-' || terminal == '*' || terminal == '/' ||
        terminal == '(' || terminal == ')' || terminal == '?')
    {
        return true;
    }
    if (terminal <= 122 && terminal >= 97)
    {
        return true;
    }
    return false;
}

bool if_epsilon_in(char arr[], int arr_len)
{
    for (int i = 0; i < arr_len; i++)
    {
        if (arr[i] == '?')
        {
            return true;
        }
    }
    return false;
}

void remove_epsilon(char arr[], int *arr_len)
{
    char temp_arr[10];
    int temp_arr_index = 0;

    for (int i = 0; i < *arr_len; i++)
    {
        if (arr[i] != '?')
        {
            temp_arr[temp_arr_index] = arr[i];
            temp_arr_index++;
        }
    }
    for (int i = 0; i < temp_arr_index; i++)
    {
        arr[i] = temp_arr[i];
    }

    (*arr_len) = temp_arr_index;
}

// Function that takes union of two arrays and returns the resultant union in the first array
void join_sets(char set1[], int *set1_len, char set2[], int set2_len)
{
    // Iterate though set 2 and join to set 1 using set_enter function
    for (int i = 0; i < set2_len; i++)
    {
        set_enter(set1, set1_len, set2[i]);
    }
}

void set_enter(char set[], int *set_len, int ele)
{
    for (int i = 0; i < *set_len; i++)
    {
        if (set[i] == ele)
        {
            return;
        }
    }
    set[*set_len] = ele;
    (*set_len)++;
}
