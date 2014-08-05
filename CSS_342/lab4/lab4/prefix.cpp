#include "prefix.h"

//-----------------------------------------------------------------------------
// setPrefix
// Set a prefix expression made of single digit operands, operators +,-,*,/
// and no blanks or tabs contained in a file. The expression is valid in
// that each operator has two valid operands.
//-----------------------------------------------------------------------------
void Prefix::setPrefix(ifstream& infile) { 
    infile >> expr;
}

//-----------------------------------------------------------------------------
// evaluatePrefix 
// Evaluate a prefix expr made of single digit operands and operators +,-,*,/
//-----------------------------------------------------------------------------
int Prefix::evaluatePrefix() const {
    if (expr[0] == '\0') 
        return 0;

    int index = -1;                               // to walk through expr
    return evaluatePrefixHelper(index);
}

//-----------------------------------------------------------------------------
// evaluatePrefixHelper
// Recursively evaluate a prefix expr made of single digit operands and 
// operators +,-,*,/
//-----------------------------------------------------------------------------
int Prefix::evaluatePrefixHelper(int& index) const 
{
    char symbol = expr[++index];
    int ret = 0;

    // Do nothing if we are at the end of the string
    if (symbol != '\0')
    {
        // Return an operand if found
        if (isdigit(symbol))
        {
            return (symbol - '0');
        }

        // Otherwise, we have a complete operation, so evaluate it recursively.
        ret = evaluateOperation(symbol, 
            evaluatePrefixHelper(index), 
            evaluatePrefixHelper(index));
    }

    return ret;
}

//-----------------------------------------------------------------------------
// evaluateOperation
// Evaluate a two-operand operation. operators +,-,*,/
//-----------------------------------------------------------------------------
int Prefix::evaluateOperation(const char op, const int one, const int two) const
{
    int ret = 0;

    switch (op)
    {
        case '+':
            ret = (one + two);
            break;

        case '-':
            ret = (one - two);
            break;

        case '*':
            ret = (one * two);
            break;

        case '/':
            ret = (one / two);
            break;

        default:
            break;
    }

    return ret;
}

//-----------------------------------------------------------------------------
// outputAsPostfix, toPostfix
// Convert prefix expression to postfix and output
//-----------------------------------------------------------------------------
void Prefix::outputAsPostfix(ostream& out) const {
    if (expr[0] == '\0') 
        return;

    int index = -1;                           // to walk through expr
    int count = -1;                           // used to build postfix array
    char postfix[MAXSIZE];
    toPostfix(index, postfix, count);
    postfix[++count] = '\0';
    out << postfix;
}

void Prefix::toPostfix(int& index, char postfix[], int& count) const {
    char symbol = expr[++index];
    int operandCount = 2;

    // Do nothing if we are at the end of the string
    if (symbol != '\0')
    {
        // If an operator is found, emit both operands, followed by the 
        // operator, in postfix format
        if (!isdigit(symbol))
        {
            for (int i = 0; i < operandCount; i++)
            {
                toPostfix(index, postfix, count);
            }
        }

        postfix[++count] = symbol;
    }
}

//-----------------------------------------------------------------------------
// operator<<
// display prefix expression as a string (char array)
//-----------------------------------------------------------------------------
ostream& operator<<(ostream& output, const Prefix& expression) {
    output << expression.expr;
    return output;
}
