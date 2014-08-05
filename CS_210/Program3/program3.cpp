#include <math.h>
#include <iostream>
#include <limits>
using namespace std;

/*
Course Name:  CS 210
Program Name: Program #3
Author:       Ethan Crawford
Date:         October 10th, 2006

Program Description: 
This program models a simple calculator. Each data entry line consists 
of a valid operator (from the list below), and a right-hand operand. 
The accumulated value in the calculator has an initial value of 0.0.
The running total is printed after every operation.

Operators:
+ ..Add
- ..Subtract
* ..Multiply
/ ..Divide
^ ..Power (raise left operand to the power of the right operand)

Any other character causes the program to emit a final total and exit.
*/

// LIST OF OPERATORS SUPPORTED BY THE CALCULATOR
enum Operator
{
	None,
	Addition,
	Subtraction,
	Multiplication,
	Division,
	Exponentiation
};

// FUNCTION PROTOTYPES

// DISPLAYS INSTRUCTIONS TO THE USER
void Usage();

// TAKES THE REQUIRED OPERATOR, THE OPERAND, AND THE CURRENT VALUE AND
// RETURNS THE RESULT OF THE CALCULATION.
double do_next_op(Operator op, double myOperand, double currentValue);

// GIVEN A CHARACTER, RETURNS THE ASSOCIATED OPERATOR
Operator GetOperatorFromChar(const char c);

void main() 
{
	const int bufferLength = 32;
	const int minInput = 2;
	char input[bufferLength];
	double currentValue = 0;

// DISPLAY USAGE
	Usage();

// LOOP FOREVER; EXIT THE LOOP WHEN SPECIFIC CONDITIONS ARE SATISFIED
	while (true)
	{
		Operator op = None;
		double operand = 0;

// READ STRING FROM STDIN. get() WILL NULL-TERMINATE FOR US.
		cin.get(input, bufferLength);

// ADVANCE THE STREAM TO THE END OF THE BUFFER, IN CASE THERE WAS EXTRA INPUT
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();

// MAP OPERATOR CHAR TO ENUM. THE PURPOSE OF THIS STEP IS TO MAKE IT EASY TO
// CHANGE THE USER INPUT SEQUENCES WITHOUT HAVING TO REWRITE THE REST OF THE
// LOGIC.
		op = GetOperatorFromChar(input[0]);

// NO OPERATION SPECIFIED; ASSUME THAT IT'S TIME TO EXIT
		if (None == op)
		{
			break;
		}

// VALIDATE THE INPUT
		if (strlen(input) < minInput)
		{
			break;
		}

// TREAT THE REST OF THE STRING AS A FLOATING-POINT DOUBLE
		operand = atof(input + 1);

// PERFORM THE CALCULATION
		currentValue = do_next_op(op, operand, currentValue);

// OUTPUT THE CURRENT VALUE
		cout << "Result so far is " << showpoint << currentValue << endl << endl;
	}

// OUTPUT THE FINAL VALUE
	cout << "The final result is " << showpoint << currentValue << endl;
    system("pause");
}

void Usage()
{
	cout << "This program is a simple calculator. To use it, enter data " << endl;
	cout << "at the prompt below. Each entry should consist of a valid" << endl;
	cout << "operator and operand. The accumulated value is displayed" << endl;
	cout << "after each operation." << endl << endl;
	cout << "Example: + 5" << endl << endl;
	cout << "To exit, enter 'q', '=', or any other character." << endl << endl;
	cout << "VALID OPERATORS:" << endl;
	cout << "+ (Add)" << endl;
	cout << "- (Subtract)" << endl;
	cout << "* (Multiply)" << endl;
	cout << "/ (Divide)" << endl;
	cout << "^ (Power)" << endl << endl;
}

double do_next_op(Operator op, double operand, double currentValue)
{
	double calculatedValue = currentValue;

	switch (op)
	{
		case Addition:
			calculatedValue += operand;
			break;

		case Subtraction:
			calculatedValue -= operand;
			break;

		case Multiplication:
			calculatedValue *= operand;
			break;

		case Division:
			calculatedValue /= operand;
			break;

		case Exponentiation:
			calculatedValue = pow(calculatedValue, operand);
			break;
	}

	return calculatedValue;
}

// GIVEN A STRING, RETURN THE ASSOCIATED OPERATOR
Operator GetOperatorFromChar(const char c)
{
	Operator op;

	switch (c)
	{
		case '+':
			op = Addition;
			break;

		case '-':
			op = Subtraction;
			break;

		case '*':
			op = Multiplication;
			break;

		case '/':
			op = Division;
			break;

		case '^':
			op = Exponentiation;
			break;

		default:
			op = None;
			break;
	}

	return op;
}
