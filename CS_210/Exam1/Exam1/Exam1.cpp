#include <math.h>
#include <iostream>
#include <limits>
#include <cstdlib> 
#include <time.h>

using namespace std;

void Usage();
bool isapal(char* str);
const int bufLen = 512;
void reverse(int* xarray, int* yarray, int numvals);
void SplitNumberIntoWholeAndFractionalParts(double number, double* whole, double* fractional);

void main() 
{
    double whole, fractional;

    SplitNumberIntoWholeAndFractionalParts(5.32, &whole, &fractional);
    system("pause");
}

void SplitNumberIntoWholeAndFractionalParts(double number, double* whole, double* fractional)
{
    int wholeInt = (int)number;
    *whole = wholeInt;
    *fractional = (number - *whole);
}

void reverse(int* xarray, int* yarray, int numvals)
{
    for (int i = 0; i < numvals; i++)
    {
        yarray[numvals - i - 1] = xarray[i];
    }
}

bool isapal(char* phrase)
{
    bool isAPal = true;
    int len;
    char* endOfString;

    if (NULL == phrase)
    {
        return false;
    }

    len = strlen(phrase);
    endOfString = phrase + len - 1;

    while (*phrase)
    {
        while (*phrase && ! isalpha(*phrase))
        {
            phrase++;
        }

        if (! *phrase)
        {
            break;
        }
        
        while (! isalpha(*endOfString))
        {
            endOfString--;
        }
        
        if (tolower(*phrase) != tolower(*endOfString))
        {
            isAPal = false;
            break;
        }

        phrase++;
        endOfString--;
    }

    return isAPal;
}

void Usage()
{
    cout << "Determines whether or not a provided phrase is a palindrome" << endl;
    cout << "(a word or phrase that contains the same letter sequence whether" << endl;
    cout << "written backwards as well as forwards)" << endl;
}

