// To check consistent spelling, your program must compile with this main with 
// no changes to this code whatsoever. This program terminates normally.
// Do NOT put an endl in operator<<.  

#include "intset.h"

int main() 
{
    IntSet A(7), B(15,3), C(10,5,8), D(12,5,10,12,-500), E, X(D), Y;

    // sets A and B acquire desired values of
    // A = { 3 5 7}
    // B = { 3 5 9 12 15}
    // from user during operator>>
    cout << "Enter ints for set A:  " << endl;
    cin >> A;
    cout << "Enter ints for set B:  " << endl;
    cin >> B;

    cout << "A = " << A << endl;               // outputs: A = { 3 5 7}
    cout << "B = " << B << endl << endl;       // outputs: B = { 3 5 9 12 15}

    cout << "Compute  C = A + B" << endl;
    C = A + B;                                 // C is replaced by A+B
    cout << "C = " << C << endl << endl;       // outputs: C = { 3 5 7 9 12 15}

    cout << "Ask if  A == C" << endl;
    cout <<  ( (A == C)? "A == C" : "A is not == C" ) << endl << endl;

    cout << "Ask if  A != C" << endl;
    cout <<  ( (A != C)? "A is not == C" : "A == C" ) << endl << endl;

    cout << "Compute  D = C - A * B" << endl;
    cout << "Before: D = " << D;                             // D = { 5 10 12}
    D = C - A * B;
    cout << "    After: " << "D = " << D << endl << endl;    // D = { 7 9 12 15}

    cout << "Test assignment operators" << endl;
    X = C = D = D;   
    Y += A -= B;
    cout << "X = " << X << "    Y = " << Y << endl << endl;

    // miscellaneous stuff
    cout << (D.isInSet(0) ? "0 in set" : "0 not in set" ) << endl;
    cout << (D.isInSet(-1000) ? "-1000 in set" : "-1000 not in set" ) << endl;
    cout << (A.isEmpty() ? "A empty" : "A not empty" ) << endl;
    bool success;
    success = D.insert(-20000);                  // handle value out of range
    success = D.remove(20000);                   // handle value out of range
    success = D.remove(-5000);                   // handle value out of range
    success = D.insert(0);                       // put 0 in the set
    success = D.insert(20000);                   // handle as you'd like

    IntSet a; // no-parameter constructor
    IntSet e(0, 1, 2); // variable-parameter constructor
    IntSet b(a); // copy constructor
    IntSet c = a; // copy constructor
    IntSet d;
    // variable parameter constructor, using MAXSUPPORTEDINT
    IntSet f(1, 2, 40, 90, MAXSUPPORTEDINT);
    // Variable parameter constructor, invalid values.
    IntSet g(-1, MAXSUPPORTEDINT + 1);

    g.insert(2);
    g.insert(50);
    g.insert(65);
    g.insert(MAXSUPPORTEDINT);

    d = e; // assignment operator
    d = d; // self-assignment; operator implementation must handle this

//    cout << "Enter integers to add to an IntSet." << endl;
//    cout << "Enter any non-numeric value to stop adding integers." << endl;
//    cin >> d;

    cout << "Union: d " << d << " + g " << g << endl;
    cout << d + g << endl << endl;

    cout << "Intersection: d " << d << " * g " << g << endl;
    cout << d * g << endl << endl;

    cout << "Difference: d " << d << " - g " << g << endl;
    cout << d - g << endl << endl;

    IntSet added = d;

    cout << "Union assignment: added " << added << " + g " << g << endl;
    added += g;
    cout << added << endl;
    added += added;
    cout << added << endl << endl;

    IntSet intersect(2, 5, 6, 7);

    cout << "Intersection assignment: intersect " << intersect << " + e " << e << endl;
    intersect *= e;
    cout << intersect << endl;
    intersect *= intersect;
    cout << intersect << endl << endl;

    IntSet difference(2, 5, 6, 7);
    cout << "Difference assignment: difference " << difference << " + e " << e << endl;
    difference -= e;
    cout << difference << endl;
    difference -= difference;
    cout << difference << endl << endl;

    cout << "Equality: d " << d << " == g " << g << endl;
    cout << (d == g) << endl << endl;

    cout << "Equality: d " << d << " == d " << d << endl;
    cout << (d == d) << endl << endl;
    IntSet copyOfD = d;

    cout << "Equality: copyOfD " << copyOfD << " == d " << d << endl;
    cout << (copyOfD == d) << endl << endl;

    cout << "Inequality: d " << d << " != g " << g << endl;
    cout << (d != g) << endl << endl;

    int insert = 30;

    cout << "Insert " << insert << " into d " << d << endl;
    d.insert(insert);
    cout << d << endl;
    cout << "isInSet " << insert << " d " << d << endl;
    cout << d.isInSet(insert) << endl << endl;;

    cout << "Remove " << insert << " from d " << d << endl;
    d.remove(insert);
    cout << d << endl;
    cout << "isInSet " << insert << " d " << d << endl;
    cout << d.isInSet(insert) << endl << endl;;

    cout << "d isEmpty " << d << endl;
    cout << d.isEmpty() << endl << endl;

   return 0;
}
