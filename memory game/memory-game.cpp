/* Only constraints: arrays are not allowed */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

//36 characters corresponding to pieces on the game board
char p0, p1, p2, p3, p4, p5,
p6, p7, p8, p9, p10, p11,
p12, p13, p14, p15, p16, p17,
p18, p19, p20, p21, p22, p23,
p24, p25, p26, p27, p28, p29,
p30, p31, p32, p33, p34, p35;

char RandomizeChar();

void GenerateRow(char& entry1, char& entry2, char& entry3, char& entry4, char& entry5, char& entry6);

int OddParity(char& entry1, char& entry2, char& entry3, char& entry4, char& entry5, char& entry6);

void DisplayBoard();

void RandomizeBoard();

void SwitchPiece(char userInput1, char userInput2);

void PlayerInput(char guessRow, char guessCol);


int main() {
    char userInput1, userInput2;
    
    //Set the seed to time to insure numbers are completely random
    srand((unsigned int) time(0));
    
    //Display header information
    cout << "Author: " << setw(18) << "Michelle"           << endl;
    cout << "Class: " << setw(21) << "CS 141, Spring 2018" << endl;
    cout << "Lab: " << setw(18) << "Tuesdays 11 am"        << endl;
    cout << "Program: " << setw(4) << "#2, Memory Game"    << endl << endl;
    
    //Welcome user, display game instructions
    cout << "Welcome to the Memory Game!" << endl << endl;
    
    cout << "Look away from the board and have a helper enter r"      << endl;
    cout << "to randomize the board until they have a random board"   << endl;
    cout << "that they like.  Then you glance at it and try to"       << endl;
    cout << "imprint it in your mind and look away.  Your helper"     << endl;
    cout << "will then select a single piece to be flipped by"        << endl;
    cout << "choosing its row and column.  The changed board is then" << endl;
    cout << "displayed. You then must try to guess which one it was." << endl;
    cout << "Enter x to exit the program." << endl << endl << endl;
    
    //Generate random board and display it
    RandomizeBoard();
    DisplayBoard();
    
    //Prompt user to enter 'r' to randomize, 'x' to exit, or a row and column to be flipped for the other player to guess
    cout << "Enter r to randomize the board, or row and column to change a value ->";
    cin >> userInput1;   //Input can be an 'x,' 'r,' or row
    
    do {
        //Exit the program if user enters 'x'
        if ((userInput1 == 'x') || (userInput1 == 'X')) {
            cout << endl << "Exiting..." << endl;    //Display exit message
            exit(0);
        }
        //Randomize another board and display if user enters 'r'
        while ((userInput1 == 'R') || (userInput1 == 'r')) {
            RandomizeBoard();
            DisplayBoard();
            cout << "Enter r to randomize the board, or row and column to change a value ->";  //Prompt user to select another option
            cin >> userInput1;
        }
    } while (userInput1 == 'X' || userInput1 == 'x' || userInput1 == 'r' || userInput1 == 'R');
    //Take second user input if the first character entered was not 'r' or 'x'
    cin >> userInput2;     //Input is a column
    
    SwitchPiece(userInput1, userInput2);   //Flip the piece corresponding to row and column entered
    return 0;
}   //end main()


//Generates 'X' for odd numbers, 'O' for even
char RandomizeChar() {
    int randomNum;
    char letter = ' ';
    
    randomNum = rand() % 2;    //If the mod of the random number is odd, it is assigned 'X'
    if (randomNum == 1) {
        letter = 'X';
    }
    if (randomNum == 0) {
        letter = 'O';         //If the mod of the random number is even, it is assigned 'O'
    }
    return letter;
}

//Generates random characters 'X', 'O' for 6 entries
void GenerateRow(char& entry1, char& entry2, char& entry3, char& entry4, char& entry5, char& entry6) {
    
    entry1 = RandomizeChar();
    entry2 = RandomizeChar();
    entry3 = RandomizeChar();     //6 positions on the board are entered and randomized to make a row of 'X' and 'O'
    entry4 = RandomizeChar();
    entry5 = RandomizeChar();
    entry6 = RandomizeChar();
}

//Counts the occurance of each X in a set of 6 entries, the entries make up a row or a column in the game board
int OddParity(char& entry1, char& entry2, char& entry3, char& entry4, char& entry5, char& entry6) {
    int countX;              //Counts each  occurence of X
    
    countX = 0;
    //Check all 6 entries in a row or column, if it contains 'X' +1 to the counter
    if (entry1 == 'X') countX += 1;
    if (entry2 == 'X') countX += 1;
    if (entry3 == 'X') countX += 1;
    if (entry4 == 'X') countX += 1;
    if (entry5 == 'X') countX += 1;
    if (entry6 == 'X') countX += 1;
    
    /* If the counter % 2 is even, the parity rule is not met so set it to 0. If it is odd set it to 1 and return
     it where it will be called in RandomizeBoard() to check parity for all 6 rows and all 6 columns together */
    countX = (countX % 2 == 0) ?  0: 1;
    return countX;
}

//Displays and formats the board; aligns numbers, rows and columns
void DisplayBoard(){
    cout << endl;     //Space between text to prompt user and the game board
    
    //Display column numbers and outline of the board for the top
    cout << setw(3) << "    1 2 3 4 5 6"   << endl;
    cout << setw(2) << "  _ _ _ _ _ _ _ _" << endl << endl;
    
    //Display rows A-F
    cout << "A | " << p0 << " " << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " | " << "A"       << endl;
    cout << "B | " << p6 << " " << p7 << " " << p8 << " " << p9 << " " << p10 << " " << p11 << " | " << "B"     << endl;
    cout << "C | " << p12 << " " << p13 << " " << p14 << " " << p15 << " " << p16 << " " << p17 << " | " << "C" << endl;
    cout << "D | " << p18 << " " << p19 << " " << p20 << " " << p21 << " " << p22 << " " << p23 << " | " << "D" << endl;
    cout << "E | " << p24 << " " << p25 << " " << p26 << " " << p27 << " " << p28 << " " << p29 << " | " << "E" << endl;
    cout << "F | " << p30 << " " << p31 << " " << p32 << " " << p33 << " " << p34 << " " << p35 << " | " << "F" << endl;
    
    cout << setw(2) << "  _ _ _ _ _ _ _ _" << endl << endl;   //Outline the bottom of the board
    return;
}

//Generate random columns and rows, check them for odd parity
void RandomizeBoard() {
    int rowA, rowB, rowC, rowD, rowE, rowF;     //Rows going across the board
    int col1, col2, col3, col4, col5, col6;     //Columns going down the board
    int sumRows, sumCols = 0;
    
    do {
        //Generate random assortment of 'X' and 'O' for all 6 rows
        GenerateRow(p0, p1, p2, p3, p4, p5);
        GenerateRow(p6, p7, p8, p9, p10, p11);
        GenerateRow(p12, p13, p14, p15, p16, p17);
        GenerateRow(p18, p19, p20, p21, p22, p23);
        GenerateRow(p24, p25, p26, p27, p28, p29);
        GenerateRow(p30, p31, p32, p33, p34, p35);
        
        //Check if the rows have an odd number of 'X'
        //Rows that have an odd number of 'X' will return a 1
        rowA = OddParity(p0, p1, p2, p3, p4, p5);
        rowB = OddParity(p6, p7, p8, p9, p10, p11);
        rowC = OddParity(p12, p13, p14, p15, p16, p17);
        rowD = OddParity(p18, p19, p20, p21, p22, p23);
        rowE = OddParity(p24, p25, p26, p27, p28, p29);
        rowF = OddParity(p30, p31, p32, p33, p34, p35);
        
        sumRows = rowA + rowB + rowC + rowD + rowE + rowF;   //Add up the values of all 6 rows
        if (sumRows!= 6){    //If each row does not return a 1, keep looping until all 6 do
            continue;
        }
        else {    //Once all 6 rows have and odd number 'X,' check if the columns are odd
            col1 = OddParity(p0, p6, p12, p18, p24, p30);
            col2 = OddParity(p1, p7, p13, p19, p25, p31);
            col3 = OddParity(p2, p8, p14, p20, p26, p32);
            col4 = OddParity(p3, p9, p15, p21, p27, p33);
            col5 = OddParity(p4, p10, p16, p22, p28, p34);
            col6 = OddParity(p5, p11, p17, p23, p29, p35);
            
            sumCols = col1 + col2 + col3 + col4 + col5 + col6;   //Add up values of all 6 columns
        }
    }while ( sumCols != 6);   //Run until both rows and columns satisfy odd parity
}

//Takes input of a row & column, flips a piece on the board
void SwitchPiece(char userInput1, char userInput2) {
    //If the piece  s an X, it will be an O and vice versa
    switch (userInput1) {
        case 'A':  //Check upper and lowercase A
        case 'a':
            if (userInput2 == '1') p0 = (p0 == 'X') ? 'O' : 'X';
            if (userInput2 == '2') p1 = (p1 == 'X') ? 'O' : 'X';
            if (userInput2 == '3') p2 = (p2 == 'X') ? 'O' : 'X';
            if (userInput2 == '4') p3 = (p3 == 'X') ? 'O' : 'X';
            if (userInput2 == '5') p4 = (p4 == 'X') ? 'O' : 'X';
            if (userInput2 == '6') p5 = (p5 == 'X') ? 'O' : 'X';
            break;
            
        case 'B':  //Check upper and lowercase B
        case 'b':
            if (userInput2 == '1') p6 = (p6 == 'X') ? 'O' : 'X';
            if (userInput2 == '2') p7 = (p7 == 'X') ? 'O' : 'X';
            if (userInput2 == '3') p8 = (p8 == 'X') ? 'O' : 'X';

            if (userInput2 == '4') {
                p9 = (p9 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '5') {
                p10 = (p10 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '6') {
                p11 = (p11 == 'X') ? 'O' : 'X';
            }
            break;
            
        case 'C':  //Check upper and lowercase C
        case 'c':
            if (userInput2 == '1') {
                p12 = (p12 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '2') {
                p13 = (p13 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '3') {
                p14 = (p14 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '4') {
                p15 = (p15 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '5') {
                p16 = (p16 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '6') {
                p17 = (p17 == 'X') ? 'O' : 'X';
            }
            break;
            
        case 'D':  //Check upper and lowercase D
        case 'd':
            if (userInput2 == '1') {
                p18 = (p18 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '2') {
                p19 = (p19 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '3') {
                p20 = (p20 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '4') {
                p21 = (p21 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '5') {
                p22 = (p22 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '6') {
                p23 = (p23 == 'X') ? 'O' : 'X';
            }
            break;
            
        case 'E':  //Check upper and lowercase E
        case 'e':
            if (userInput2 == '1') {
                p24 = (p24 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '2') {
                p25 = (p25 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '3') {
                p26 = (p26 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '4') {
                p27 = (p27 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '5') {
                p28 = (p28 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '6') {
                p29 = (p29 == 'X') ? 'O' : 'X';
            }
            break;
            
        case 'F':  //Check upper and lowercase F
        case 'f':
            if (userInput2 == '1') {
                p30 = (p30 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '2') {
                p31 = (p31 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '3') {
                p32 = (p32 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '4') {
                p33 = (p33 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '5') {
                p34 = (p34 == 'X') ? 'O' : 'X';
            }
            if (userInput2 == '6') {
                p35 = (p35 == 'X') ? 'O' : 'X';
            }
            break;
    }
    PlayerInput(userInput1, userInput2);   //Prompt the player to guess which piece was moved upon flipping
}

//Takes user;s guess for piece flipped, displays end-game messages
void PlayerInput(char userInput1, char userInput2) {
    char guessRow = ' ';
    char guessCol = ' ';
    
    //Separates flipped piece input from the generated board using 25 blank lines
    for (int i = 0; i < 25; i++) {
        cout << endl;
    }
    
    DisplayBoard();
    
    //Allow user to type in their guess of which piece was flipped
    cout << "What piece do you think it was? ->";
    cin >> guessRow;
    
    //Exit the program if user enters 'x'
    if ((guessRow == 'x') || (guessCol == 'X')) {
        cout << endl << "Exiting..." << endl;     //Display exit message
        exit(0);
    }
    else {   //If the user did not exit, take the input of the column
        cin >> guessCol;
        cout << endl;
    }
    //If the user guesses the correct piece that was flipped, congratulate them
    if ((guessRow == userInput1) && (guessCol == userInput2)) {
        cout << "*** Congratulations!  You did it! *** " << endl << endl;
        cout << "Thank you for playing.  Exiting..." << endl;
    }
    else {  //If they guess incorrectly, tell them what the correct answer is
        cout << "Sorry, it was " << userInput1 << userInput2 << ". Better luck next time." << endl;
    }
    return;
}
