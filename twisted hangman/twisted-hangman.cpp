#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cctype>
using namespace std;

void DisplayHeader() { 
    cout << "Class: CS 141                                      \n"
    << "Author: Kirun Haque and Michelle Baginski               \n"
    << "Lab: Tues 1pm (Kirun) and 11am (Michelle)               \n"
    << "System: C++ Mac Xcode                                   \n\n";
    
    cout << "Welcome to the Twisted Hangman Game!               \n"
    << "Enter the length of the word you want.                  \n"
    << "Your goal is to guess the word within 15 tries.         \n"
    << "So keep guessing letters until you guess the word!      \n\n";
}

// accesses the dictionary file, makes a dictionary vector of strings using the words in that file
vector<string> MakeDictionaryVector() {
    ifstream inputFile;                 // input file
    vector<string> dictionary;          // dictionary
    vector<string> modifiedDictionary;  // modified dictionary
    string word;                        // string variable for word in the dictionary
    int wordLength;                     // user's wanted word length
    unsigned long length;               // length for each word in the dictionary
    
    // verify access to file
    inputFile.open("dictionary.txt");
    if (!inputFile.is_open()) {
        cout << "Could not locate file.  Exiting..." << endl;  //If not, exit
        exit(-1);
    }
    
    // add each word from the file to the vector
    while (inputFile >> word) {
        unsigned long i = word.find('\'');  // do not include words with an apostrophe
        if (i > word.length()) {
            dictionary.push_back(word);
        }
    }
    
    // display starting number of words
    cout << "Starting with " << dictionary.size() << " words." << endl;
    cout << "What length word do you want? ";  //User will choose the length of the word
    cin  >> wordLength;
    // append the appropiate word length in a new vector
    for (int i = 0; i < dictionary.size(); i++) {
        length = dictionary.at(i).size();
        if (length == wordLength) { 
            modifiedDictionary.push_back(dictionary.at(i));
        }
    }
    
    cout << "We now have " << modifiedDictionary.size() << " words of length " << wordLength; // display the starting words number
    cout << endl << endl;
    ConvertToUpper(modifiedDictionary);  // convert each word in the dictionary to uppercase
    
    return modifiedDictionary;
}

// convert each word in the vector to uppercase and takes in a vector dictionary as its parameter
void ConvertToUpper(vector<string> &listWords) {
    string word, upperStr;            
    char letter;            // parsing the letters in the word

    // convert the word from lowercase to uppercase 
    for (int i = 0; i < listWords.size(); i++) {
        word = listWords.at(i);
        for (int j = 0; j < word.length(); j++) { 
            letter = word[j];
            letter = toupper(word[j]);
            upperStr.push_back(letter);         
        }
        listWords.at(i) = upperStr;            // add the uppercase string to the vector 
        word = "";
        upperStr = "";
    }
}

// displays letters used and found, and eliminates the appropiate words
void PlayGame(vector<string> &listWords, char userGuess, string &lettersGuessed, string &lettersFound) {
    // check with WordCounter function to not eliminate every word with an if statement
    if (WordCounter(listWords, userGuess) == true) {
        EliminateWords(listWords, userGuess);          		// if not, eliminate the words with the char
        lettersGuessed.push_back(userGuess);           		// append letters and space in a lettersGuessed string
        lettersGuessed.push_back(' ');
        AlphabeticalSort(lettersGuessed);              		// call function to sort the letters in the string
        
    }
    // if all words that have the same character, finalize the set of words so there are not 0 words left
    else {
        ChosenWords(listWords, lettersFound, userGuess);    // call the function that will choose the word and eliminate the appropiate words
        cout << "You found letter " << userGuess << endl;
    }

    CheckWin(lettersFound, listWords);                     // call the function that will exit the program if the user won.
    cout << "Now we have " << listWords.size() << " words." << endl << endl;
}

// checks if the word has been found
bool CheckWin(string &lettersFound, vector<string> &listWords) {
    int countUnderScores = 0;                       // count underscores
    unsigned long int wordLength;                   
    int unsigned long numWords = listWords.size();  
    wordLength= lettersFound.size();                // size of the string
    
    for (int i = 0; i < wordLength; ++i) {            
        if (lettersFound[i] == '_') {
            countUnderScores += 1;
        }
    }
    
    // if there are no underScores in the string, then display the winning message and exit the program
    if (countUnderScores == 0) {
        cout << "Now we have " << numWords - 1 << " words." << endl;
        cout << lettersFound << endl
        << " *** Congratulations, you did it! ***"        
        << "\n\nDone" << endl;
        exit(0);  // exit the program...
    }
    return false;
}

// counts the number of words that contain the user's letter guess
bool WordCounter(vector<string> &listWords, char userGuess) {
    int wordCount = 0;
    
    for (int i = 0; i < listWords.size(); i++) { // count how many words that contains the user's guessin the vector
        if (listWords.at(i).find(userGuess) != string::npos) {
            wordCount +=1;
        }
    }
    
    if (listWords.size() != wordCount) return true;
    else return false;                           // if the size of the vector = amount of wordCount, return false
}


// creates a string of the chosen word length that holds the places for the letters being used
string LettersFound(vector<string> &listWords) {
    unsigned long wordLength;
    string lettersUsed;
    
    // get the length of any single word in the list
    wordLength = listWords.at(0).size();
    
    // append place holders to the string for the length of the word with for loop
    for (int i = 0; i < wordLength; i++) {
        lettersUsed.push_back('_');
        lettersUsed.push_back(' ');
    }
    return lettersUsed;
}


// display characters used so far, sorts them in alphabetical order using selection sort
void AlphabeticalSort(string &lettersGuessed) {
    unsigned long int length, minIndex;
    length = lettersGuessed.length();
    
    // loop through all letters in the string, increment by 2 to disregard '_'
    for (int i = 0; i < length-2; i+=2) {
        minIndex = i;
        for(int j = i+2; j < length-1; j+=2 ) {
            if (lettersGuessed.at(j) < lettersGuessed.at(minIndex) ) {  // if the element is on the left is of a lesser ASCII value, it is the new min index
                minIndex = j;
            }
        }
        swap(lettersGuessed.at(minIndex), lettersGuessed.at(i));      	// swap appropriate  elements
    }
}


// removes all words that contain any occurence of letters chosen by the player
void EliminateWords(vector<string> &listWords, char userGuess) {
    for (int i = 0; i < listWords.size(); i++) {
        if(listWords.at(i).find(userGuess) != string::npos) {      // if located, erase the char from the vector
            listWords.erase(listWords.begin()+i);
            i-=1;
        }
    }
}


/* see if the word in the dictionary and lettersUsed string has matching patterns
   if there are matching pattern, then return false. If not, return true. */
bool PatternMatch (string wordCheck, string lettersFound, char userGuess){
    int unsigned long wordLength = wordCheck.size();
    
    // check if index in word matches with lettersFound string, then check rest of the letters in the word
    for (int i = 0; i < wordLength; ++i) {
        if (lettersFound[i*2] == userGuess) {           // if there's a match, check rest of the letters in the word
            for (int j = i+1; j < wordLength; ++j){
                if ((wordCheck[j] == userGuess) && (wordCheck[j] != lettersFound[j*2])){
                    return false;                       //return false, so the word could be removed
                }
                if ((lettersFound[j*2] != '_') && (wordCheck[j] != lettersFound[j*2]))  {
                    return false;                       // return false, so the word could be removed
                }
            }
        }
    }
    return true; //Return true if the wordCheck and lettersFound string pattern match.
}


// eliminate the words that don't match the first word in the dictionary's pattern
void ChosenWords(vector<string> &listWords, string &lettersFound,  char userGuess) {
    string chosenWord;                  // first word in the vector
    string wordCheck;                   // check the rest of words in the vector
    int unsigned long charIndex;        // get the index position of the word.
    
    // pick the first word from the listWord vector
    chosenWord = listWords.at(0);
    charIndex = chosenWord.find(userGuess);  // locate index of the char in the word
    
    unsigned long int wordLength;
    wordLength = chosenWord.size();
    // assign the lettersUsed string to the user's guess to the appropiate index
    for (int i = 0; i < wordLength; ++i) {
        if (chosenWord[i] == userGuess) {
            lettersFound[i*2] = userGuess;
        }
    }
    
    // remove words that do not share the same character index
    for (int i = 0; i < listWords.size(); i++) {
        wordCheck = listWords.at(i);
        // remove word if the pattern of the word does not match with lettersFound string or if it doesn't have the same index letter
        if ((listWords.at(i).find(userGuess) != charIndex) || (!PatternMatch(wordCheck, lettersFound, userGuess))) {
            listWords.erase(listWords.begin()+i);
            i-=1;
        }
    }
}


int main() {
    DisplayHeader();
    int numTurn = 15;
    string lettersFound, lettersGuessed;
    char userGuess;
    vector <string> listWords = MakeDictionaryVector();
    lettersFound = LettersFound(listWords);
    
    while (numTurn > 0) {
        cout << numTurn << ". Letters used so far: " << lettersGuessed << endl;
        cout << "Letters found: " << lettersFound << endl;
        cout << "Guess a letter: "; // ask the user to guess a letter
        // take user letter guess input, convert it to uppercase
        cin >> userGuess;
        userGuess = toupper(userGuess);
        
        //Display the list of words if tilde is entered
        if (userGuess == '~') {
            for (int i = 0; i < listWords.size(); i++) {
                cout << listWords.at(i) << " ";
            }
            cout << endl << endl;
            continue;
        }
        PlayGame(listWords, userGuess, lettersGuessed, lettersFound);
        numTurn-=1;
    }
    // if the program was not exited before the loop, display losing message 
    cout << "Sorry, you lose..." << endl;
}
