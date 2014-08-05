#include <iostream>
#include <math.h>
using namespace std;

/*
Course Name:  CS 210
Program Name: Program #2
Author:       Ethan Crawford
Date:         October 1st, 2006

Program Description: 
This program calculates and displays:
1. The number of square centimeters of material needed to manufacture a 
   specified quantity of flat washers 
2. The weight of the left-over material after the washers are stamped.
*/

// CALCULATE THE AREA OF A SQUARE
// THE FORMULA IS LENGTH * WIDTH (the same value, in a square)
double GetSquareArea(double length)
{
    return (length * length);
}

// CALCULATE THE AREA OF A CIRCLE
// THE FORMULA IS PI * r^2
double GetCircleArea(double diameter)
{
    double pi = 4.0 * atan(1.0);
    double r = diameter / 2;

    return (pi * r * r);
}

// CALCLUATE THE WEIGHT OF WASTE LEFT OVER AFTER PRODUCING A SINGLE WASHER
double GetWasteWeight(double outerDiameter, double innerDiameter, double thickness, double density)
{
    double area, volume;

// CALCULATE THE AREA OF MATERIAL WASTED IN A SINGLE SQUARE
// THE FORMULA IS (TOTAL AREA - (AREA OF LARGER CIRCLE - AREA OF SMALLER CIRCLE))
    area = GetSquareArea(outerDiameter) - ((GetCircleArea(outerDiameter) - GetCircleArea(innerDiameter)));

// CALCULATE THE VOLUME OF MATERIAL WASTED IN A SINGLE SQUARE
// THE FORMULA IS AREA * THICKNESS (lwh)
    volume = area * thickness;

// CALCULATE THE WEIGHT OF MATERIAL WASTED IN A SINGLE SQUARE
// THE FORMULA IS VOLUME * DENSITY
    return volume * density;
}

void main() 
{
    double density, thickness, innerDiameter, outerDiameter, totalArea, totalWasteWeight;
    int quantity;

// GATHER USER PARAMETERS
    cout << "Washer calculator" << endl;
    cout << "------------------------------" <<	 endl;

    cout << "Enter thickness of material (cm)" << endl;
    cin >> thickness;

    cout << "Enter density of material (g/cm^3)" << endl;
    cin >> density;

    cout << "Enter washer inner diameter (cm)" << endl;
    cin >> innerDiameter;

    cout << "Enter washer outer diameter (cm)" << endl;
    cin >> outerDiameter;

    cout << "Enter washer quantity" << endl;
    cin >> quantity;

// VALIDATE PARAMETERS
    if (thickness <= 0 ||
        density <= 0 ||
        outerDiameter <= 0 ||
        innerDiameter < 0 || 
        quantity < 0 ||
        innerDiameter > outerDiameter)
    {
        cout << "Invalid parameter - check inputs." << endl;
    }
    else
    {
// GET THE TOTAL AREA OF MATERIAL REQUIRED TO PRODUCE 'quantity' WASHERS
        totalArea = quantity * GetSquareArea(outerDiameter);

// GET THE TOTAL WEIGHT OF THE MATERIAL WASTED IN PRODUCING 'quantity' WASHERS
        totalWasteWeight = quantity * GetWasteWeight(outerDiameter, innerDiameter, thickness, density);

// OUTPUT THE VALUES
        cout << endl << "Area of material required to manufacture " << quantity << " washer(s): " << totalArea << " sq. cm. " << endl;
        cout << "Total weight of waste material: " << totalWasteWeight << " g" << endl << endl;
    }

    system("pause");
}   
