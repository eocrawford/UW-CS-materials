#include "poly.h"

//---------------------------------  <<  ------------------------------------
// Overloaded left-shift operator <<:
// Outputs an entire polynomial, e.g., print " +5x^3 -2x^2 +4"  to 
// represent the Poly 5 x cubed minus 2 x squared plus 4.
// No new line or extra blanks beyond the sample are added
// The power is not displayed for a power of 1 
// The 'x' variable is not shown for a power of 0, e.g.,  +1x^4 +2x -10
// Don't print a term if the coefficient is zero unless if all the
// coefficients are zero, then print " 0".
ostream& operator<<(ostream& output, const Poly& b)
{
    bool fIsEmpty = true;

    // Print the polynomial from largest to smallest 
    // The array index is the power
    for (int i = b.m_arraySize - 1; i >= 0; i--)
    {
        int coeff = b.m_rgPoly[i];

        // Do not print the coefficient if it is 0
        if (coeff != 0)
        {
            fIsEmpty = false;

            // print sign and coefficient
            output << ' ';
            
            if (coeff > 0)
            {
                output << '+';
            }
            
            output << coeff;

            // Add a variable if the power > 0
            if (i > 0)
            {
                output << 'x';
            }

            // Add the power if > 1
            if (i > 1)
            {
                output << '^' << i;
            }
        }
    }

    // if all the coefficients are zero, then output " 0" 
    if (fIsEmpty)
    {
        output << " 0";
    }

    return output;
}

//---------------------------------  >>  ------------------------------------
// Overloaded right-shift operator >>:
// Input all polynomial values in a loop, in the form 'coefficient power';
// When done, enter 0 for both
// E.g., to set Poly A(2,7) so that A is +5x^7 -4x^3 +10x –2,
// the user enters the pairs in any order: 5 7  10 1  -4 3  -2 0  0 0 
// on one line or multiple lines.
istream& operator>>(istream& input, Poly& b)
{
    int coeff, power;

    // Get a coefficient and power while it's possible
    while (input >> coeff >> power)
    {
        if (coeff == 0 && power == 0)
        {
            break;
        }

        b.setCoeff(coeff, power);
    }

    return input;
}

//----------------------------------- Poly ----------------------------------
// Zero-parameter constructor
Poly::Poly()
{
    init();
}

//----------------------------------- Poly ----------------------------------
// Single-parameter constructor
// Initializes the x^0 term
Poly::Poly(int coeff)
{
    init();
    setCoeff(coeff, 0);
}

//----------------------------------- Poly ----------------------------------
// Multi-parameter constructor
Poly::Poly(int coeff, int power)
{
    init();
    setCoeff(coeff, power);
}

//----------------------------------- Poly ----------------------------------
// Copy constructor
Poly::Poly(const Poly& b)
{
    init();
    copyPoly(*this, b);
}

//---------------------------------- ~Poly ----------------------------------
// Destructor
Poly::~Poly()
{
    reset();
}

//------------------------------------ + ------------------------------------
// Add two Polys and return the result
Poly Poly::operator+(const Poly& b) const
{
    Poly poly;

    // Make a copy of ourselves. This will create any entries outside
    // the range of b.
    copyPoly(poly, *this);

    // Add b to this.
    for (int i = b.m_arraySize - 1; i >= 0; i--)
    {
        poly.setCoeff(b.getCoeff(i) + getCoeff(i), i);
    }
    
    return poly;
}

//------------------------------------ - ------------------------------------
// Subtract two Polys and return the result
Poly Poly::operator-(const Poly& b) const
{
    Poly poly;

    // Make a copy of ourselves. This will create any entries outside
    // the range of b.
    copyPoly(poly, *this);

    // Subtract b from this.
    for (int i = b.m_arraySize - 1; i >= 0; i--)
    {
        poly.setCoeff(getCoeff(i) - b.getCoeff(i), i);
    }
    
    return poly;
}

//------------------------------------ * ------------------------------------
// Multiply two Polys and return the result
Poly Poly::operator*(const Poly& b) const
{
    Poly poly;
    int polyCount = m_arraySize;
    int bCount = b.m_arraySize;

    // Distribute the terms.
    for (int i = 0; i < polyCount; i++)
    {
        for (int j = 0; j < bCount; j++)
        {
            int polyCoeff = getCoeff(i);
            int bCoeff = b.getCoeff(j);

            // Multiply coefficients and add exponents
            if (polyCoeff != 0 && bCoeff != 0)
            {
                int mltipliedPower = i + j;

                // Gather up like terms
                poly.setCoeff(poly.getCoeff(mltipliedPower) + 
                    (polyCoeff * bCoeff), mltipliedPower);
            }
        }
    }
    
    return poly;
}

//------------------------------------ = ------------------------------------
// Assign a Poly to this one
Poly& Poly::operator=(const Poly& b)
{
    if (this != &b)
    {
        reset();
        copyPoly(*this, b);
    }

    return *this;
}

//------------------------------------ += -----------------------------------
// Add one Poly to the current Poly
Poly& Poly::operator+=(const Poly& b)
{
    return operator=(operator+(b));
}

//------------------------------------ -= -----------------------------------
// Subtract one Poly from the current Poly
Poly& Poly::operator-=(const Poly& b)
{
    return operator=(operator-(b));
}

//------------------------------------ *= -----------------------------------
// Multiply one Poly with the current Poly
Poly& Poly::operator*=(const Poly& b)
{
    return operator=(operator*(b));
}

//------------------------------------ == -----------------------------------
// Compare two Polys; return true if they are the same
bool Poly::operator==(const Poly& b) const
{
    return (compare(*this, b) == 0);
}

//------------------------------------ != -----------------------------------
// Compare two Polys; return false if they are the same
bool Poly::operator!=(const Poly& b) const
{
    return !(*this == b);
}

//------------------------------------ < ------------------------------------
// Compare two polys; return true if this is less than b
bool Poly::operator<(const Poly& b) const
{
    return (compare(*this, b) < 0);
}

//------------------------------------ <= -----------------------------------
// Compare two polys; return true if this is less than or equal to b
bool Poly::operator<=(const Poly& b) const
{
    return (compare(*this, b) <= 0);
}

//------------------------------------ > ------------------------------------
// Compare two polys; return true if this is greater than b
bool Poly::operator>(const Poly& b) const
{
    return (compare(*this, b) > 0);
}

//------------------------------------ >= -----------------------------------
// Compare two polys; return true if this is greater than or equal to b
bool Poly::operator>=(const Poly& b) const
{
    return (compare(*this, b) >= 0);
}

//--------------------------------- setCoeff --------------------------------
// Mutator: setCoeff()
// set one coefficient, called setCoeff,  e.g.,  P.setCoeff(50,0); 
// set x^0 coeff to 50
void Poly::setCoeff(int coeff, int power)
{
    // Unsupported exponent value for this project.
    if (power < 0)
    {
        return;
    }

    // Is there already enough space to store this integer?
    if (power >= m_arraySize)
    {
        // No, so allocate a new array
        int size = power + 1;
        int* rgNew = new int[size];

        // Check for OOM
        if (! rgNew)
        {
            return;
        }

        // Zero out the new array
        memset(rgNew, 0, size * sizeof(int));

        if (m_rgPoly != NULL)
        {
            // Copy the old array
            memcpy(rgNew, m_rgPoly, m_arraySize * sizeof(int));

            // Delete the old array
            delete[] m_rgPoly;
        }

        // Assign the new array to the field
        m_rgPoly = rgNew;

        // Save off the new array size
        m_arraySize = size;
    }

    // Perform the assignment
    m_rgPoly[power] = coeff;
}


//--------------------------------- getCoeff --------------------------------
// Accessor: getCoeff
// Get one term's coefficient, e.g., int coeff = P.getCoeff(2); 
// cout << "coeff of power 2 of P is: " << coeff << endl;
int Poly::getCoeff(int power) const
{
    int ret = 0;

    if (power < m_arraySize && power >= 0)
    {
        ret = m_rgPoly[power];
    }

    return ret;
}

//----------------------------------- init ----------------------------------
// Initialize data members in constructors
void Poly::init()
{
    m_rgPoly = NULL;
    m_arraySize = 0;
}

//---------------------------------- reset ----------------------------------
// Clear out the set, freeing all memory and resetting all relevant variables
void Poly::reset()
{
    delete[] m_rgPoly;
    init();
}

//--------------------------------- copyPoly --------------------------------
// Copy the contents of src into dst
void Poly::copyPoly(Poly& dst, const Poly& src) const
{
    // Ensure dst capacity
    dst.setCoeff(0, src.m_arraySize - 1);

    // Copy the data
    memcpy(dst.m_rgPoly, src.m_rgPoly, src.m_arraySize * sizeof(int));

    // Set the arraysize counter
    dst.m_arraySize = src.m_arraySize;
}

//--------------------------------- compare ---------------------------------
// Compares Polys a and b. Returns 1 if a is bigger than b,
// -1 if b is bigger than a, and 0 if a is equal to b.
int Poly::compare(const Poly& a, const Poly& b) const
{
    int n = 0;
    int aSize = a.m_arraySize;
    int bSize = b.m_arraySize;

    // Dial down the actual array sizes, if there are leading zeros
    // at the top of the polynomial.
    while (a.getCoeff(aSize - 1) == 0 && aSize > 0)
    {
        aSize--;
    }

    while (b.getCoeff(bSize - 1) == 0 && bSize > 0)
    {
        bSize--;
    }

    if (aSize > bSize)
    {
        n = 1;
    }
    else if (aSize < bSize)
    {
        n = -1;
    }
    else
    {
        for (int i = aSize - 1; i >= 0; i--)
        {
            int aTerm = a.getCoeff(i);
            int bTerm = b.getCoeff(i);

            if (aTerm > bTerm)
            {
                n = 1;
                break;
            }
            else if (bTerm > aTerm)
            {
                n = -1;
                break;
            }
        }
    }

    return n;
}
