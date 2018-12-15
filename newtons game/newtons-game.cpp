#include <iostream>
#include <cctype>
using namespace std;

const int ROWS = 8;
const int COLUMNS = 5;

// display header, game instructions
void DisplayInstructions() {
    cout <<  "CS 141                \n"
         <<  "Program #3            \n"
         <<  "Mac Xcode             \n"
         <<  endl;

    cout <<  "Welcome to the game of Newton, where you try to be the first      \n"
         <<  "to get 5 in a row either vertically, horizontally or diagonally.  \n"
         <<  "Two players alternate making moves.  On each turn you may enter   \n"
         <<  "the column number where your piece will be placed, where that     \n"
         <<  "piece is inserted from the top and slides down as far as it can   \n"
         <<  "go in that column.  You may also enter 'r' to rotate a piece out  \n"
         <<  "of the bottom of a column to be dropped back in at the top of     \n"
         <<  "that column.  Enter 'x' to exit.                                  \n"
         <<  endl << endl;     //Blank lines before displaying board
}

// formats & displays the game board
void DisplayBoard(char gameBoard[][COLUMNS]) {
    int i, j = 0;

    cout << endl; 
    
    // display column numbers
    cout << " 1    2    3    4    5 \n";
    
    cout << "___  ___  ___  ___  ___ \n";  // denote columns, top
    
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLUMNS; j++) {
            cout << " " << gameBoard[i][j] << "   ";  // space between elements for output
        }
        cout << endl;
    }
    cout << "___  ___  ___  ___  ___ \n  \n";  // denote columns, bottom
}

// validates user input
bool ValidInput(char userChar) {
	// user types in char 1-5
    if (userChar >= 49 && userChar <= 53) 
    	return true;  
    else if (userChar == 'x' || userChar == 'r') 
    	return true;
    return false;
}

// checks if a column is full
bool IsFull( char gameBoard[][COLUMNS], int col) {
    int i, numPieces = 0;

    // count the number of game pieces in the column
    for ( i = ROWS - 1; i > -1; i--) {
        if ( gameBoard[i][col] != '.') numPieces +=1;
    }

    if (numPieces == ROWS) // if there is a piece in each row, the column is full
    	return true;  
    else 
    	return false;
}

// rotates the board: moves the bottom most piece of the column, shifts the piece down and moves it to the top of the column
void RotateBoard( char gameBoard[][COLUMNS], int col) {
    char temp = ' ';
    int i = 0;
    
    temp = gameBoard[7][col];  // save the game piece of the last row in the column
    
    // if there is only one piece in column, it should remain there
    if (( gameBoard[7][col]) != '.' && (gameBoard[6][col] == '.')) {
        gameBoard[7][col] = temp;
        DisplayBoard(gameBoard);
        return;
    }
    // a column without any game pieces remains the same
    else if (gameBoard[7][col] == '.') {
        gameBoard[7][col] = '.';
        return;
    }
    
    for ( i = ROWS-1; i > 0; i--) {
        gameBoard[i][col] = gameBoard[i-1][col];  // move each piece in the column down one
    }
    
    // place the original last piece in the column on top
    for (i = 0; i < ROWS; ++i) {
        if (( gameBoard[i+1][col] == 'X') || ( gameBoard[i+1][col] == 'O')) {
            gameBoard[i][col] = temp;
            break;
        }
    }
    DisplayBoard(gameBoard);
}



// places a game piece in the column entered by the user
void PlayGame(char gameBoard[][COLUMNS], int playerNum, char gamePiece) {
    int i, userNum = 0;

    userNum = int( playerNum - 49); // converts char into its corresponding number
    
    // takes the column entered and looks for the furthrest empty spot
    for (i = ROWS; i > 0; i--) {
        if (( gameBoard[i-1][userNum] == 'X') || ( gameBoard[i-1][userNum] == 'O')) {  /* If the entry is already a character X or O, */
            continue;                                                                  /* continue searching upwards in the column    */
        }
        else {
            gameBoard[i-1][userNum] = gamePiece;  // assign an available board position with a game piece
            DisplayBoard( gameBoard);
            break;
        }
    }
}

/* checks for a winner by taking the sum of the 5 pieces in possible winning combinations
 * if the total of the sum is 440, X wins.  If the total of the sum is 395, O wins.
 * returns the winning character to be displayed                                        */
char IsWinner(char gameBoard[][COLUMNS]) {
    int i, j = 0;

    // checks for a winner across rows
    for (i = 0; i < ROWS; i++) {
        if (gameBoard[i][0] + gameBoard[i][1] + gameBoard[i][2] + gameBoard[i][3] + gameBoard[i][4] == 440) return 'X';  //Counts X by ASCII: 88
        if (gameBoard[i][0] + gameBoard[i][1] + gameBoard[i][2] + gameBoard[i][3] + gameBoard[i][4] == 395) return 'O';  //Counts O by ASCII: 65
    }
    
    // checks for a winner down columns
    for (i = 0; i < COLUMNS; i++) {
        for (j = 0; j < 4; j++) {
            if (gameBoard[i][j] + gameBoard[i+1][j] + gameBoard[i+2][j] + gameBoard[i+3][j] + gameBoard[i+4][j] == 440) return 'X';  //Counts X
            if (gameBoard[i][j] + gameBoard[i+1][j] + gameBoard[i+2][j] + gameBoard[i+3][j] + gameBoard[i+4][j] == 395) return 'O';  //Counts O
        }
    }
    
    // checks for a winner diagonally -- begins top left
    for (i = 0; i < 4; i++) {
        if (gameBoard[i][0] + gameBoard[i+1][1] + gameBoard[i+2][2] + gameBoard[i+3][3] + gameBoard[i+4][4] == 440) return 'X';
        if (gameBoard[i][0] + gameBoard[i+1][1] + gameBoard[i+2][2] + gameBoard[i+3][3] + gameBoard[i+4][4] == 395) return 'O';
    }
    
    // checks for a winner diagonally -- begins top right
    for (i = 0; i < 4; i++) {
        if (gameBoard[i][4] + gameBoard[i+1][3] + gameBoard[i+2][2] + gameBoard[i+3][1] + gameBoard[i+4][0] == 440) return 'X';
        if (gameBoard[i][4] + gameBoard[i+1][3] + gameBoard[i+2][2] + gameBoard[i+3][1] + gameBoard[i+4][0] == 395) return 'O';
    }
    
    return ' ';  // returns no winning character if one is not found
}


int main() {
    char gameBoard[ROWS][COLUMNS]; // 2D array of the game board
    char playerChar = '\0', playerInput1, playerInput2 = ' ';
    int turnNum = 1;  
    int i, j = 0;
    
    // initialize a 2D array of the game board 8 x 5
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLUMNS; j++) {
            gameBoard[i][j] = '.';  // assign each element with '.'
        }
    }
    
    DisplayInstructions();
    DisplayBoard(gameBoard);
    
    // continue to run the game until a winning combination of 5 is found
    while (IsWinner(gameBoard) != 'X' && IsWinner(gameBoard) != 'O' ) {
        if (turnNum % 2 == 1) playerChar = 'X';  //X's turn on odd numbers
        if (turnNum % 2 == 0) playerChar = 'O';  //O's turn on even numbers
        
        cout << turnNum << ". Enter column number to place " << playerChar << " or 'r' to rotate: ";
        cin >> playerInput1;  // get player input
        playerInput1 = tolower(playerInput1);

        if ( playerInput1 == 'x') exit(0);  // exit if X is entered
        
        // prompt for additional input if it is invalid
        if ( !ValidInput(playerInput1)) {
            cout << endl << "*** Invalid input.  Please retry..." << endl;
            continue;
        }
        
        // if the column is full and the player wants to add another piece, prompt them to retry
        if ( IsFull(gameBoard, int(playerInput1 - 49)) && playerInput1 != 'r') {
            cout << endl << "*** Sorry, that column is already full.  Please choose another." << endl;
            continue;
        }
        
        // rotate the column if R is entered
        if ( playerInput1 == 'r')  {
            cin >> playerInput2;  //Take in a column number
            RotateBoard(gameBoard, int(playerInput2 - 49)); // ASCII val of column - 49, converts a char number to an int
            if ( IsWinner(gameBoard) != ' ') break;
        }

        PlayGame( gameBoard, playerInput1, playerChar);
        turnNum += 1;  // increment to the next player's turn
    }
    
    // display congratulatory message for the winning player
    cout << "*** Congratulations, player " << IsWinner(gameBoard) << "!  You are the winner! *** ";
    cout << endl;
    
    return 0;
}
