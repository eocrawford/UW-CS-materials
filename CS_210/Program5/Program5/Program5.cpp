#include <math.h>
#include <iostream>
#include <limits>
#include <cstdlib> 
#include <time.h>

using namespace std;

/*
Course Name:  CS 210
Program Name: Program #5
Author:       Ethan Crawford
Date:         November 6th, 2006

Program Description:
Plays the game of "Hangman", wherein the user tries to guess the letters of a word.
Each time an incorrect letter is chosen, a new portion of a hanged man is drawn. 
If the fully hanged man figure is drawn, the user loses the game. If the user guesses 
the word correctly within the maximum number of guesses, the user wins. 

The user is permitted to choose between playing against the computer or against 
another human. If playing against the computer, the computer chooses from a built-in 
list of words, picking one of them at random; If playing against another human, the 
human "hangman" enters the word via cin.

The player is asked for the maximum number of incorrect guesses to allow the guesser, 
within the range 4 to 10 inclusive.

Before each guess entered by the player, the program displays the letters already chosen,
the number of remaining guesses, the portion of the word already guessed, inserting 
an asterisk for each unguessed letter, and the portion of the hanged man drawn for 
that turn, starting with a scaffold, then the head, body, arms, legs, face, etc...

Whe the player enters a character, the program checks to see if the character was previously 
attempted and if the character is non-alphabetic (e.g., ?, /, 4, $, etc.)

If the player correctly guesses a letter contained in the word, the displayed 
word is updated, with the appropriate asterisks replaced by the correctly guessed 
letter. (E.g., if the word were "EAGLE" and the user guessed "E", both the first and 
last letters would be filled in : E***E.)

If the player correctly guesses the entire word before using all of the allowed 
incorrect guesses, a congratulatory message is displayed. If, on the other hand, the 
player runs out of incorrect guesses, a message of condolence is displayed. 

When the game completes, the player is prompted to play another game. 'n' or 'N' terminates 
the program; otherwise it begins the entire game over again.
*/

// MACRO FOR DETERMINING THE NUMBER OF ELEMENTS IN AN ARRAY OF STACK OBJECTS
#define  ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))

const int g_intervalOffset = 4;
const char* g_startingFrame = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n____________________\r\n";

// EACH LINE BELOW REPRESENTS A FRAME IN THE HANGMAN ANIMATION.
const char* g_animationFrames[] = 
{
    g_startingFrame,
    g_startingFrame,
    "   ____________\r\n  |/* _________|\r\n  |*|/\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|          |\r\n  |*|          |\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|         /|\r\n  |*|        / |\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|         /|\\\r\n  |*|        / | \\\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|         /|\\\r\n  |*|        / | \\\r\n  |*|          |\r\n  |*|         /\r\n  |*|        /\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|         /|\\\r\n  |*|        / | \\\r\n  |*|          |\r\n  |*|         /\r\n  |*|       _/\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (   )\r\n  |*|         `-'\r\n  |*|         /|\\\r\n  |*|        / | \\\r\n  |*|          |\r\n  |*|         / \\\r\n  |*|       _/   \\\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
    "   ____________\r\n  |/* _________|\r\n  |*|/         |\r\n  |*|          |\r\n  |*|         .-.\r\n  |*|        (x x)\r\n  |*|         `-'\r\n  |*|         /|\\\r\n  |*|        / | \\\r\n  |*|          |\r\n  |*|         / \\\r\n  |*|       _/   \\_\r\n  |*|\r\n  |*|\r\n__|_|_______________\r\n",
};

// EACH INT ARRAY BELOW REPRESENTS THE NUMBER OF FRAMES TO BE ANIMATED
// WHEN THE NUMBER OF GUESSES IS BETWEEN 4 AND 10, INCLUSIVE.
const int g_animationIntervals[][11] = 
{
    {1, 2, 3, 3, 2},
    {1, 2, 2, 2, 2, 2},
    {1, 2, 2, 2, 2, 1, 1},
    {1, 2, 2, 2, 1, 1, 1, 1},
    {1, 2, 2, 1, 1, 1, 1, 1, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// BUILT-IN WORD LIST
const char* g_wordList[] = 
{
    "rhythm",
    "deoxyribonucleic",
    "orange",
    "tutelage",
    "lucre",
    "malleable",
    "disparate",
    "flippant",
    "canard",
    "alacrity",
    "scion",
    "ululate",
    "verdant",
    "patina",
    "harridan",
    "gloaming",
    "arrant",
    "spurious",
    "forfend",
    "gainsay",
};

// FUNCTION PROTOTYPES
void PlayGame();
int GetGuessCount(int minGuesses, int maxGuesses);
int GetGuess(char* word, int previousGuessCount, char* currentGuessList);
void DrawGame(char* displayWord, char* guesses, int currentGuess, int maxGuesses, bool fSolved);
void DrawAnimation(int nGuessCount, int nMaxGuesses);
char* GetWord();
bool FormatWord(char* formattedWord, size_t cch, char* word, char* guesses);
void ClearScreen();
void IgnoreRestOfLine(istream& stream);

void main() 
{
    bool fPlayGame = true;
    cout << "------------------------- HANGMAN --------------------------" << endl;
    cout << "------------------------------------------------------------" << endl;

    while (fPlayGame)
    {
        char c;

        PlayGame();

        cout << "Play again? (Y/N)" << endl;
        cin >> c;
        IgnoreRestOfLine(cin);
        c = tolower(c);

        switch (c)
        {
            case 'y':
                ClearScreen();
                break;

            case 'n':
                fPlayGame = false;
                break;
        }
    }

    system("pause");
}

void PlayGame()
{
// MIN GUESSES 4, MAX GUESSES 10.
    int minGuesses = g_intervalOffset;
    int maxGuesses = ARRAYSIZE(g_animationIntervals) - 1 + minGuesses;
// GET A WORD, EITHER FROM USER OR A HARDCODED ARRAY
    char* word = GetWord();

    if (NULL != word)
    {
 // GET THE DESIRED NUMBER OF GUESSES FROM USER
        int guessCount = GetGuessCount(minGuesses, maxGuesses);

        if (guessCount >= minGuesses && guessCount <= maxGuesses)
        {
            char guesses[26] = {0};
            char* formattedWord = NULL;
            int currentGuessCount = 0;
            size_t cch = strlen(word) + 1;
            bool fSolved = false;

            formattedWord = new char[cch];

            if (NULL != formattedWord)
            {
// PLAY THE GAME. BREAK OUT OF THE LOOP EITHER WHEN THE MAX GUESS COUNT IS EXCEEDED OR THE PUZZLE IS SOLVED.
                while (true)
                {
// FORMAT THE WORD, CHECKING TO SEE IF THE WORD HAS BEEN GUESSED.
                    fSolved = FormatWord(formattedWord, cch - 1, word, guesses);

// DRAW THE GAME BOARD, INFORMING IT IF THE GAME HAS BEEN WON
                    DrawGame(formattedWord, guesses, currentGuessCount, guessCount, fSolved);

                    if (currentGuessCount >= guessCount)
                    {
                        break;
                    }

// IF THE WORD HAS NOT BEEN GUESSED, GET A GUESS
                    if (fSolved)
                    {
                        break;
                    }
                    else
                    {
                        currentGuessCount = GetGuess(word, currentGuessCount, guesses);
                    }
                }
            }
        }
        else
        {
            cout << "Invalid guess count, please try again." << endl;
        }
    
        delete[] word;
    }
    else
    {
        cout << "Invalid word, please try again." << endl;
    }
}

// CLEAR THE SCREEN
void ClearScreen()
{
    system("cls");
}

// IGNORE THE REST OF THE DATA IN A STREAM, UP TO THE NEWLINE
void IgnoreRestOfLine(istream& stream)
{
    stream.ignore(numeric_limits<streamsize>::max(), '\n');
    stream.clear();
}

// GET A WORD TO GUESS, EITHER FROM THE USER OR FROM THE BUILT-IN LIST
char* GetWord()
{
    int bufferLen = 0x100;
    char c;
    char* ret = new char[bufferLen];
    const char* listWord = NULL;

    if (ret != NULL)
    {
        cout << "H: Play against another (H)uman" << endl;
        cout << "C: Play against the (C)omputer" << endl;

        cin.width(bufferLen);
        cin >> c;
        IgnoreRestOfLine(cin);

        c = tolower(c);

// MAKE SURE THE USER-ENTERED WORD CONTAINS ONLY VALID CHARACTERS
        switch (c)
        {
            case 'h':
                while (true)
                {
                    bool fValidWord = true;
                    size_t len;

                    cout << "Please enter a word:" << endl;
                    cin.width(bufferLen);
                    cin >> ret;
                    IgnoreRestOfLine(cin);

                    len = strlen(ret);

// 0-CHARACTER WORDS ARE INVALID
                    if (0 == len)
                    {
                        fValidWord = false;
                    }

                    for (unsigned int i = 0; i < len; i++)
                    {
                        if (! isalpha(ret[i])|| isspace(ret[i]))
                        {
                            fValidWord = false;
                            break;
                        }
                    }

                    if (!fValidWord)
                    {
                        cout << "Invalid word. Must contain only alphanumeric characters and no whitespace." << endl;
                        cout << "Please try again." << endl;
                    }
                    else
                    {
                        break;
                    }
                }
                break;

// PSEUDO-RANDOMLY SELECT A WORD FROM THE BUILT-IN LIST. THIS ISN'T REALLY THAT RANDOM, BUT IT'S GOOD
// ENOUGH FOR THIS GAME.
            case 'c':
            {
                srand(time(NULL));

                listWord = g_wordList[(rand() % (ARRAYSIZE(g_wordList) - 1))];

                strcpy_s(ret, bufferLen, listWord);

                break;
            }

            default:
                break;
        }

// MAKE THE WORD LOWER-CASE TO SIMPLIFY CHECKING LATER
        if (ret != NULL)
        {
            size_t len = strlen(ret);

// 0-LENGTH STRINGS ARE NOT VALID ENTRIES.
            if (len <= 0)
            {
                delete[] ret;
                ret = NULL;
            }

            for (unsigned int i = 0; i < len; i++)
            {
                ret[i] = tolower(ret[i]);
            }
        }
    }

    return ret;
}

// GET THE DESIRED NUMBER OF GUESSES FROM THE USER
int GetGuessCount(int minGuesses, int maxGuesses)
{
    int ret = 0;

    cout << "Enter number of guesses (minimum " << minGuesses << ", maximum " << maxGuesses << ")" << endl;

    cin >> ret;
    IgnoreRestOfLine(cin);

    return ret;
}

// DRAW THE GAME BOARD
void DrawGame(char* displayWord, char* guesses, int currentGuess, int maxGuesses, bool fSolved)
{
// DRAW HANGMAN
    DrawAnimation(currentGuess, maxGuesses);

// DRAW FORMATTED WORD
    cout << "Word: " << displayWord << endl;
    
// DRAW GUESS LIST (NULL)
    cout << "Current Guesses: '" << guesses << "'" << endl;

// DRAW REMAINING GUESSES
    cout << "Guesses remaining: " << (maxGuesses - currentGuess) << endl;

// DISPLAY APPROPRIATE CONSOLATION MESSAGE
    if (currentGuess >= maxGuesses)
    {
        cout << endl << "You failed to guess the word! The condemned man has been executed. Many condolences..." << endl << endl;
    }
    else if (fSolved)
    {
        cout << endl << "Hooray, you guessed the word! The condemned man lives another day." << endl << endl;
    }
}


// FORMAT THE DISPLAY WORD AND CHECK IF ALL CHARACTERS HAVE BEEN REVEALED

void DrawAnimation(int nGuessCount, int nMaxGuesses)
{
    int currentInterval = nMaxGuesses - g_intervalOffset;
    int currentFrame = 0;

// VALIDATE PARAMETERS
    if (nMaxGuesses < nGuessCount || 
        currentInterval > (ARRAYSIZE(g_animationFrames) - 1) ||
        nGuessCount > (ARRAYSIZE(g_animationIntervals) + g_intervalOffset - 1) ||
        currentInterval < 0)
    {
        return;
    }

// CALCULATE THE STARTING FRAME BY ADDING UP THE FRAMES THAT HAVE COME BEFORE.
    for (int i = 0; i < ARRAYSIZE(g_animationIntervals[currentInterval]) - 1; i++)
    {
// STOP COUNTING FRAMES IF WE HAVE REACHED THE CURRENT GUESS
        if (i >= nGuessCount)
        {
            break;
        }

        currentFrame += g_animationIntervals[currentInterval][i];
    }

// ANIMATE AS MANY FRAMES OF THE SEQUENCE AS REQUIRED BY THE MAX GUESS COUNT
// ADJUST FOR 0-INDEXED ARRAYS
    for (int frameCount = 0; frameCount < g_animationIntervals[currentInterval][nGuessCount]; frameCount++)
    {
        ClearScreen();
        cout << g_animationFrames[currentFrame++];
    }
}

bool FormatWord(char* formattedWord, size_t cch, char* word, char* guesses)
{
// CREATE A BOOLEAN ARRAY OF LENGTH cch
    bool* rgGuessedChars = new bool[cch];
    const char formattingChar = '*';
    bool fSolved = true;

    memset(rgGuessedChars, false, ((cch) * sizeof(bool)));

    if (rgGuessedChars && formattedWord && word && guesses)
    {
        size_t guessCount = strlen(guesses);

// PREPARE THE BUFFER FOR FORMATTING
        strcpy_s(formattedWord, cch + 1, word);

// FOR EACH GUESS IN GUESSES,
        for (unsigned int i = 0; i < guessCount; i++)
        {
            char* guessPosition = word;

// FIND ALL INSTANCES OF THE GUESSED CHARACTER IN THE WORD
            while ((guessPosition = strchr(guessPosition, guesses[i])) != NULL)
            {
// CHECK IF THE CHARACTER IS CONTAINED IN word.
// IF SO, FLAG THE BOOLEAN ARRAY AT THAT CHARACTER POSITION
// USE POINTER ARITHMETIC TO FIND THE STRING ARRAY INDEX
                rgGuessedChars[guessPosition - word] = true;
                guessPosition++;
            }
        }

// USING THE BOOLEAN ARRAY AS REFERENCE, FORMAT THE STRING. IF THE BOOL
// AT THE CORRESPONDING CHAR INDEX IS TRUE, REVEAL THE CHARACTER AT THAT
// LOCATION. OTHERWISE, DISPLAY AN ASTERISK.
// IF ALL ELEMENTS IN BOOL ARRAY ARE TRUE, RETURN TRUE FROM THE FUNCTION
// TO INDICATE THAT THE STRING HAS BEEN COMPLETELY REVEALED.
        for (unsigned int i = 0; i < cch; i++)
        {
            if (! rgGuessedChars[i])
            {
                fSolved = false;
                formattedWord[i] = formattingChar;
            }
        }
    }

    delete[] rgGuessedChars;
    return fSolved;
}

// GET A CHARACTER FROM THE USER AND VERIFY IT'S A VALID ENTRY AND HAS NOT BEEN USED BEFORE
int GetGuess(char* word, int previousGuessCount, char* currentGuessList)
{
    int currentGuessCount = previousGuessCount;
    char c;

    while (true)
    {
        cout << "Enter a guess: " << endl;
        cin >> c;
        IgnoreRestOfLine(cin);

// CHARACTER MUST BE ALPHANUMERIC AND NOT ALREADY ON THE GUESS LIST
        if (! isalpha(c))
        {
            cout << "Invalid value '" << c << "'; please try again." << endl; 
        }
        else if (strchr(currentGuessList, c))
        {
            cout << "You've already tried '" << c << "'; please enter another letter." << endl;
        }
        else
        {
            break;
        }
    }

    c = tolower(c);

// IF THE GUESS WAS NOT CORRECT, INCREMENT currentGuessCount
    if (! strchr(word, c))
    {
        currentGuessCount++;
    }

// SAVE THE GUESS FOR FUTURE REFERENCE
    currentGuessList[strlen(currentGuessList)] = c;

    return currentGuessCount;
}
