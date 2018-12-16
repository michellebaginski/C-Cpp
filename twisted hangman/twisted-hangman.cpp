#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cctype>
using namespace std;

vector<string> MakeDictionaryVector();
void ConvertToUpper(vector<string> &listWords);
string LettersFound(vector<string> &listWords);
void PlayGame(vector<string> &listWords, char userGuess, string &lettersGuessed, string &lettersFound);
void EliminateWords(vector<string> &listWords, char userGuess);
bool WordCounter(vector<string> &listWords, char userGuess);
void ChosenWords(vector<string> &listWords, string &lettersFound, char userGuess);
void AlphabeticalSort(string &lettersUsed);
bool CheckWin(string &lettersFound,vector<string> &listWords);
bool PatternMatch (string wordCheck, string lettersUsed,char userGuess);

void DisplayHeader() {
    cout << "Class: CS 141						\n"
	 << "Authors: Kirun Haque and Michelle Baginski			\n"
	 << "Lab: Tues 1pm (Kirun) and 11am (Michelle)			\n"
	 << "System: C++ Mac Xcode					\n\n";
		    
    cout << "Welcome to the Twisted Hangman Game!			\n"
    	 << "Enter the length of the word you want.			\n"
    	 << "Your goal is to guess the word within 15 tries.		\n"
   	 << "So keep guessing letters until you guess the word!		\n\n";
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

// converts each word in the vector to uppercase and takes in a vector as its parameters(the dictionary)
void ConvertToUpper(vector<string> &listWords) {
    string word, upperStr;           
    char letter;            // parse the letters of the word

    for (int i = 0; i < listWords.size(); i++) {
        word = listWords.at(i);
        for (int j = 0; j < word.length(); j++) { // convert each letter in the worr to uppercase
            letter = word[j];
            letter = toupper(word[j]);
            upperStr.push_back(letter);         // push back the capatalized letter to the new string
        }
        listWords.at(i) = upperStr;		// add the uppercase string to the vector element
        word = "";
        upperStr = "";
    }
}

// eliminate words that don't match the first word in the dictionary's pattern
void ChosenWords(vector<string> &listWords, string &lettersFound,  char userGuess) {
    string chosenWord;                  // first word in the vector
    string wordCheck;                   // checks against the rest of the words
    int unsigned long charIndex;        // stores index position of the word
    
    chosenWord = listWords.at(0);
    charIndex = chosenWord.find(userGuess); 
    unsigned long int wordLength;
    wordLength = chosenWord.size();

    // assign the lettersUsed string to the user's guess at the appropiate index
    for (int i = 0; i < wordLength; ++i) {
        if (chosenWord[i] == userGuess) {
            lettersFound[i*2] = userGuess;
        }
    }
    
    // remove words that do not share the same character index
    for (int i = 0; i < listWords.size(); i++) {
        wordCheck = listWords.at(i);
        // remove the word if the pattern of the word does not match with lettersFound, or if it does not have the same index letter
        if ((listWords.at(i).find(userGuess) != charIndex) || (!PatternMatch(wordCheck, lettersFound, userGuess))) {
            listWords.erase(listWords.begin()+i);
            i-=1;
        }
    }
}

// accesses dictionary file, makes a dictionary vector of strings using the words read in from the file
vector<string> MakeDictionaryVector() {
    ifstream inputFile;               
    vector<string> dictionary;          
    vector<string> modifiedDictionary;  
    string word;                        
    int wordLength;                     
    unsigned long length;	// length for each word in the dictionary           
    
    // verify access to file
    inputFile.open("dictionary.txt");
    if (!inputFile.is_open()) {
        cout << "Could not locate file.  Exiting..." << endl; 
        exit(-1);
    }
    
    // add each word from the file to the vector
    while (inputFile >> word) {
        unsigned long i = word.find('\'');  // do not include words with apostrophes
        if (i > word.length()) {
            dictionary.push_back(word);
        }
    }
    
    // display starting number of words
    cout << "Starting with " << dictionary.size() << " words." << endl;
    cout << "What length word do you want? ";  
    cin  >> wordLength;
    // append the words of the user's chosen length to the modified dictionary
    for (int i = 0; i < dictionary.size(); i++) {
        length = dictionary.at(i).size();
        if (length == wordLength) { 
            modifiedDictionary.push_back(dictionary.at(i));
        }
    }
    
    cout << "We now have " << modifiedDictionary.size() << " words of length " << wordLength; //Display the starting words number
    cout << endl << endl;
    ConvertToUpper(modifiedDictionary); 
    
    return modifiedDictionary;
}

// checks if the word has been found
bool CheckWin(string &lettersFound, vector<string> &listWords) {
    int countUnderScores = 0;                      
    unsigned long int wordLength;                  
    int unsigned long numWords = listWords.size(); 
    wordLength = lettersFound.size();                
    
    for (int i = 0; i < wordLength; ++i) {	// count the underscores
        if (lettersFound[i] == '_') {
            countUnderScores += 1;
        }
    }
    
    // if there are no underScores in the string, display the congratulatory message and exit
    if (countUnderScores == 0) {
        cout << "Now we have " << numWords - 1 << " words." << endl;
        cout << lettersFound << endl
        << " *** Congratulations, you did it! ***"      
        << "\n\nDone" << endl;
        exit(0);  
    }
    return false;
}

// counts the number of words that contain the user's letter guess
bool WordCounter(vector<string> &listWords, char userGuess) {
    int wordCount = 0;
    
    for (int i = 0; i < listWords.size(); i++) {	// count how many words contain the user's guess
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

// displays characters used so far, sorts them in alphabetical order using selection sort
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
        swap(lettersGuessed.at(minIndex), lettersGuessed.at(i));   	// swap the appropriate items
    }
}

/* check if the word in the dictionary and lettersUsed string has matching patterns
 * if there's matching pattern, then return false. If not, return true.		 */
bool PatternMatch (string wordCheck, string lettersFound, char userGuess){
    int unsigned long wordLength = wordCheck.size();
    
    // check if the index in the word matches with lettersFound string
    for (int i = 0; i < wordLength; ++i) {
        if (lettersFound[i*2] == userGuess) {           // if there's a match, check the rest of the letters in the word
            for (int j = i+1; j < wordLength; ++j) {
                if ((wordCheck[j] == userGuess) && (wordCheck[j] != lettersFound[j*2])) {
                    return false;                       // no word could be removed
                }
                if ((lettersFound[j*2] != '_') && (wordCheck[j] != lettersFound[j*2])) {
                    return false;                       // word can be removed
                }
            }
        }
    }
    return true; // true if the wordCheck and lettersFound string pattern match.
}

// displays letters used and found; eliminates the apropiate words
void PlayGame(vector<string> &listWords, char userGuess, string &lettersGuessed, string &lettersFound) {

    if (WordCounter(listWords, userGuess) == true) {
        EliminateWords(listWords, userGuess);           // eliminate the words with that char
        lettersGuessed.push_back(userGuess);          	// append the user's guess and a space
        lettersGuessed.push_back(' ');
        AlphabeticalSort(lettersGuessed);		// sort the letters in the string
        
    }
    // if all words that have the same character, finalize the set of words so there are not 0 words left
    else {
        ChosenWords(listWords, lettersFound, userGuess);    // call the function that will choose the word and eliminate the appropiate words
        cout << "You found letter " << userGuess << endl;
    }
    
    CheckWin(lettersFound, listWords);                     // check for a winner, exit if found
    cout << "Now we have " << listWords.size() << " words." << endl << endl;
}


int main() {
    DisplayHeader();
    int numTurn = 15;
    string lettersFound;
    string lettersGuessed;
    char userGuess;
    vector <string> listWords = MakeDictionaryVector();
    lettersFound = LettersFound(listWords);
    
    while (numTurn > 0) {
        cout << numTurn << ". Letters used so far: " << lettersGuessed << endl;
        cout << "Letters found: " << lettersFound << endl;
        cout << "Guess a letter: "; 

        // take user letter guess input, convert it to uppercase
        cin >> userGuess;
        userGuess = toupper(userGuess);
        
        // display the list of words if tilde is entered
        if (userGuess == '~') {
            for (int i = 0; i < listWords.size(); i++) {
                cout << listWords.at(i) << " ";
            }
            cout << endl << endl;
            continue;
        } // call the function that will play the game and eliminate the appropiate words.
        PlayGame(listWords, userGuess, lettersGuessed, lettersFound);
        numTurn-=1;
    }
    // if the program was not exited before the loop, then display losing message after the loop
    cout << "Sorry, you lose..." << endl;

    return 0;
}

