#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool debugMode = false;

typedef struct mazeStruct {
    char arr[32][32];  // Allows for a maze of size 30x30 plus outer walls
    int xsize, ysize;
    int xstart, ystart;
    int xend, yend;
} maze;

typedef struct node {
    int x;
    int y;
    struct node *next;
} node;


// Prepends a node to the linked list after visiting the location
node* PushNode (node* head, int xdata, int ydata, char arr[][32]) {
    // Dynamically allocate a new node
    node* n = (node*)malloc(sizeof(node));
    node* temp = (node*)malloc(sizeof(node));
    arr[xdata][ydata] = 'X';  // Mark visited
    if (debugMode) {
        printf("Pushing: (%d, %d)\n", xdata, ydata);
    }
    
    // Store x and y coordinate in the node
    n->x = xdata;
    n->y = ydata;
    n->next = NULL;
    
    // If the list is empty, have the head point to the new node
    if (head == NULL) {
        head = n;
    }
    else {
        temp = head;
        n->next = temp;
        head = n;
    }
    return head;
}


// Pops a node off the stack
void PopNode(node** head) {
    node *temp;
    
    if (debugMode)
        printf("Popping: (%d, %d)\n", (*head)->x, (*head)->y);
    
    temp = *head;
    *head =(*head)->next;
    free(temp);
}


// Displays the solution path from start to end
void PrintList(node* head) {
    node* curr = (node*)malloc(sizeof(node));
    
    curr = head;
    printf("Solution path from end to start:\n");
    while (curr != NULL) {
        printf("(%d, %d)\n",curr->x, curr->y);
        curr = curr->next;
    }
}

// Access the top element of the stack
node* TopStack(node **head) {
    return *head;
}

// Initializes the stack with the starting coordinates
void InitStack(node** head, int xstart, int ystart, char arr[][32]) {
    *head = PushNode(*head, xstart, ystart, arr);
}

// Checks if an unblocked location in the maze has been visited
bool IsVisited(maze m1, int xpos, int ypos) {
    if (m1.arr[xpos][ypos] == 'X') {
        return true;
    }
    else
        return false;
    
}

// Checks if the stack is empty
bool IsEmpty(node** head) {
    if (*head == NULL) {
        return true;
    }
    else return false;
}

// Validates input
int ValidateInput(int *validInput, int xpos, int ypos, maze* m1) {
    int i, j;
    if (*validInput == 0) {  // Size of the maze has not yet been set
        if (xpos > 0 && ypos > 0) {
            // Set the size of the maze with the first valid line of input
            m1->xsize = xpos;
            m1->ysize = ypos;
            // Initialize the maze to empty with all places '.', including borders (+2)
            for (i = 0; i < m1->xsize+2; i++)
                for (j = 0; j < m1->ysize+2; j++)
                    m1->arr[i][j] = '.';
            return *validInput+=1;;
        }
        else {
            fprintf(stderr, "Invalid: Maze size must be greater than 0\n");
        }
    }
    // Start coordinates have not yet been set
    if (*validInput == 1)  {
        if (xpos > 0 && ypos > 0) {
            if (xpos <= m1->xsize) {
                if (ypos <= m1->ysize) {
                    // Set the starting coordinates
                    m1->xstart = xpos;
                    m1->ystart = ypos;
                    return *validInput +=1;
                }
                else {
                    fprintf(stderr, "Invalid: Column %d is out of range from 1 to %d\n", ypos, m1->ysize);
                }
            }
            else {
                fprintf(stderr, "Invalid: Row %d is out of range from 1 to %d\n", xpos, m1->xsize);
            }
        }
        else {
            fprintf(stderr, "Invalid: 0 is not a valid maze coordinate\n");
        }
    }
    // End coordinates have not yet been set
    if (*validInput == 2)  {
        if (xpos > 0 && ypos > 0) {
            if (xpos <= m1->xsize) {
                if (ypos <= m1->ysize) {
                    // Set the end coordinates
                    m1->xend = xpos;
                    m1->yend = ypos;
                    return *validInput +=1;
                }
                else {
                    fprintf(stderr, "Invalid: Column %d is out of range from 1 to %d\n", ypos, m1->ysize);
                }
            }
            else {
                fprintf(stderr, "Invalid: Row %d is out of range from 1 to %d\n", xpos, m1->xsize);
            }
        }
        else {
            fprintf(stderr, "Invalid: 0 is not a valid maze coordinate\n");
        }
    }
    
    if (*validInput == 3) {
        // Ignore values of 0 or below, continue processing
        if (xpos <= 0 || ypos <= 0) {
            fprintf(stderr, "Invalid: Coordinates must be greater than 0\n");
        }
        // Row out of range
        else if (xpos > m1->xsize ) {
            fprintf(stderr, "Invalid: Row %d is outside of range from %d to %d\n", xpos, 1, m1->xsize);
        }
        // Column out of range
        else if (ypos > m1->ysize) {
            fprintf(stderr, "Invalid: Column %d is outside of range from %d to %d\n", ypos, 1, m1->ysize);
        }
        // Blocking start pos
        if (xpos == m1->xstart &&  ypos == m1->ystart) {
            fprintf(stderr, "Invalid: Attempting to block starting position\n");
        }
        // Blocking end pos
        if (xpos == m1->xend && ypos == m1->yend) {
            fprintf(stderr, "Invalid: Attempting to block ending position\n");
        }
        else {
            m1->arr[xpos][ypos] = '*';  // Set the block
        }
    }
    return *validInput;
}

// Search the maze using DFS(iterative)
void Search(node** head, maze* m1) {
    // Continue searching while the stack is not empty and the end has not been visited
    while(*head != NULL && m1->arr[m1->xend][m1->yend] != 'X') {
        // If the coords at the top of the stack is the end pos, maze has been solved
        if ((*head)->x == m1->xend && (*head)->y == m1->yend) {
            break;
        }
        if (m1->arr[(*head)->x][(*head)->y+1] != '*' && m1->arr[(*head)->x][(*head)->y+1] != 'X') {  // Right
            *head = PushNode(*head, (*head)->x, (*head)->y+1, m1->arr);
            continue;
        }
        if (m1->arr[(*head)->x+1][(*head)->y] != '*' && m1->arr[(*head)->x+1][(*head)->y] != 'X') {  // Down
            *head = PushNode(*head, (*head)->x+1,(*head)->y, m1->arr);
            continue;
        }
        if (m1->arr[(*head)->x][(*head)->y-1] != '*' && m1->arr[(*head)->x][(*head)->y-1] != 'X') {  // Left
            *head = PushNode(*head, (*head)->x, (*head)->y-1, m1->arr);
            continue;
        }
        if (m1->arr[(*head)->x-1][(*head)->y] != '*' && m1->arr[(*head)->x-1][(*head)->y] != 'X') {  // Up
            *head = PushNode(*head, (*head)->x-1, (*head)->y, m1->arr);
            continue;
        }
        else {
            PopNode(head);
        }
    }
}


int main (int argc, char **argv) {
    maze m1;
    int xpos, ypos;
    int i,j;
    
    FILE *src;
    
    // check if debug mode is on
    for (int i = 0; i<argc; i++){
        if ((argv[i][0] == '-') && (argv[i][1] == 'd')){
            debugMode = true;
        }
    }
    
    //This program will always pick the first file if given more than 1 file;
    if ((argc > 2) && (strcmp(argv[2], "-d") != 0) && (strcmp(argv[1], "-d") != 0) ){
        if ((src = fopen( argv[1], "r" )) == NULL ){
            fprintf(stderr, "Can't open input file: %s\n", argv[1] );
            exit(-1);
        }
        fprintf(stderr, "First input file will be used for this program.\n");
        
    }
    
    // Check for two command arguments
    if(argc == 2){
        if ((src = fopen( argv[1], "r" )) == NULL ){
            fprintf(stderr, "Can't open input file: %s\n", argv[1] );
            exit(-1);
        }
    }
    
    // Check if the -d is at index 1 or at index 2
    if(argc > 2){
        if(strcmp(argv[2], "-d") == 0){
            if (( src = fopen( argv[1], "r" )) == NULL ){
                fprintf(stderr, "Can't open input file: %s\n", argv[1] );
                exit(-1);
            }
        }
        else if (strcmp(argv[1], "-d") == 0){
            if ((src = fopen( argv[2], "r" )) == NULL ){
                fprintf(stderr, "Can't open input file: %s\n", argv[2] );
                exit(-1);
            }
        }
    }
   
    // Try to open the file
    // 2nd param 'r' indicates that the file being opened is for reading only
    bool file = false;
    for (i = 1; i < argc; i++) {
        if (( src = fopen( argv[i], "r" )) != NULL)
        {
            file = true;
            break;
        }
        else {
            printf ("Can't open input file: %s\n", argv[i]);
        }
    }
    if (file == false) exit(-1);
    
    // After scanning the first 3 lines of data...mark the blocked positions in the maze with *'s
    // Error check
    int validInput = 0;
    while (fscanf (src, "%d %d", &xpos, &ypos) != EOF) {
        ValidateInput(&validInput, xpos, ypos, &m1);
    }
    
    // Exit the program if the file is unable to read 3 valid lines of input
    if (validInput < 3) {
        fprintf(stderr, "File ended before 3 valid lines of input were read.\n");
        exit(-1);
    }
    
    // Print out size, start, end
    printf ("size: %d, %d\n", m1.xsize, m1.ysize);
    printf ("start: %d, %d\n", m1.xstart, m1.ystart);
    printf ("end: %d, %d\n", m1.xend, m1.yend);
    
    // Mark the borders of the maze with *'s
    for (i=0; i < m1.xsize+2; i++) {
        m1.arr[i][0] = '*';  // Left
        m1.arr[i][m1.ysize+1] = '*';  // Right
    }
    for (i=0; i < m1.ysize+2; i++) {
        m1.arr[0][i] = '*';  // Top
        m1.arr[m1.xsize+1][i] = '*';  // Bottom
    }
    
    // Mark the starting and ending positions in the maze
    m1.arr[m1.xstart][m1.ystart] = 's';
    m1.arr[m1.xend][m1.yend] = 'e';
    
    // Print out the initial maze
    for (i = 0; i < m1.xsize+2; i++)
    {
        for (j = 0; j < m1.ysize+2; j++)
            printf ("%c", m1.arr[i][j]);
        printf("\n");
    }
    
    node* head = (node*)malloc(sizeof(node));  // Serves as top of the stack, nodes will be prepended
    head = NULL;
    
    // Push the starting position onto the stack
    InitStack(&head, m1.xstart, m1.ystart, m1.arr);
    // Preform DFS to traverse the maze for a solution
    Search(&head, &m1);
    
    // Print out the traversed maze
    for (i = 0; i < m1.xsize+2; i++) {
        for (j = 0; j < m1.ysize+2; j++)
            printf ("%c", m1.arr[i][j]);
        printf("\n");
    }
    
    // If the stack is empty, no solution to the maze
    if (IsEmpty(&head)) {
        printf("The maze has no solution.\n");
    }
    // Print the solution if there is one
    else {
        PrintList(head);
    }
    
    fclose(src);
    
    return 0;
}
