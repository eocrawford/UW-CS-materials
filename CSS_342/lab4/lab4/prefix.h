#ifndef PREFIX_H
#define PREFIX_H

//---------------------------------------------------------------------------
// prefix.h
// Prefix expression evaluator class
// Author(s): Ethan Crawford (spec and partial code from Dr. Carol Zander)
// Date: May 9, 2007
//---------------------------------------------------------------------------
// Prefix expression class: evaluates prefix expressions and outputs postfix
//   expressions.
//   Includes the following features:
//   -- Parse and evaluate prefix expressions
//   -- Output postfix expressions
//
// Assumptions:
//   -- Expressions in input files are in a valid format
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
using namespace std;

int const MAXSIZE = 100;

class Prefix 
{
    friend ostream& operator<<(ostream&, const Prefix&);

public:
    Prefix() { expr[0] = '\0'; }                       
    void setPrefix(ifstream& infile);
    int evaluatePrefix() const;
    void outputAsPostfix(ostream&) const;

private:
    char expr[MAXSIZE];
    int evaluatePrefixHelper(int&) const;
    int evaluateOperation(const char op, const int one, const int two) const;
    void toPostfix(int&, char [], int&) const;
};

#endif
