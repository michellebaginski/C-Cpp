#include <iostream>
#include <cstring>      // for getline
#include <cstdlib>      // for the system command
#include <cctype>       // for the letter checking functions, e.g. toupper( )
#include <fstream>      // for file input and output
#include <ctime>
#include <cstdlib>
using namespace std;

// Global data structures for the board
const int BOARD_EDGE = 16;
const int BOARD_SIZE = BOARD_EDGE * BOARD_EDGE;
const int NUMBER_OF_ROBOTS = 4;
// declare constants to later use instead of numbers to select values from theBoard
const int NUMBER_OF_ELEMENTS = 5;   // Number of elements for each piece, enumerated below
const int PIECE = 0;     // to reference the character, such as in: theBoard[ i][ PIECE]
const int LEFT  = 1;     // to reference the left wall character, such as in: theBoard[ i][ LEFT]
const int ABOVE = 2;     // to reference the above wall character, such as in: theBoard[ i][ ABOVE]
const int RIGHT = 3;     // to reference the right wall character, such as in: theBoard[ i][ RIGHT]
const int BELOW = 4;     // to reference the below wall character, such as in: theBoard[ i][ BELOW]

class Board {
private:
    char theBoard[BOARD_SIZE][NUMBER_OF_ELEMENTS];
    int theRobots[NUMBER_OF_ROBOTS+1] = {0, 0, 0, 0, 0};  // Ignore the 0th robot
    int winningLetterIndex;
    int numDestinationPieces;
public:
    Board();  // Default constructor
    void readFileAndModifyBoard();
    void displayHeaderAndInfo();
    void displayBoard();
    void assignRobotAndDestination();
    bool makeMove(char robot, char direction, bool &winner);
    bool validateMove(char robot, int robotPosition);
    bool takeAndCheckInput(char robot, char direction);
    char winningRobot, winningDestination;
    
};

// Default constructor, populates the board with '.' for each piece
// Sets the elements for each piece on the board
Board::Board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        theBoard[i][PIECE] = '.';
        theBoard[i][LEFT]  = ' ';  // Initialize all wall pieces as blanks
        theBoard[i][ABOVE] = ' ';
        theBoard[i][RIGHT] = ' ';
        theBoard[i][BELOW] = ' ';
        
        // Add wall information for each of the 4 elements for each piece
        // Left
        if (i % BOARD_EDGE == 0) {  // If i % 16 == 0, the element is the first in it's row and has a wall to the left
            theBoard[i][LEFT] = '|';
        }
        // Above
        if (i < BOARD_EDGE) {  // Above wall exists only for the first 16 elements of the board
            theBoard[i][ABOVE] = '-';
        }
        // Right
        if ((i+1) % 16 == 0) {         // If the 15+1th element modulus returns 0, it is the last element of the row
            theBoard[i][RIGHT] = '|';  // Wall on the right side
        }
        if (i >= 240 && i < BOARD_SIZE) {     // The last row of pieces will have a wall below them
            theBoard[i][BELOW] = '-';
        }
    }
}


// Displays header and author information
void Board::displayHeaderAndInfo() {
    // Author information
    cout << "Author: Michelle Baginski       " << endl
    << "Program 5: RicoRobots in C++         " << endl
    << "TA: Ashour Dankha, T 11:00 AM        " << endl
    << "March 22, 2018                       " << endl;
    
    // Display instructions
    cout << "Welcome to Ricochet Robots                                " << endl
    << "(\'Ricochet Robot\' is a registered trademark of Hans im Glück " << endl
    << "Munich, Germany, 1999. The game was created by Alex Randolph.) " << endl
    << endl
    << "The object of the game is to get the designated numbered robot " << endl
    << "to the indicated letter position.                              " << endl
    << "The numbered squares are all robots.  They unfortunately have  " << endl
    << "no brakes, and so will continue in whatever direction they are " << endl
    << "moving until they encounter an obstacle.                       " << endl
    << "For each move enter the robot number and the desired direction." << endl
    << "For instance entering                                          " << endl
    << "   1 U                                                         " << endl
    << "would move the #1 robot up as far as it can go.                " << endl
    << "The first letter of input is the robot number (1 - 4),         " << endl
    << "and the second letter is the direction:                        " << endl
    << "  (L=left, U=up, R=right, D=down)                              " << endl
    << "Enter x to exit.  Have fun!                                    " << endl
    <<  endl;
}


// Reads in the txt file and updates game piece directional elements with wall information
void Board::readFileAndModifyBoard() {
    ifstream inStream;
    string inputLine;
    int numSpecialSquares, pieceNumber;
    char c;
    
    inStream.open("board.txt");  // Open the datafile
    // Verify that the file was successfully opened
    if (inStream.fail()) {
        cout << "Unable to open input file.  Exiting..." << endl;
        exit(-1);
    }
    
    // Read from the data file one line at a time
    // The first 3 lines are documentation only.
    getline(inStream, inputLine);
    getline(inStream, inputLine);
    getline(inStream, inputLine);
    
    inStream >> numDestinationPieces;  // Read in how many destination pieces there are (line 4)
    inStream >> numSpecialSquares;     // Read in how many squares there are with walls around them (line 5)
    inStream.ignore();                 // Get ready to read in line by line wall information below
    
    // Update special squares on the board using the data file
    for (int i = 0; i < numSpecialSquares; i++) {
        getline(inStream, inputLine);
        char *pinputLine = &inputLine[0];  // Points to the first element of the array (piece number)
        // Extract the piece number from the input line array
        sscanf(pinputLine, "%d", &pieceNumber);  // Gets input between whitespace.  Starting at pointer to the 0th element, saves it to pieceNumber
        pinputLine = strchr(pinputLine, ' ');    // Advance the inputLine pointer to the space after the piece number
        
        // Read in information about the walls for the piece number
        for (int position = 1; position <= 4; position++) {
            pinputLine++;  // Advance to the next character to be read
            sscanf(pinputLine, " %c", &c);  // Read the next potential wall char, save it into a var
            // If the char is a 0, this means it is blank and has no wall, convert it to a space
            if (c == '0') {
                c = ' ';
            }
            theBoard[pieceNumber][position] = c;  // Update elements for the board piece with their wall characters
        }
        pinputLine++;  // Advance the pointer to the increment line to the next place after the 4 characters
        // Check if there is a piece letter to still be read on the input line ex: 18 00|- A
        // Find the end of the line and see the length of the string
        if (strlen(pinputLine) > 0) {  // If there remains a char after the 4 walls have been read, save the piece letter into the piece number
            sscanf(pinputLine, " %c", &theBoard[pieceNumber][PIECE]);
        }
    }
    
    // At the end of the data file, read in the information on robot location and update the board
    char junk[80];  // Used to read and discard file information
    for (int i = 1; i <= NUMBER_OF_ROBOTS; i++) {  // Position begins at 1, corresponding to the first robot...to the 4th
        inStream >> theRobots[i];      // Save the piece number of the robot to the corresponding robots array index
        /*            cout << theRobots[i] << " --------" << endl; */
        inStream.getline(junk, '\n');  // Discard the rest of the line which is color information (if using graphics)
        theBoard[theRobots[i]][PIECE] = (char)('0'+i);  // Record the robot number in the place on the board
    }
}

// Displays the board using wall information stored previously from the file for each piece
void Board::displayBoard() {
    // display the top edge
    cout << "  ---------------------------------------------------------------- " << endl;
    // Display the body of the board
    for(int i = 0; i < BOARD_SIZE; i++) {
        // Figure out what character should be displayed to the left
        // It will be a wall if the current spot has a left wall, or if the spot to the left has a right wall
        char leftCharacter = theBoard[i][LEFT];   // Set left display char
        // See if the piece to the left has a right wall
        // Don't do this for the first piece on the board, since it has no left neighbor
        if ( (i>0) && (theBoard[ i-1][RIGHT] != ' ')) {
            leftCharacter = theBoard[i-1][ RIGHT];
        }
        cout << " " << leftCharacter << " " << theBoard[i][PIECE];
        
        // See if we're at the end of a row
        if( ((i+1) % BOARD_EDGE) == 0) {
            // Display right wall and go to next line
            cout << "  " << theBoard[i][RIGHT] << endl;
            // Display any walls immediately below the line of pieces just displayed
            // Backup our index counter j to the beginning of this line again, to find any
            // walls displayed below this line.  Don't do it for the bottom row.
            if( i < BOARD_SIZE - BOARD_EDGE) {
                cout << " |";       // Display the left boundary
                for (int j = i-BOARD_EDGE+1; j < (i+1); j++) {
                    // Set the character to be displayed.
                    // This is a wall if the current spot has a wall below, or if the spot below has a wall above
                    char belowCharacter = theBoard[j][BELOW];
                    // Only check the square below if we're NOT on the bottom row
                    if ((j < (BOARD_SIZE - BOARD_EDGE)) &&      // Verify not on bottom row
                        (theBoard[ j+16][ ABOVE] != ' ')) {    // The piece below has wall above
                        belowCharacter = theBoard[j+16][ABOVE];
                    }
                    for(int i = 0; i < 3; i++) {
                        cout << belowCharacter;        // Display the character
                    }
                    // Display extra spaces, if we're not at the end of a row
                    if( ((j+1) % BOARD_EDGE) != 0) {
                        cout << " ";
                    }
                }//end for( int j...
                cout << " |" << endl;       // display the right boundary
            }//end if( i< BOARD_SIZE...
            
        }//end if( ((i+1...
        
    }//end for( int i=0;...
    
    // Display the bottom edge
    cout << "  ---------------------------------------------------------------- " << endl;
    cout << endl;
    
}

// Assigns the user a robot number and destination: selection, random or default assignment options
void Board::assignRobotAndDestination() {
    char userSelection;
    char destinationLetter = '\0';
    char robot = '\0';
    
    // Set the seed for random
    srand((unsigned int)time(0));
    
    cout << "Enter 'r' for random robot and goal, 'd' for default or 's' to select: ";
    cin >> userSelection;
    userSelection = toupper(userSelection);  // Convert input to lowercase
    
    // Check for valid input, prompt user to re-enter if invalid
    if ((userSelection != 'R') && (userSelection != 'S') && (userSelection != 'D') && (userSelection != 'X')) {
        cout << "Invalid entry.  Retry..." << endl;
        assignRobotAndDestination();
    }
    
    switch (userSelection) {
        case 'X':
            exit(0);  // Exit the game if X is entered
        case 'R':     // Selects a random robot for the user num 1-4
            robot = char(rand() % 4 + 1)+48;                  // Randomize robot num, convert to char
            destinationLetter = char((rand() % numDestinationPieces + 1)+ 64);  // Randomize a destination letter, convert to char
            break;
        case 'S':  // User selects robot and destination
            displayBoard();
            // Validate input for the correct number of robots & destination pieces
            do {
                cout << "Enter the goal robot number and the destination letter: " << endl;
                cin >> robot;
                if ((robot == 'X') || (robot == 'x')) exit(0);  // Exit game for 'X' input
                cin >> destinationLetter;
                destinationLetter = toupper(destinationLetter);  // Convert to uppercase
                if (destinationLetter == 'X') exit(0);
            } while ((!(robot >= 1) && (robot <= 4)) || (!((destinationLetter >= 'A') && (destinationLetter <= 'A'+numDestinationPieces))));
            break;
        case 'D':
            destinationLetter = 'M';
            robot = '2';
            break;
    }
    system("clear");
    cout << "Move robot " << robot << " to square " << destinationLetter << endl;
    
    // Set the winning destination letter and robot
    winningDestination = destinationLetter;
    winningRobot = robot;
    
    // Find the position of the winning destination, save it
    for (int i = 0; i < BOARD_SIZE; i++)
        if (theBoard[i][PIECE] == destinationLetter) winningLetterIndex = i;
    
    // Reset all letters on the board except for the destination piece
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((theBoard[i][PIECE] != destinationLetter)) {
            if ((theBoard[i][PIECE] != '1') && (theBoard[i][PIECE] != '2') &&
                (theBoard[i][PIECE] != '3') && (theBoard[i][PIECE] != '4')) {
                theBoard[i][PIECE] = '.';
            }
        }
    }
}


// Makes moves for up, down, left, right
// Parameters: robot number and the direction of movement
bool Board::makeMove(char robot, char direction, bool &winner) {
    int numRobot = robot-48;  // Convert the robot char to an int
    int robotPosition = theRobots[numRobot];
    int moveRobot = 0;
    
    switch (direction) {
        case 'L':  // Move left
            moveRobot = robotPosition-1;  // Set the direction of movement for the robot
            // If there is a wall to the immediate left or the right side of the preceding piece, the move cannot be made
            if (((theBoard[robotPosition][LEFT] == '|') || (theBoard[robotPosition-1][RIGHT] == '|')) ||
                (validateMove(direction, robotPosition))) {
                return true;
            }
            break;
        case 'R':  // Move right
            moveRobot = robotPosition+1;
            // If there is a wall to the immediate right of the current piece or to the left of the piece ahead, the move cannot be made
            if (((theBoard[robotPosition][RIGHT] == '|') || (theBoard[robotPosition+1][LEFT] == '|')) ||
                (validateMove(direction, robotPosition))) {
                return true;
            }
            break;
        case 'U':  // Move up
            moveRobot = robotPosition-16;
            // If there is a wall directly above the current piece or is below the piece above, the move cannot be made
            if (((theBoard[robotPosition][ABOVE] == '-') || (theBoard[robotPosition-16][BELOW] == '-')) ||
                (validateMove(direction, robotPosition))) {
                return true;
            }
            break;
        case 'D':  // Move down
            moveRobot = robotPosition+16;
            // If there is a wall directly below the current piece or above the piece below, the move cannot be made
            if (((theBoard[robotPosition][BELOW] == '-') || (theBoard[robotPosition+16][ABOVE] == '-')) ||
                (validateMove(direction, robotPosition))) {
                return true;
            }
            break;
    }
    // Set the location the robot is moving from to the default piece
    theBoard[robotPosition][PIECE] = '.';
    
    if ((moveRobot == winningLetterIndex) && (robot == winningRobot)) {
        theRobots[numRobot] = moveRobot;
        robotPosition = theRobots[numRobot];
        theBoard[robotPosition][PIECE] = '0'+numRobot;
        winner = true;  // Return true to break out of the game loop in main
    }
    // If the piece being moved from is the location of the destination piece, once the robot moves out, it must display the destination letter
    if ((robotPosition == winningLetterIndex) && (theBoard[robotPosition][PIECE] != winningDestination) &&
        (numRobot != winningRobot)) {
        theBoard[robotPosition][PIECE] = winningDestination;
    }
    
    theRobots[numRobot] = moveRobot;                // Move the robot in the designated direction
    robotPosition = theRobots[numRobot];            // Set the robot
    theBoard[robotPosition][PIECE] = '0'+numRobot;  // Set the piece to the correct integer char
    makeMove(robot, direction, winner);             // Repeat until hitting a wall or another robot
    
    return false;
}


// Checks for valid user input on the robot number and direction of movement
bool Board::takeAndCheckInput(char robot, char direction) {
    if ((!((robot >= '1') && (robot <= '4'))) &&     // Displays message for both invalid robot number and direction
        ((direction != 'L') && (direction != 'R') && (direction != 'U') && (direction != 'D'))) {
        cout << "    Invalid robot number and direction.  Retry Input..." << endl;
        return false;
    }
    else if (!((robot >= '1') && (robot <= '4'))) {  // Invalid robot number
        cout << "    Invalid robot number.  Retry input..." << endl;
        return false;
    }
    else if ((direction != 'L') && (direction != 'R') && (direction != 'U') && (direction != 'D')) {  // Invalid direction
        cout << "    Invalid direction of movement.  Retry input..."  << endl;
        return false;
    }
    else return true;
}


// Checks if the destination is another robot, wall, or destination
// Returns a bool to calling function makeMove, which dictates whether or not to make the move
bool Board::validateMove(char direction, int robotPosition) {
    // Checks the piece the robot is trying to move over, returns true if it is another robot
    switch (direction) {
        case 'L':  // Left
            if ((theBoard[robotPosition-1][PIECE] >= '1') && (theBoard[robotPosition-1][PIECE] <= '4')) {
                return true;  // There is a robot to the left
            }
            else return false;
        case 'R':  // Right
            if ((theBoard[robotPosition+1][PIECE] >= '1') && (theBoard[robotPosition+1][PIECE] <= '4')) {
                return true;  // There is a robot to the right
            }
            else return false;
        case 'U':  // Up
            if ((theBoard[robotPosition-16][PIECE] >= '1') && (theBoard[robotPosition-16][PIECE] <= '4')) {
                return true;  // There is a robot above
            }
            else return false;
        case 'D':  // Down
            if ((theBoard[robotPosition+16][PIECE] >= '1') && (theBoard[robotPosition+16][PIECE] <= '4')) {
                return true;  // There is a robot below
            }
            else return false;
        case 'X':
            exit(0);
    }
    return false;
}


int main() {
    Board RobotGame;  // Declare object
    RobotGame.displayHeaderAndInfo();  // Display game instructions, header
    RobotGame.readFileAndModifyBoard();
    RobotGame.assignRobotAndDestination();
    RobotGame.displayBoard();
    
    int numTurn = 1;
    char robot, direction;
    bool winner = false;
    
    while (!winner) {
        do {
            cout << numTurn << ". Please enter the robot to move and the direction (e.g. 2 r): ";
            cin >> robot;      // Input robot
            if ((robot == 'X') || (robot == 'x')) exit(0);  // Exit game if X is entered
            cin >> direction;  // Input direction
            direction = toupper(direction);  // Convert to uppercase
            if (direction == 'X') exit(0);
        } while (!(RobotGame.takeAndCheckInput(robot, direction)));  // Repeat the loop while user input is invalid
        // Make the move
        // If the move trying to be made is an obstacle, don't iterate turn number and have them select another move before displaying the board
        if (RobotGame.makeMove(robot, direction, winner)) {
            cout << "There is already an obstacle there. Try again. " << endl;
            continue;
        }
        else {
            system("clear");
            cout << "Move robot " << RobotGame.winningRobot << " to square " << RobotGame.winningDestination << endl;
            RobotGame.displayBoard();
        }
        numTurn+=1;
    }
    // Display congratulatory message
    cout << "Congratulations, you won!  It only took you " << numTurn << " tries." << endl;
    
    return 0;
}
