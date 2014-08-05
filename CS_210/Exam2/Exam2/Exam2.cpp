#include <math.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib> 
#include <time.h>
#include <string>

using namespace std;

/*
Course Name:  CS 210
Program Name: Final Exam
Author:       Ethan Crawford
Date:         December 1st, 2006

Program Description:
Date class (Points: 20)    
  Specify, Design, and Implement a class for an abstract data type that models a day of 
  the year.  The Date object should store three int values with the month between 1 and 
  12, day between 1 and 31 (Or, for SOME months, 28, 29, or 30), and year between 0 and 
  2100.  Provide AT LEAST the following: 

• default constructor should set dat to Jan 1, 2000

• constructor with three ints (M, D, Y)

• constructor with string and 2 ints - Month (as “January”) should be converted to an 
integer, and the day and the year from the 2 ints

• three accessors for the three data items

• displayMonth returns the Month as a String

• displayDate returns the entire Date as a string, using displayMonth for the month

• compareDates returns -1 if the Date object it’s applied to comes before the argument 
Date, 0 if the two dates are equal, and +1 if the Date object is after the argument Date

• validMonth returns true is the value is a valid month

 dayNumber returns the day number of the year (1 to 366) for a particular Date object

• Any other functions you think might be useful
 
*/


// FUNCTION PROTOTYPES

typedef struct TagMONTHINFO
{
    char* monthName;
    int daysInMonth;
} MONTHINFO, *PMONTHINFO;

MONTHINFO g_monthNames[] =
{
    {"January", 31},
    {"February", 28},
    {"March", 31},
    {"April", 30},
    {"May", 31},
    {"June", 30},
    {"July", 31},
    {"August", 31},
    {"September", 30},
    {"October", 31},
    {"November", 30},
    {"December", 31}
};

// MACRO FOR DETERMINING THE NUMBER OF ELEMENTS IN AN ARRAY OF STACK OBJECTS
#define  ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))

class CDate
{
public:
    CDate();

    CDate(int month, int day, int year);

    CDate(char* month, int day, int year);

    int GetYear();

    int GetMonth();

    int GetDay();

    char* displayMonth();

    char* displayDate();

    int compareDates(CDate& one);

    bool validMonth();

    int dayNumber(CDate& date);

private:
    char* MonthStringFromInt(int month);
    int MonthIntFromString(char* month);

    int m_day;
    int m_month;
    int m_year;
    
    char* m_monthName;
};

CDate::CDate()
{
    CDate(1, 1, 2000);
}

CDate::CDate(char* month, int day, int year)
{
    CDate(MonthIntFromString(month), day, year);
}

CDate::CDate(int month, int day, int year) :
    m_monthName(NULL)
{
    m_day = day;
    m_month = month;
    m_year = year;
}

int CDate::GetYear()
{
    return m_year;
}

int CDate::GetMonth()
{
    return m_month;
}

int CDate::GetDay()
{
    return m_day;
}

char* CDate::displayMonth()
{
    return MonthStringFromInt(m_month);
}

char* CDate::displayDate()
{
    int cch = 32;
    char* dateText = new char[cch];

    sprintf(dateText, "%s %d, %d", MonthStringFromInt(m_month), m_day, m_year);

    return dateText;
}

// • compareDates returns -1 if the Date object it’s applied to comes before the argument 
// Date, 0 if the two dates are equal, and +1 if the Date object is after the argument Date
int CDate::compareDates(CDate& one)
{
    int ret = 0;
    int dayOne = one.GetDay();
    int monthOne = one.GetMonth();
    int yearOne = one.GetYear();

    if (yearOne > m_year)
    {
        ret = -1;
    }
    else if (m_year > yearOne)
    {
        ret = 1;
    }
    else
    {
        if (monthOne > m_month)
        {
            ret = -1;
        }
        else if (m_month > monthOne)
        {
            ret = 1;
        }
        else
        {
            if (dayOne > m_day)
            {
                ret = -1;
            }
            else if (m_day > dayOne)
            {
                ret = 1;
            }
        }
    }

    return ret;
}

bool CDate::validMonth()
{
    return (m_month >= 1 && m_month <= 12);
}

//  dayNumber returns the day number of the year (1 to 366) for a particular Date object
int CDate::dayNumber(CDate& date)
{
    int dayNumber = 0;

    for (int i = 0; i < ARRAYSIZE(g_monthNames); i++)
    {
        if (0 == stricmp(MonthStringFromInt(date.GetMonth()), g_monthNames[i].monthName))
        {
            break;
        }

        dayNumber += g_monthNames[i].daysInMonth;
    }

// ACCOUNT FOR LEAP YEARS
    if (date.GetMonth() == 2 &&
        (0 == (year % 4) && 0 != (year % 100)) || (0 == (year % 400)))
    {
        dayNumber++;
    }

    dayNumber += date.GetDay();

    return dayNumber;
}

char* CDate::MonthStringFromInt(int month)
{
    return g_monthNames[month - 1].monthName;
}

int CDate::MonthIntFromString(char* month)
{
    int ret = -1;

    for (int i = 0; i < ARRAYSIZE(g_monthNames); i++)
    {
        if (0 == stricmp(month, g_monthNames[i].monthName))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

/*
Write a function that copies several data lines typed at the keyboard to a text file.  
The copy process should be terminated when the user enters a null line 
(a line with no characters - only the carriage return). 
CLEARLY STATE YOUR ASSUMPTIONS! (Such as about the necessary parameters, etc) */

void WriteUserInputToFile()
{
    const int bufLen = 512;
    char buffer[bufLen];
    char* outFileName = "out.txt";
    ofstream outfile;

// WE ARE ASSUMING THAT OUT.TXT OPENED SUCCESSFULLY
    outfile.open(outFileName);

    cin.width(bufLen);

    while (true)
    {
        cin.getline(buffer, bufLen);

// EMPTY STRING; WE ARE ASSUMING THAT THIS IS BECAUSE A \n was entered
        if (strlen(buffer) <= 0)
        {
            break;
        }

        outfile << buffer;
    }
}

/*
In some English words, the letters are accidentally in alphabetical order - for example, the words 
“hot”, “fry”, “fuzz”, “am”, “in”, “hoops”, and “almost”.  WRITE a function called CheckAlpha(), 
which takes a string and returns true if the letters are in alphabetical order, and false 
if they are not.  ( You may assume all characters in the string are valid, lowercase letters) 
*/
bool CheckAlpha(char* str)
{
    bool isAlphabetical = true;

    char largestVal = 0;

    while (*str)
    {
        if (*str < largestVal)
        {
// IF THE CURRENT VALUE IS SMALLER THAN ANY PREVIOUS VALUES, THE CHARACTER IS
// OUT OF SEQUENCE AND THE STRING IS NOT ALPHABETIZED.
            isAlphabetical = false;
            break;
        }
        else if (*str > largestVal)
        {
            largestVal = *str;
        }

        str++;
    }

    return isAlphabetical;
}

/*
  Write a function hyphenate() that takes as parameters a string s and an integer i, 
  with 0<= i <= s.length(), and inserts a hyphen (“-”) at index i in s.  
  For example, if s contained “knowhow”, then after called hyphenate() 
  with s and with i == 4, it would then contain “know-how”.  
  You may assume that i is in the indicated range, and that s is large enough to hold the extra character. 
 */
void hyphenate(std::string& s, int i)
{
    s.insert(i, "-");
}

void main() 
{
    cout << "------------------------ FINAL EXAM -------------------------" << endl;
    cout << "------------------------------------------------------------" << endl;
// Write a “while” loop equivalent to the following “for” loop:  (2 Points) 
int i,t;

bool foo = CheckAlpha("almost");

t = 0;
i = 0;

while (i < 12)
{
    t = t + i;
    cout << t;
    i += 3;
}
cout << endl;
    system("pause");
}
