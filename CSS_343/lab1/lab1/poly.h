//---------------------------------------------------------------------------
// poly.h
// Polynomial class
// Author(s): Ethan Crawford
// Date: October 3, 2007
//---------------------------------------------------------------------------
// Polynomial class: represents a polynomial (an array of coefficients and 
// exponents)
//   Includes the following features:
//   -- Multiple constructors
//   -- A complete range of overloaded operators to perform assignment, 
//      calculation, and comparison.
//   -- Accessor for printing the polynomial, and mutator for loading 
//      multiple coefficients
//   -- Handles dynamic memory allocation and cleanup of coefficient array, 
//      as-needed.
//
// Assumptions:
//   -- user must enter valid integers when using >>. 
//   -- Exponent values smaller than 0 are ignored.
//---------------------------------------------------------------------------

#include <iostream>
#include <limits>

using namespace std; 

class Poly
{
    // Accessor: overloaded << operator
    // Outputs an entire polynomial, e.g., print " +5x^3 -2x^2 +4"  to 
    // represent the Poly 5 x cubed minus 2 x squared plus 4.
    // No new line or extra blanks beyond the sample are added
    // The power is not displayed for a power of 1 
    // The 'x' variable is not shown for a power of 0, e.g.,  +1x^4 +2x -10
    // Don't print a term if the coefficient is zero unless if all the
    // coefficients are zero, then print " 0".
    friend ostream& operator<<(ostream& output, const Poly& b);

    // Mutator: overloaded >> operator
    // Input all polynomial values in a loop, in the form 'coefficient 
    // power';
    // When done, enter 0 for both
    // E.g., to set Poly A(2,7) so that A is +5x^7 -4x^3 +10x –2,
    // the user enters the pairs in any order: 5 7  10 1  -4 3  -2 0  0 0 
    // on one line or multiple lines.
    friend istream& operator>>(istream& input, Poly& b);

public:
    // Zero-parameter constructor
    Poly();

    // Single-parameter constructor
    // Initializes the x^0 term
    Poly(int coeff);

    // Multi-parameter constructor
    Poly(int coeff, int power);

    // Copy constructor
    Poly(const Poly& b);

    // Destructor
    ~Poly();

    // Add two Polys and return the result
    Poly operator+(const Poly& b) const;

    // Subtract two Polys and return the result
    Poly operator-(const Poly& b) const;
    
    // Multiply two Polys and return the result
    Poly operator*(const Poly& b) const;

    // Assign one Poly to another
    Poly& operator=(const Poly& b);

    // Add one Poly to the current Poly
    Poly& operator+=(const Poly& b);

    // Subtract one Poly from the current Poly
    Poly& operator-=(const Poly& b);

    // Multiply one Poly with the current Poly
    Poly& operator*=(const Poly& b);

    // Compare two Polys; return true if they are the same
    bool operator==(const Poly& b) const;

    // Compare two Polys; return false if they are the same
    bool operator!=(const Poly& b) const;

    // Compare two polys; return true if this is less than b
    bool operator<(const Poly& b) const;

    // Compare two polys; return true if this is less than or equal to b
    bool operator<=(const Poly& b) const;

    // Compare two polys; return true if this is greater than b
    bool operator>(const Poly& b) const;

    // Compare two polys; return true if this is greater than or equal to b
    bool operator>=(const Poly& b) const;

    // Mutator: setCoeff()
    // set one coefficient, called setCoeff,  e.g.,  P.setCoeff(50,0); 
    // set x^0 coeff to 50
    void setCoeff(int coeff, int power);

    // Accessor: getCoeff
    // Get one term's coefficient, e.g., int coeff = P.getCoeff(2); 
    // cout << "coeff of power 2 of P is: " << coeff << endl;
    int getCoeff(int power) const;

private:
    // Initialize data members in constructors
    void init();

    // Clear out the set, freeing all memory and resetting all relevant variables
    void reset();

    // Copy the contents of src into dst
    void copyPoly(Poly& dst, const Poly& src) const;

    // Compare Polys a and b. Returns 1 if a is bigger than b,
    // -1 if b is bigger than a, and 0 if a is equal to b.
    int compare(const Poly& a, const Poly& b) const;

    int* m_rgPoly;
    int m_arraySize;
};
