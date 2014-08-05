//-----------------------------------------------------------------------------
// Sort and output to STDOUT a list of STUDENT structs using a selection sort.
// The list is read from a file.
//
// Programmer: Ethan Crawford
//     Course: CSS 342
//       Date: April 4, 2007
//
// Assumptions:
//    -- None. Data must be in the form 'lastname firstname grade', 30 chars
//       max for lastname and firstname and valid integer for grade. Malformed 
//       input results in a no-op.
//-----------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <fstream>  
using namespace std; 

// Useful macro for checking the size of fixed-width buffers
#define ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))

#define MAXLENGTH 31     // 30 characters + a NULL
#define MAXCLASSSIZE 100 // 100 students max in a class

// Student struct
typedef struct
{                          
   char firstName[MAXLENGTH];   
   char lastName[MAXLENGTH];    
   int grade;
} STUDENT;

// function prototypes
int CompareStudentNames(const STUDENT& x, const STUDENT& y);
int CompareStudentGrades(const STUDENT& x, const STUDENT& y);
bool GetStudentData(STUDENT students[], int studentCount, int& recordCount);
void SelectionSort(STUDENT students[], int studentCount, bool compareNames);
bool GetData(STUDENT students[], int studentCount);
void PrintStudentList(STUDENT students[], int studentCount);
void PrintStudentData(const STUDENT& student);
template <typename T> void SwapObjects(T& x, T& y);

//----------------------------------  main  -----------------------------------
int main()
{
    // Grab enough stack space for max class size.
    STUDENT students[MAXCLASSSIZE] = {0};
    int size = 0;
    int recordCount = 0;

    // get student data from a file, fill array then sort and print
    if (GetStudentData(students, ARRAYSIZE(students), recordCount))
    {
        // sort the students in ascending order firstname/lastname and print the list
        SelectionSort(students, recordCount, true);

        // Print the output
        PrintStudentList(students, recordCount);
    }

    return 0;
}

//----------------------------  PrintStudentList ----------------------------
void PrintStudentList(STUDENT students[], int studentCount)
{
    int sum = 0;
    int i = 0;
    int max = 0;

    cout << "List of names sorted:" << endl;

    for (; i < studentCount; i++)
    {
        PrintStudentData(students[i]);

        sum += students[i].grade;

        if (students[i].grade > max)
        {
            max = students[i].grade;
        }
    }

    cout << endl << "Average grade: " <<  (sum / (i + 1)) << endl;

    // Resort the array on grade to determine the median
    SelectionSort(students, studentCount, false);

    cout << "Median grade: " <<  students[(i + 1) / 2].grade << endl;
    cout << endl << "Received highest grade: " << endl;

    i = 0;

    // Find and print the highest grades
    for (; i < studentCount; i++)
    {
        if (students[i].grade >= max)
        {
            PrintStudentData(students[i]);
        }
    }
}

//---------------------------  PrintStudentData -----------------------------
void PrintStudentData(const STUDENT& student)
{
    cout << student.grade << " " << student.firstName << 
        " " << student.lastName << endl;
}

//------------------------------- GetData -----------------------------------
bool GetStudentData(STUDENT students[], int studentCount, int& recordCount)
{
    char delim = ' ';
    char* fileName = "../data1.txt";
    ifstream infile(fileName);
    int index = 0;

    if (!infile)
    {
        cerr << "Could not open file " << fileName << endl;
        return false;
    }

    recordCount = 0;

    // Get as many records as possible
    // exit the loop if we are at the end of the file
    while (index < studentCount && !infile.eof())
    {
        // Return failure if the input could not be validated
        if (infile.fail())
        {   
            return false;
        }

        // Read the names, up to MAXLENGTH or until the delimiter is found.
        // This prevents buffer overruns when reading malformed input.
        infile.get(students[index].lastName, 
            ARRAYSIZE(students[index].lastName), delim);

        // Read and discard the delimiter
        infile.get();

        infile.get(students[index].firstName, 
            ARRAYSIZE(students[index].firstName), delim);

        // Read and discard the delimiter
        infile.get();

        // Use the overloaded shift operator to perform an easy atoi().
        infile >> students[index].grade;

        // Read and discard the \n
        infile.get();

        index++;
    }

    // the last item read will trigger an EOF, so we never 
    // want to count it as a record.
    recordCount = index - 1;

    infile.close();
    return true;
}

//------------------------------  SwapObjects -------------------------------
template <typename T> void SwapObjects(T& x, T& y) 
{
    if (&x == &y)
    {
        return;
    }

    T temp = x;
    x = y;
    y = temp;
}

//------------------------------  SelectionSort -----------------------------
void SelectionSort(STUDENT students[], int studentCount, bool compareNames)
{
    int minIndex = 0;

    for (int i = 0; i < studentCount - 1; i++)
    {
        minIndex = i;

        // Find the item of lowest value and set it to 'min'
        for (int j = i + 1; j < studentCount; j++)
        {
            if (compareNames)
            {
                if (CompareStudentNames(students[j], students[minIndex]) < 0)
                {
                    minIndex = j;
                }
            }
            else
            {
                if (CompareStudentGrades(students[j], 
                    students[minIndex]) < 0)
                {
                    minIndex = j;
                }
            }
        }

        SwapObjects(students[i], students[minIndex]);
    }
}

//---------------------------  CompareStudentNames --------------------------
int CompareStudentNames(const STUDENT& x, const STUDENT& y)
{
    int ret = 0;

    // Only compare the first names if the last names are identical
    if ((ret = strcmp(x.lastName, y.lastName)) == 0)
    {
        ret = strcmp(x.firstName, y.firstName);
    }

    return ret;
}

//--------------------------  CompareStudentGrades --------------------------
int CompareStudentGrades(const STUDENT& x, const STUDENT& y)
{
    int ret = 0;

    // Compare only grades
    if (x.grade > y.grade)
    {
        ret = 1;
    }
    else if (x.grade < y.grade)
    {
        ret = -1;
    }

    return ret;
}
