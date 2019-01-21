#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool debugMode = false;
typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;

typedef struct node {
    char* name;
    int size;
    bool inStatus;
    struct node* next;
} node;


void clearList(node *head) {
    node *tmp = NULL;
    while(head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

/** traverses the entire waiting list: prints out group name, group size and in-restaurant status
    <d> command
 */
void DisplayListInformation(node** head) {
    node* tmp = *head;
    // empty list
    if ((*head) == NULL) {
        printf("The waiting list is empty.\n");
        return;
    }
    // non-empty list
    int i = 1;
    while(tmp != NULL) {
        printf("Group %d - Name: %s, Size: %d, Inside-status: ", i, tmp->name, tmp->size);
        if (tmp->inStatus == 1)
            printf("YES\n");
        else
            printf("NO\n");
        tmp = tmp->next;
        i++;
    }
}

/** adds a new group to the end of the queue
    <a>, <c> commands
 */
void AddToList(node** head, int size, char *name, boolean status) {
    node *tmp = (*head);
    node *curr = (node*)malloc(sizeof(node));
    
    // set the fields for the new member
    curr->next = NULL;
    curr->name = name;
    curr->size = size;
    curr->inStatus = status;
    
    // case for adding to an empty list
    if ((*head) == NULL) {
        (*head) = curr;
    }
    else {  // find the last node in the list
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = curr;  // append the new party to the list
    }
}

/** checks if the name already exists inside of the list
    <a>, <c>, <w>, <l> commands
 */
bool DoesNameExist(node** head, char name[]) {
    node* n = *head;
    
    while (n != NULL) {
        if (strcmp(name, n->name) == 0) { // if 0 is returned, names are identical
            return true;
        }
        n = n->next;
    }
    return false;
}

/** changes the in-restaurant status to IN when a call group arrives to the restaurant
    <w> command
 */
void UpdateStatus(node** head, char name[]) {
    node* tmp = *head;
    int i = 1;  // keeps track of group number

    while (tmp != NULL) {
        if (debugMode) {
            printf("Group %d - Name: %s, Size: %d, Inside-status: ", i, tmp->name, tmp->size);
            if (tmp->inStatus == 0) printf("NO\n");
            else if (tmp->inStatus == 1) printf("YES\n");
        }
        // if the name is found, change the in-restaurant status to IN
        if ((strcmp(name, tmp->name) == 0) && tmp->inStatus == 0) {
            printf("Updating Inside-Status for group \"%s\" to YES\n", tmp->name);
            tmp->inStatus = YES;
            return;
        }
        // the name is already marked as inside the restaurant
        else if(strcmp(name, tmp->name) == 0 && tmp->inStatus== 1) {
            printf("Group \"%s\" is already waiting in the restaurant.\n", tmp->name);
            return;
        }
        i++;
        tmp = tmp->next;
    }
}

/** counts groups ahead of a specified group and shows group sizes
    <l> command
 */
void CountGroupsAhead (node **head, char name[]) {
    int num_groups = 0;
    node* tmp = *head;
    
    while(tmp != NULL) {
        if (strcmp(name, tmp->name) == 0)
            break;
        // only count a group ahead of a specific group if they are inside the restaurant
        if (tmp->inStatus)
            num_groups++;
        tmp = tmp->next;
    }
    printf("The number of groups infront of group \"%s\" is: %d\n", name, num_groups);
}

/** removes the first group in the waiting list that can fit at a given table
    <r>
 */
char* RetrieveAndRemove(node** head, int size) {
    int i = 1;
    node *prev, *tmp;
    char *name;
    prev = *head;
    tmp = *head;
    
    while (tmp != NULL) {
        if (tmp->size <= size) {
            // display debugging info
            if (debugMode) {
                printf("Group %d - Name: %s, Size: %d, Inside-status: ", i, tmp->name, tmp->size);
                if (tmp->inStatus == 0) printf("NO\n");
                else if (tmp->inStatus == 1) printf("YES\n");
            }
            // group has been matched, remove
            if (tmp->inStatus == 1) {
                name = tmp->name;
                // singleton case
                if ((*head) == tmp && tmp->next == NULL) {
                    free(tmp);
                    return name;
                }
                // removing the first group in the list (non-singleton)
                else if ((*head) == tmp && tmp->next != NULL) {
                    (*head) = tmp->next;
                }
                // removing last group in the list
                else if (tmp->next == NULL && tmp != (*head)) {
                    prev->next = NULL;
                }
                // removing a non-first or non-last group
                else {
                    prev->next = tmp->next;
                }
                free(tmp);
                tmp = NULL;
                return name;
            }
        }
        prev = tmp;
        tmp = tmp->next;
        i++;
    }
    name = "none";
    return name;
}

/** traverses down the list until a specific group name is encountered. as each node is traversed, print out its group size.
    <l> command
*/
void DisplayGroupSizeAhead(node**head, char name[], bool willDisplay) {
    int num_ahead = 0;
    node *tmp;
    tmp = *head;
    
    while(tmp != NULL) {
        num_ahead++;
        if (strcmp(name, tmp->name) == 0) return; // exit when the name is found
        
        // print the information for each group ahead if they are waiting inside the restaurant
        else if (tmp->inStatus == 1 && willDisplay) {
            printf("Group %d - Name: %s, Size: %d, Inside-status: ", num_ahead, tmp->name, tmp->size);
            if (tmp->inStatus == 0)
                printf("NO\n");
            else
                printf("YES\n");
        }
        tmp = tmp->next;
    }
}

/* forward definition of functions */
void clearToEoln();

/* Read in until the first Non-White-Space character is Read */
/* The white space characters are:
 *      space, tab \t, newline \n, vertical tab \v,
 *      form feed \f, and carriage return \r
 */
int getNextNWSChar () {
    int ch;
    ch = getc(stdin);
    // Read in a char. If it is EOF or a whitespace character, return it to run the next loop or end the istream
    if (ch == EOF || ch == '\n')
    return ch;
    // While char is WS, continue to get the next char in the stream
    while (isspace (ch)) {
        ch = getc(stdin);
        if (ch == EOF || ch == '\n')  // return if EOF or newline
        return ch;
    }
    return ch;  // return non WS char
}

// read in the next Positive Integer or error
int getPosInt() {
    int value = 0;
    
    // clear white space characters
    int ch;
    ch = getc(stdin);
    
    // Looking for digit.
    // If a newline is given before a digit, or a space is not given, there is an error
    while (!isdigit(ch)) {
        if ('\n' == ch)  // error \n ==> no integer given
        return 0;
        if (!isspace(ch)) { // error non white space ==> integer not given next *
            clearToEoln();
            return 0;
        }
        ch = getc(stdin);  // Get next char until a digit is found
    }
    
    value = ch - '0';  // Convert char to int with ascii
    
    ch = getc(stdin);  // Check if the int is greater than 1 digit
    
    while (isdigit(ch)) {
        value = value * 10 + ch - '0';  // Convert value to the appropriate integer amt using ascii
        
        ch = getc(stdin);  // Check if there is still another digit
    }
    ungetc (ch, stdin);  // put the last read character back in input stream
    
    /* Integer value of 0 is an error in this program */
    if (0 == value)
    clearToEoln();
    
    return value;
}

// Read in the name until the end of the input
char *getName() {
    // Skip over the white space characters */
    int ch;
    ch = getc(stdin);
    
    while (isspace(ch)) {
        if ('\n' == ch)  /* error \n ==> no name given */
        return NULL;
        ch = getc(stdin);
    }
    
    char *word;
    int size;
    size = 10;
    word = (char*) malloc (sizeof(char) * size);
    
    // read in character-by-character until the newline is encountered
    int count = 0;
    
    while (ch != '\n') {
        // Grow the dynamic array if the char count exceeds default size
        if (count+1 >= size) {
            // to grow an array to make it "dynamically sized" using malloc
            char* temp;
            int i;
            size = size * 2; // double the size
            temp = (char *) malloc (sizeof(char) * size);
            
            // copy the characters to the new array
            for (i = 0 ; i < count ; i++)
            temp[i] = word[i];
            
            free (word);  // free the original array
            word = temp;  // new dynamic array
        }
        
        word[count] = ch;
        count++;
        word[count] = '\0';  // NUL character
        
        // read next character until getting to the newline
        ch = getc(stdin);
    }
    
    if (count > 30) {
        count = 30;
        word[count] = '\0';
    }
    
    // clear ending white space characters */
    while (isspace (word[count-1])) {
        count--;
        word[count] = '\0';
    }
    return word;
}

// Clear input until next End of Line Character - \n
void clearToEoln() {
    int ch;
    
    do {
        ch = getc(stdin);
    }
    while ((ch != '\n') && (ch != EOF));
}

/* Print out a list of the commands for this program */
void printCommands() {
    printf ("The commands for this program are:\n\n");
    printf ("q - to quit the program\n");
    printf ("? - to list the accepted commands\n");
    printf ("a <size> <name> - to add a group to the wait list\n");
    printf ("c <size> <name> - to add a call-ahead group to the wait list\n");
    printf ("w <name> - to specify a call-ahead group is now waiting in the restaurant\n");
    printf ("r <table-size> - to retrieve the first waiting group that can fit at the available table size\n");
    printf ("l <name> - list how many groups are ahead of the named group\n");
    printf ("d - display the wait list information\n");
    
    // clear input to End of Line
    clearToEoln();
}

void doAdd (node** head) {
    /* get group size from input */
    int size = getPosInt();
    if (size < 1) {
        printf ("Error: Add command requires an integer value of at least 1\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    /* get group name from input */
    char *name = getName();
    if (NULL == name) {
        printf ("Error: Add command requires a name to be given\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    // Check whether the name already exists inside the list
    if (!DoesNameExist(head, name)) {
        AddToList(head, size, name, YES);   // append to the list
        printf ("Adding In-restaurant group \"%s\" of size %d\n", name, size);
        if (debugMode) {
            DisplayListInformation(head);
        }
    }
    else {
        printf("Unable to add group, the name already exists.\n");
    }
}

void doCallAhead (node** head) {
    // get group size from input
    int size = getPosInt();
    if (size < 1) {
        printf ("Error: Call-ahead command requires an integer value of at least 1\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    // get group name from input
    char *name = getName();
    if (NULL == name) {
        printf ("Error: Call-ahead command requires a name to be given\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    // Check whether the name already exists inside the list
    if (!DoesNameExist(head, name)) {
        AddToList(head, size, name, NO);
        printf ("Adding Call-ahead group \"%s\" of size %d...\n", name, size);
        if (debugMode)
        DisplayListInformation(head);
    }
    else {
        printf("Unable to add the group, the name already exists.\n");
    }
}

// Marks the call ahead group as waiting in the restaurant
void doWaiting (node** head) {
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Waiting command requires a name to be given\n");
        printf ("Waiting command is of form: w <name>\n");
        printf ("  where: <name> is the name of the group that is now waiting\n");
        return;
    }
    
    // If the name is found on the waiting list, update the status
    if (DoesNameExist(head, name)) {
        UpdateStatus(head, name);
    }
    else {  // Unable to be found
        printf("Unable to modify the group. The name does not exist.\n");
    }
}


// Input table size and removes first waiting group that can fit
void doRetrieve (node** head) {
    /* get table size from input */
    int size = getPosInt();
    if (size < 1) {
        printf ("Error: Retrieve command requires an integer value of at least 1\n");
        printf ("Retrieve command is of form: r <size>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        return;
    }
    clearToEoln();
    
    char* name;
    name = RetrieveAndRemove(head, size);
    if (strcmp(name, "none") == 0) {
        printf("Unable to seat a group due to size not matching any available table or group not being in restaurant.\n");
    }
    else {
        printf ("Retrieved (and removed) group \"%s\" that can fit at a table of size %d\n", name, size);
    }
}


// Lists num of groups in the list before the specified group, and the group size of each
void doList(node** head, bool willDisplay) {
    // get group name from input
    char *name = getName();
    if (NULL == name) {
        printf ("Error: List command requires a name to be given\n");
        printf ("List command is of form: l <name>\n");
        printf ("  where: <name> is the name of the group to inquire about\n");
        return;
    }
    
    // If the name exists in the list, print out appropriate group count, size for the group
    if (DoesNameExist(head, name)) {
        CountGroupsAhead(head, name);
        DisplayGroupSizeAhead(head, name, true);
    }
    else {
        printf("Unable to locate the group. The name does not exist.\n");
    }
}

void doDisplay (node** head) {
    clearToEoln();
    DisplayListInformation(head);
}


int main (int argc, char **argv) {
    int j, i = 0;
    bool listDisplay = false;
    
    // Check if debugging mode is on
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
        debugMode = true;
        else if (strcmp(argv[i], "-D")== 0)
        debugMode = true;
    }
    
    char *input;
    int ch = '\0';
    node* head = NULL;
    
    printf ("Starting Restaurant Wait List Program\n");
    printf ("Input '?' to list commands.\n\n");
    printf ("Enter command: ");
    
    // Processes standard input char by char
    while ((ch = getNextNWSChar ()) != EOF) {
        // check for the various commands
        if ('q' == ch) {
            clearList(head);
            printf ("Quitting Program\n");
            return (0);
        }
        else if ('?' == ch) {
            printCommands();
        }
        else if('a' == ch) {
            doAdd(&head);
        }
        else if('c' == ch) {
            doCallAhead(&head);
        }
        else if('w' == ch) {
            doWaiting(&head);
        }
        else if('r' == ch) {
            doRetrieve(&head);
        }
        else if('l' == ch) {
            listDisplay = true;
            doList(&head, listDisplay);
        }
        else if('d' == ch) {
            doDisplay(&head);
        }
        else if('\n' == ch) {
            /* nothing entered on input line     *
             * do nothing, but don't give error  */
        }
        else {
            printf ("%c - is not a valid command\n", ch);
            printf ("For a list of valid commands, type ?\n");
            clearToEoln();
        }
        printf ("\nEnter command: ");
    }
    
    printf ("Quiting Program - EOF reached\n");
    return 1;
}
