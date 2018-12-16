#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
bool debugMode = false;	// toggles debug mode

typedef struct stack {
    char *stack;
    int size;
    int top;
} stack;


// initialize the stack with 2 positions
void initializeStack(stack* s) {
    s->size = 2;    
    s->stack = (char*)malloc(2*sizeof(char)); 
    s->top = -1;  // empty stack index -1
}

void clearStack(stack* s) {
    free(s->stack);
    initializeStack(s);
}

void push(stack* s, char symbol) {
    int prevSize = 0;
    int i;
    
    // the stack is full, grow the array
    if (s->top == s->size) {
        char *newStack = (char*)malloc((s->size+2)*sizeof(char));   // grow the stack by 2
        if (debugMode == true) {
            printf("***STACK RESIZE***\n");
            printf("The size of the old stack is: %d\n", prevSize);
            printf("The size of the new allocated stack is: %d\n", prevSize+2);
            printf("There are %d elements being copied over from the old stack.\n", s->top+1);
        }
        for (i = 0; i <= s->size; i++) {  // copy the values into the new array
            newStack[i] = s->stack[i];
        }
        newStack[s->top+1] = symbol;      // top of the stack will be the new symbol
        free(s->stack);                   // deallocate the full stack
        s->stack = newStack;              // adjust the pointer to the new stack
        s->size = s->size+2;              // update stack size
    }
    // the stack is not full
    else {
        s->stack[s->top+1] = symbol;      // add the symbol to the top
    }
    s->top++;
    if (debugMode == true) {
        printf("%c is being pushED onto the stack.\n", symbol);
    }
}

void pop(stack* s) {
    if (debugMode == true) {
        char c = s->stack[s->top];
        printf("%c is being popPED off the stack.\n", s->stack[s->top]);
    }
    // reset the top
    s->top = s->top-1;
}

bool isEmpty(stack* s) {
    if (s->top == -1) 
        return true;
    else 
        return false;
}

char top(stack* s) {
    return s->stack[s->top];
}

// checks if an opening symbol is encountered
bool openingSymbol(char symbol) {
    if (symbol == '(') return true;
    if (symbol == '[') return true;
    if (symbol == '<') return true;
    if (symbol == '{') return true;
    else
        return false;
}

// checks if a closing symbol is encountered
bool closingSymbol(char symbol) {
    if (symbol == ')') return true;
    if (symbol == ']') return true;
    if (symbol == '}') return true;
    if (symbol == '>') return true;
    else
        return false;
}

// returns the corresponding symbol
char correspondingSymbol(char symbol) {
    if(symbol == '(') return ')';
    if(symbol == ')') return '(';
    
    if(symbol == '{') return '}';
    if(symbol == '}') return '{';
    
    if(symbol == '[') return ']';
    if(symbol == ']') return '[';
    
    if(symbol == '<') return '>';
    if(symbol == '>') return '<';
    
    return '\0';
}

// moves ^ to the appropriate place in the expression for error display
void displayError(char *expression, int index) {
    int j;
    printf("%s", expression);
    for (j = 0; j < index; j++) {
        printf(" ");
    }
}


int main(int argc, const char * argv[]) {
    stack symbolStack;      // Declare struct the_stack of type stack
    initializeStack(&symbolStack);  // Initialize the stack
    char expression[300];        // Stores input
    int i = 0;
    
    // check if debug mode has been toggled from the command line args
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            debugMode = true;
        else if (strcmp(argv[i], "-D")== 0)
            debugMode = true;
    }
    
    printf("Welcome to the Balance Checker!\n");
    bool runChecker = true;     // runs the balance checker while input is not Q
    while (runChecker) {
        printf("Enter an expression or Q to exit the program: ");
        fgets(expression, 300 ,stdin);
        
        // exit program if Q is entered
        if ((expression[0] == 'q' || expression[0] == 'Q') && expression[1] == '\n') {
            runChecker = false;
            exit(0);
        }

        while (expression[i] != '\0') {
            // check for opening symbols
            if (openingSymbol(expression[i])) {
                push(&symbolStack, expression[i]);  // push the symbol to the top of the stack
            }
            // check for closing symbols
            else if (closingSymbol(expression[i])) {
                // if the top of the stack matches the corresponding closing symbol, pop it off
                if (top(&symbolStack) == correspondingSymbol(expression[i])) {
                    pop(&symbolStack);
                }
                // expecting a different closing symbol -- wrong symbol at the top of the stack
                else if ((expression[i] != correspondingSymbol(top(&symbolStack))) && !isEmpty(&symbolStack)) {
                    displayError(expression, i);
                    printf("^ Expecting %c\n", correspondingSymbol(top(&symbolStack)));
                    break;
                }
                // there is a missing symbol in the expression 
                else if(isEmpty(&symbolStack) && closingSymbol(expression[i])) {
                    displayError(expression, i);
                    printf("^ Missing %c\n", correspondingSymbol(expression[i]));
                    break;
                }
            }
            i++;   // next char
        }
        
        // if the stack is empty after the expression has ended, it is balanced
        if(isEmpty(&symbolStack) && expression[i] == '\0') {
            printf("%s", expression);
            printf("The expression is balanced.\n");
        }
        // if the stack is not empty after the expression has ended, there is a missing closing symbol
        else if(!isEmpty(&symbolStack) && expression[i] == '\0') {
            displayError(expression, strlen(expression));
            printf("^ Missing %c\n", correspondingSymbol(top(&symbolStack)));
        }
        
        clearStack(&symbolStack);
        }
    
    return 0;
}
