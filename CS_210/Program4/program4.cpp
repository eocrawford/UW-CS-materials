#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

/*
Course Name:  CS 210
Program Name: Program #4
Author:       Ethan Crawford
Date:         October 17th, 2006

Program Description: 
This is a menu-driven program that verifies International Standard Book
Number formatting. The ISBNs are retrieved either from stdin or from 
a file named isbntest.txt. For each ISBN entered, the program
validates the ISBN format and prints 'valid' or 'invalid'.

The file format is:
##: an integer value between 0 and 20 that specifies the number of ISBN's to follow
ISBN (one per line)

An ISBN (International Standard Book Number) identifies a unique publication. 
An ISBN is ten digits. The first nine digits must be decimal digits (0...9). 
Up to three single dashes may be between any of the characters.
An ISBN must not begin or end with a dash.
The tenth digit is a checksum, calculated by taking modulo 11 of the sum of 
each digit multiplied by its position in the ISBN. The letter X corresponds 
to a value of 10.
*/

// FUNCTION PROTOTYPES

// VALIDATE THE ISBN - RETURNS TRUE IF THE ISBN IS VALID; FALSE OTHERWISE
bool ValidateISBN(const char* isbn);

// VALIDATE ISBNs IN A FILE - RETURNS TRUE IF THE FILE FORMAT IS VALID AND
// ALL ISBNS IN THE FILE ARE VALID; FALSE OTHERWISE.
bool ValidateFile(const char* isbnFile);

// DISPLAY THE MENU
void DisplayMenu();

// CLEAR THE STREAM BUFFER UP TO THE END OF THE LINE
void IgnoreRestOfLine(istream& stream);

// CONSTANTS
const char* g_filename = "isbntest.txt";
const int g_bufferLen = 0x100;

void main() 
{
    char command;
    char isbn[g_bufferLen];
    bool loop = true;

    cin.width(g_bufferLen);

    cout << "--------------------- ISBN Validator ---------------------" << endl;
    cout << "----------------------------------------------------------" << endl << endl;

    DisplayMenu();

    while (cin >> command)
    {
        bool ret;

// EXTRACT ONLY THE FIRST CHARACTER AND IGNORE THE REST
        IgnoreRestOfLine(cin);

// CANONICALIZE THE COMMAND
        command = tolower(command);

// EXECUTE THE SELECTION
        switch (command)
        {
// DISPLAY THE MENU
            case 'd':
                DisplayMenu();
                break;

// VALIDATE THE CONTENTS OF AN INPUT FILE
            case 'l':
                ret = ValidateFile(g_filename);
                break;

// VALIDATE AN ISBN FROM CIN
            case 'e':
                cout << "Enter ISBN:" << endl;
                cin >> isbn;
                IgnoreRestOfLine(cin);
                ret = ValidateISBN(isbn);
                break;

// EXIT THE APPLICATION
            case 'q':
                loop = false;
                break;

// TEST THE VALIDATION CODE WITH A PRESET LIST OF INPUTS
            case 't':
                ValidateISBN("0-201-88337-6");
                ValidateISBN("0-13-117334-0");
                ValidateISBN("0821211315");
                ValidateISBN("1-57231-866-X");
                ValidateISBN("0-201-8A337-6");
                ValidateISBN("0-201-88337-63");
                ValidateISBN("0-201-88-337-6");
                ValidateISBN("0-201883376");
                ValidateISBN("0-201-88337-3");
                ValidateISBN("-013-117334-0");
                ValidateISBN("157231--866-X");
                ValidateISBN("013-1134-0");
                ValidateISBN(NULL);
                ValidateISBN("");
                ValidateISBN("----------");
                break;

            default:
                cout << "Invalid entry, please try again." << endl;
                break;
        }

        if (! loop)
        {
            break;
        }
    }

    system("pause");
}

void DisplayMenu()
{
    cout << "Main Menu:" << endl;
    cout << "L: (L)oad ISBNs from a file named " << g_filename << endl;
    cout << "E: (E)nter ISBN using keyboard" << endl;
    cout << "D: (D)isplay the menu" << endl;
    cout << "T: (T)est the validation procedure with a list of preset values" << endl;
    cout << "Q: (Q)uit" << endl << endl;
}

bool ValidateFile(const char* isbnFile)
{
    ifstream infile(isbnFile);
    char isbn[g_bufferLen];
    int entryCount;
    const int minCount = 0;
    const int maxCount = 20;

// HANDLE FILE OPEN FAILURE
    if (infile.fail())
    {
        cout << "Could not open " << isbnFile << endl;
        return false;
    }

// SET THE INPUT BUFFER LENGTH TO AVOID OVERFLOW
    infile.width(g_bufferLen);

// THE FIRST LINE IS THE NUMBER OF ENTRIES TO READ FROM THE FILE
    infile >> entryCount;
    IgnoreRestOfLine(infile);

    if (entryCount < minCount || entryCount > maxCount)
    {
        cout << "Invalid entry count '" << entryCount << "'; the first line must contain " << endl <<
            "an integer value between " << minCount << " and " << maxCount << "." << endl;
        infile.close();
        return false;
    }

    for (int i = 0; i < entryCount; i++)
    {
// EXIT THE LOOP IF THERE ARE NO MORE ENTRIES TO BE READ
        if (infile.eof())
        {
            break;        
        }

// EXTRACT THE ISBN
        infile.getline(isbn, g_bufferLen);

// VALIDATE THE ISBN
        ValidateISBN(isbn);
    }

// CLOSE THE INFILE
    infile.close();
    return true;
}

void IgnoreRestOfLine(istream& stream)
{
    stream.ignore(numeric_limits<streamsize>::max(), '\n');
    stream.clear();
}

bool ValidateISBN(const char* isbn)
{
    const char dash = '-';
    const char ten = 'X';
    const int minDashes = 0;
    const int maxDashes = 3;
// AN ISBN MUST BE EITHER TEN OR THIRTEEN CHARACTERS (TEN DIGITS + 3 OPTIONAL DASHES)
    const int minLen = 10;
    const int maxLen = minLen + maxDashes;
// 11 IS THE MODULO VALUE USED TO CALCULATE THE CHECKSUM
    const int mod = 11;
    size_t len = 0;
    int dashCount = 0;
    int expectedCheckSum = 0;
    int actualCheckSum = 0;
    char* mem = NULL;
    char* str = NULL;
// RETURN VALUE FOR THE STRING VALIDATION LOOP
    bool isStringFormatValid = true;
// RETURN VALUE FOR ENTIRE FUNCTION
    bool isValid = false;
 
// THE ISBN STRING POINTER MUST NOT BE NULL.
    if (NULL != isbn)
    {
        len = strlen(isbn);
 
// AN ISBN MUST BE EITHER 10 OR 13 CHARACTERS.
        if (minLen == len || maxLen == len)
        {
// MAKE A LOCAL COPY OF THE ISBN FOR MODIFICATION PURPOSES
// ALLOCATE ENOUGH MEMORY FOR THE STRING + A NULL
            int cch = len + 1;

            mem = new char[cch];
 
            if (NULL != mem)
            {
                strcpy_s(mem, cch, isbn);
                str = mem;
            }
        }
    }

// IF STR IS NULL, THAT MEANS THE STRING LENGTH CHECK FAILED
// AND IT DOESN'T MAKE ANY SENSE TO GO ANY FURTHER.
// A DASH MUST NOT BE IN THE FIRST POSITION.
    if (NULL != str && dash != *str)
    {
// GET A POINTER TO THE LAST CHARACTER IN THE ISBN
        char* pChecksum = str + len - 1;
        char prevChar;
        int currentPosition = 1;
 
// GET THE EXPECTED CHECKSUM AND REMOVE IT FROM THE STRING.
// THE CHECKSUM MUST EITHER BE A DECIMAL DIGIT OR AN 'X'.
        if (isdigit(*pChecksum))
        {
// CONVERT CHAR TO INT BY SUBTRACTING ASCII '0' FROM THE VALUE.
            expectedCheckSum = (*pChecksum - '0');
        }
        else if (ten == *pChecksum)
        {
            expectedCheckSum = 10;
        }
        else
        {
            isStringFormatValid = false;
        }
 
// OVERWRITE THE EXPECTED CHECKSUM BY NULL-TERMINATING THE STRING ONE CHARACTER 
// BEFORE THE END. THIS SIMPLIFIES THE STRING VALIDATION LOOP.
        *pChecksum = '\0';
 
// VALIDATE THE REST OF THE STRING AND CALCULATE THE ACTUAL CHECKSUM
        while (*str && isStringFormatValid)
        {
            char c = *str;
 
// EVERY CHARACTER MUST EITHER BE A DECIMAL DIGIT OR A DASH.
            if (isdigit(c))
            {
// CALCULATE RUNNING CHECKSUM BASED ON INTEGER VALUE OF DIGIT AND CURRENT STRING POSITION
                actualCheckSum += currentPosition * (c - '0');
                currentPosition++;
            }
            else if (dash == c)
            {
                dashCount++;
 
// DASHES MUST NOT BE SEQUENTIAL
                if (prevChar == c)
                {
                    isStringFormatValid = false;
                    break;
                }
 
// NO GREATER THAN THREE DASHES MAY BE PRESENT IN THE STRING
                if (dashCount > maxDashes)
                {
                    isStringFormatValid = false;
                    break;
                }
            }
// ERROR - INPUT WAS NOT A DECIMAL DIGIT OR A DASH.
            else
            {
                isStringFormatValid = false;
                break;
            }
 
            prevChar = *str;
            str++;
        }
 
// CALCULATE EXPECTED CHECKSUM
        actualCheckSum %= mod;
 
// EITHER ZERO OR THREE DASHES MUST BE PRESENT
// EXPECTED CHECKSUM MUST MATCH ACTUAL CHECKSUM
        if (isStringFormatValid &&
            (minDashes == dashCount || maxDashes == dashCount) &&
            actualCheckSum == expectedCheckSum)
        {
            isValid = true;
        }
    }
 
    delete[] mem;
    cout << "ISBN " << (NULL == isbn ? "NULL" : isbn) << " is " << (isValid ? "valid" : "invalid") << endl;
    return isValid;
}
