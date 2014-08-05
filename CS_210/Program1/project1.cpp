#include "stdafx.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int n, m, msquared, nsquared, side1, side2, hypotenuse;

	cout << "Pythagorean triples calculator" << endl;
	cout << "------------------------------" << endl;
	cout << "Enter two integers" << endl;

	cin >> n >> m;

	// Given two positive integers, m and n, where m > n, a Pythagorean triple can be generated.
	if (m == n)
	{
		cout << "Error: one integer must be larger than the other to calculate a triple." << endl;
		goto Exit;
	}

	// Integers must be positive.
	if (m < 0 || n < 0)
	{
		cout << "Error: both integers must be positive to calculate a triple." << endl;
		goto Exit;
	}

	// Make sure that m > n
	if (m < n)
	{
		int swap = m;
		m = n;
		n = swap;
	}

	// Perform the calculations
	msquared = m * m;
	nsquared = n * n;
	side1 = msquared - nsquared;
	side2 = 2 * m * n;
	hypotenuse = msquared + nsquared;

	// Validate the calculations.
	if (msquared < 0 || 
		nsquared < 0 || 
		side2 < 0 ||
		hypotenuse < 0)
	{
		cout << "Error: Integer overflow" << endl;
		goto Exit;
	}

	cout << "[" << side1 << ", " << side2 << ", " << hypotenuse << "]" << endl << endl;

Exit:
	system("pause");
	return 0;
}
