// DO NOT change anything in this file. Your code must compile with this main.

#include "poly.h"
// make sure the file containing the member function source is: poly.cpp
// both are lowercase file names
// assumes poly.h includes iostream
// this main does not do a thorough job of testing

using namespace std;

int main () {
   Poly A(5,7), B(3,4), C(2), D(A), X, Y;

   // set polynomials A and B to desired values
   // A = +5x^7 -4x^3 +10x -2
   // B = +3x^4 +1x^3 
   cout << "Enter terms for polynomial A.  Enter a coefficient " << endl
        << "then exponent for each term. Enter 0 0 to terminate." << endl;
   cin >> A;
   cout << "Enter terms for polynomial B.  Enter a coefficient " << endl
        << "then exponent for each term. Enter 0 0 to terminate." << endl;
   cin >> B;

   // outputs exactly what is in quotes: "A = +5x^7 -4x^3 +10x -2"
   cout << "A =" << A << endl;
   // outputs exactly what is in quotes: "B = +3x^4 +1x^3"
   cout << "B =" << B << endl << endl;

   C = A + B;  
   cout << "Compute C = A + B" << C << endl << endl;

   cout <<  ( (A == B) ? "A == B" : "A is not == B" ) << endl;
   cout <<  ( (A != B) ? "A is not == B" : "A == B" ) << endl;
   cout <<  ( (A < B) ? "A < B" : "A is not < B" ) << endl;
   cout <<  ( (A > B) ? "A > B" : "A is not > B" ) << endl;
   cout <<  ( (A <= B) ? "A <= B" : "A is not <= B" ) << endl;
   cout <<  ( (A >= B) ? "A >= B" : "A is not >= B" ) << endl << endl;

   cout << "Compute  D = A * B - 15" << endl;
   cout << "Before: D =" << D << "    and After: ";
   D = A * B - 15;
   cout << "D =" << D << endl << endl;

   cout << "Test assignment operators" << endl;
   X = C = D = D;    Y += A -= B *= A;
   cout << "X =" << X << "    Y =" << Y << endl << endl;

   cout << "Get and Set coefficient" << endl;
   int coeff = D.getCoeff(0); 
   cout << "coeff of subscript 0 of D is: " << coeff << endl;
   cout << "Doing getCoeff for term -20000" << endl;
   coeff = D.getCoeff(-20000);              // handle value out of range
   D.setCoeff(50,0);                        // set x^0 coeff to 50
   D.setCoeff(50,20000);                    // set x^20000 coeff to 50
   cout << "D =" << D << endl << endl;

   return 0;
}

