//---------------------------------------------------------------------------
// intset.h
// Simple Set class for integers
// Author(s): Ethan Crawford (spec from Dr. Carol Zander)
// Date: April 9, 2007
//---------------------------------------------------------------------------
// Mathematical Set class: stores sets of integers, kept within a bool array.
//   Includes the following features:
//   -- Allows input and output of the whole set.
//   -- Allows for comparison of 2 sets, element by element.
//   -- Exposes operators to perform union, intersection, and difference 
//      operations, as well as assignments of the results of the operations.
//   -- Handles dynamic memory allocation and cleanup of bool array, as-needed.
//
// Assumptions:
//   -- user must enter valid integers when using >>. Invalid integers are
//      ignored; non-integers or out-of-bounds integers cause the input
//      loop to exit.
//---------------------------------------------------------------------------

#include <iostream>
#include <limits>

using namespace std; 

// Largest integer supported by the IntSet. Anything larger than this
// will be ignored.
const int MAXSUPPORTEDINT = 20000;

// Invalid value - used to set default values for multi-parameter constructor.
const int INVALIDVALUE = MAXSUPPORTEDINT + 1;

class IntSet
{
    // Display { 5 10 12 20 25 100} to represent the set containing the integers 
	// 5, 10, 12, 20, 25, and 100. The empty set is displayed as {}.
	// (One blank before each number.  Note that operator<< should not do an endl.)
    friend ostream& operator<<(ostream& output, const IntSet& b);

    // Input an entire set.  
    // (Note: This is a simple loop to take integers; terminate at
	// some sentinel value.  Ignore invalid integers.)
    friend istream& operator>>(istream& input, IntSet& b);

public:
    // Zero-parameter constructor
    IntSet();

    // Multi-parameter constructor
    IntSet(int one, 
           int two = INVALIDVALUE, 
           int three = INVALIDVALUE, 
           int four = INVALIDVALUE, 
           int five = INVALIDVALUE);

    // Copy constructor
    IntSet(const IntSet& b);

    // Destructor
    ~IntSet();

    // returns the union of two sets, the set of elements that 
    // are contained in either or both sets.
    IntSet operator+(const IntSet& b) const;

    // returns the intersection of two sets, the set of all 
    // elements that are common to both sets.
    IntSet operator*(const IntSet& b) const;

    // returns the difference of two sets, say A - B, which is 
    // the set of all elements that are in set A but not in set B.
    IntSet operator-(const IntSet& b) const;
    
    // the assignment operator to assign one set to another.
    IntSet& operator=(const IntSet& b);

    // the union assignment operator
    IntSet& operator+=(const IntSet& b);

    // the intersection assignment operator
    IntSet& operator*=(const IntSet& b);

    // the difference assignment
    IntSet& operator-=(const IntSet& b);

    // bool equality
    bool operator==(const IntSet& b) const;

    // bool inequality
    bool operator!=(const IntSet& b) const;

    // insert an element into a set at the specifed index of the bool array
    bool insert(int n);

    // remove an element from a set, bool return value, 
    // e.g.,  bool success = A.remove(10);
	// (Note:  ignore invalid integers, return false, 
    // e.g,.  bool success = A.remove(-10); ).
    bool remove(int n);

    // determine if a set is empty or not,  e.g.,  if (A.isEmpty())
    bool isEmpty() const;

    // determine if an integer is in the set or not,
    // e.g., if (A.isInSet(5)) ... (Returns false for invalid integers).
    bool isInSet(int n) const;

private:
    // Initialize data members in constructors
    void init();

    // Clear out the set, freeing all memory and resetting all relevant variables
    void reset();

    // Returns true if the integer is a valid entry for the set; false otherwise.
    bool isValidNumber(int n) const;

    // Copy the contents of src into dst
    void copySet(IntSet& dst, const IntSet& src) const;

    bool* m_rgSet;
    int m_arraySize;
};
