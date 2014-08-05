#include "intset.h"

//------------------------------- IntSet ------------------------------------
// No-parameter constructor.
IntSet::IntSet()
{
    init();
}

//------------------------------- IntSet ------------------------------------
// Multi-parameter constructor. Initializes a new set with multiple ints.
IntSet::IntSet(int one, int two, int three, int four, int five)
{
    init();

    insert(one);
    insert(two);
    insert(three);
    insert(four);
    insert(five);
}

//------------------------------- IntSet ------------------------------------
// Copy constructor. Initializes a new set with an existing set.
IntSet::IntSet(const IntSet& b)
{
    init();

    copySet(*this, b);
}

//------------------------------- ~IntSet -----------------------------------
// Destructor. Cleans up memory.
IntSet::~IntSet()
{
    reset();
}

//---------------------------------  +  -------------------------------------
// Overloaded addition operator +:
// Returns the union (elements contained in either or both sets) of the
// current object and an IntSet parameter.
IntSet IntSet::operator+(const IntSet& b) const
{
    IntSet intSet;

    copySet(intSet, b);
    copySet(intSet, *this);

    return intSet;
}

//---------------------------------  *  -------------------------------------
// Overloaded multiplication operator *:
// Returns the intersection (elements contained in both sets) of the
// current object and an IntSet parameter.
IntSet IntSet::operator*(const IntSet& b) const
{
    IntSet intSet;
    
    // Loop backwards to forwards to reduce the number of times 
    // the bool array must be resized.
    for (int i = b.m_arraySize - 1; i >= 0; i--)
    {
        if (b.isInSet(i) && isInSet(i))
        {
            intSet.insert(i);
        }
    }

    return intSet;
}

//---------------------------------  -  -------------------------------------
// Overloaded subtraction operator -:
// Returns the difference (elements that are in the current object (set A)
/// but not in set B) of the current object and an IntSet parameter.
IntSet IntSet::operator-(const IntSet& b) const
{
    IntSet intSet;

    // Loop backwards to forwards to reduce the number of times 
    // the bool array must be resized.
    for (int i = m_arraySize - 1; i >= 0; i--)
    {
        if (! b.isInSet(i) && isInSet(i))
        {
            intSet.insert(i);
        }
    }

    return intSet;
}

//---------------------------------  =  -------------------------------------
// Overloaded assignment operator =:
// Assigns the contents of the IntSet parameter to the current object.
IntSet& IntSet::operator=(const IntSet& b)
{
    if (this != &b)
    {
        reset();
        copySet(*this, b);
    }

    return *this;
}

//---------------------------------  +=  ------------------------------------
// Overloaded union assignment operator +=:
// Adds the union of the IntSet parameter to the current object.
IntSet& IntSet::operator+=(const IntSet& b)
{
    // The union of an object against itself is a functional no-op, so 
    // don't bother making the call if that is the case.
    if (this != &b)
    {
        return operator=(operator+(b));
    }

    return *this;
}

//---------------------------------  *=  ------------------------------------
// Overloaded intersection assignment operator *=:
// Sets the current object to the result of the intersection of the current
// object and the IntSet parameter
IntSet& IntSet::operator*=(const IntSet& b)
{
    // Intersecting an object against itself is a functional no-op, so 
    // don't bother making the call if that is the case.
    if (this != &b)
    {
        return operator=(operator*(b));
    }

    return *this;
}

//---------------------------------  -=  ------------------------------------
// Difference assignment operator -=:
// Adds the difference of the IntSet parameter to the current object.
IntSet& IntSet::operator-=(const IntSet& b)
{
    // The difference of an object against itself is a null set, so just
    // clear out the data if that is the case. The overloads will do that
    // for us, but this is slightly quicker.
    if (this != &b)
    {
        return operator=(operator-(b));
    }
    else
    {
        reset();
        return *this;
    }
}

//---------------------------------  ==  ------------------------------------
// Overloaded equality operator ==:
// Returns true if the IntSet parameter is the same as the current object;
// false otherwise.
bool IntSet::operator==(const IntSet& b) const
{
    bool ret = true;

    if (this == &b)
    {
        return true;
    }

    // Find the largest array. We can't just return false if the 
    // arrays are different sizes, because it can still potentially
    // be the same logical set.
    int leftSize = m_arraySize;
    int rightSize = b.m_arraySize;
    int size = (leftSize > rightSize) ? leftSize : rightSize;

    for (int i = 0; i < size; i++)
    {
        // If a difference is found, exit the loop early
        if (isInSet(i) != b.isInSet(i))
        {
            ret = false;
            break;
        }
    }

    return ret;
}

//---------------------------------  !=  ------------------------------------
// Overloaded inequality operator !=:
// Returns true if the IntSet parameter is different than the current object;
// false otherwise.
bool IntSet::operator!=(const IntSet& b) const
{
    return (! operator==(b));
}

//---------------------------------  <<  ------------------------------------
// Overloaded left-shift operator <<:
// Display { 5 10 12 20 25 100} to represent the set containing the integers 
// 5, 10, 12, 20, 25, and 100. The empty set is displayed as {}.
// (One blank before each number.  Note that operator<< should not do an endl.)
ostream& operator<<(ostream& output, const IntSet& b)
{
    int arraySize = b.m_arraySize;

    output << '{';

    for (int i = 0; i < arraySize; i++)
    {
        if (b.isInSet(i))
        {
            output << ' '  << i;
        }
    }

    output << '}';

    return output;
}

//---------------------------------  >>  ------------------------------------
// Overloaded right-shift operator >>:
// Input an entire set.  
// (Note: This is a simple loop to take integers; terminate at
// some sentinel value.  Ignore invalid integers.)
istream& operator>>(istream& input, IntSet& b)
{
    int n = INVALIDVALUE;

    // Take in integers. Non-integers will cause the
    // loop to terminate.
    while (input >> n)
    {
        b.insert(n);
    }

    // Ignore any remaining invalid characters.
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');

    return input;
}

//-------------------------------- insert -----------------------------------
// Insert an element into a set at the specified index.
// Ignore invalid integers.
// Return true if successful; false otherwise
bool IntSet::insert(int n)
{
    if (! isValidNumber(n))
    {
        return false;
    }

    // Is there already enough space to store this integer?
    if (n >= m_arraySize)
    {
        // No, so allocate a new array
        int size = n + 1;
        bool* rgNew = new bool[size];

        // Check for OOM
        if (! rgNew)
        {
            return false;
        }

        // Zero out the new array
        memset(rgNew, 0, size * sizeof(bool));

        if (m_rgSet != NULL)
        {
            // Copy the old array
            memcpy(rgNew, m_rgSet, m_arraySize * sizeof(bool));

            // Delete the old array
            delete[] m_rgSet;
        }

        // Assign the new array to the field
        m_rgSet = rgNew;

        // Save off the new array size
        m_arraySize = size;
    }

    // Perform the assignment
    m_rgSet[n] = true;

    return true;
}

//------------------------------- remove ------------------------------------
// remove an element from a set, bool return value, 
// e.g.,  bool success = A.remove(10);
// (Note:  ignore invalid integers, return false, 
// e.g,.  bool success = A.remove(-10); ).
bool IntSet::remove(int n)
{
    bool ret = false;

    if (! isValidNumber(n))
    {
        return false;
    }

    // Don't try to toggle the value unless
    // it's within our allocated range.
    if (n <= m_arraySize)
    {
        m_rgSet[n] = false;
        ret = true;
    }
    
    return ret;
}

//------------------------------ isEmpty ------------------------------------
// determine if a set is empty or not,  e.g.,  if (A.isEmpty())
bool IntSet::isEmpty() const
{
    bool ret = true;

    // Start looking through the array for a 'true' value
    for (int i = 0; i < m_arraySize; i++)
    {
        // If one is found, we have a non-empty set, so
        // no point in looking any further.
        if (m_rgSet[i])
        {
            ret = false;
            break;
        }
    }

    return ret;
}

//------------------------------ isInSet ------------------------------------
// determine if an integer is in the set or not,
// e.g., if (A.isInSet(5)) ... (Returns false for invalid integers).
bool IntSet::isInSet(int n) const
{
    bool ret = false;

    if (! isValidNumber(n))
    {
        return false;
    }

    // If the number is bigger than our current array, it's defintely
    // not in the set
    if (n < m_arraySize)
    {
        ret = m_rgSet[n];
    }

    return ret;
}

//------------------------------- init --------------------------------------
// Helper function to initialize data members in constructors
// Set up default state
void IntSet::init()
{
    m_rgSet = NULL;
    m_arraySize = 0;
}

//------------------------------- init --------------------------------------
// Clear out the set, freeing all memory and resetting all relevant variables
// Free any memory and clean out the set
void IntSet::reset()
{
    if (m_rgSet != NULL)
    {
        delete[] m_rgSet;
        m_rgSet = NULL;
        m_arraySize = 0;
    }
}

//----------------------------- isValidNumber--------------------------------
// Returns true if the integer is a valid entry for the set; false otherwise.
bool IntSet::isValidNumber(int n) const
{
    return (n >= 0 && n <= MAXSUPPORTEDINT);
}

//--------------------------- copySet -------------------------------------
// Copy the contents of src into dst
void IntSet::copySet(IntSet& dst, const IntSet& src) const
{
    // Loop backwards to forwards to reduce the number of times 
    // the bool array must be resized.
    for (int i = src.m_arraySize - 1; i >= 0; i--)
    {
        if (src.isInSet(i))
        {
            dst.insert(i);
        }
    }
}
