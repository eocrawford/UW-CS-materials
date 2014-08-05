#include <math.h>
#include <iostream>
#include <limits>
#include <cstdlib> 
#include <time.h>

using namespace std;

/*
Course Name:  CS 210
Program Name: Program #6
Author:       Ethan Crawford
Date:         November 25th, 2006

Program Description:
CS 210 Program #6

Lunar Lander, implemented using a C++ class.
Designs and implements a class that simulates a lunar lander, a small spacecraft 
that transports astronauts from lunar orbit to the surface of the moon, and a program 
that consumes the class to implement a lunar lander game, allowing the user to interact 
with the lander to attempt to achieve a safe lunar landing.
*/

// FUNCTION AND CLASS PROTOTYPES
void Usage();
void PlayGame();
void ClearScreen();
void IgnoreRestOfLine(istream& stream);

class CLunarLander
{
public:
    CLunarLander(
        double throttlePercentage,
        double verticalVelocity,
        double altitude,
        double fuelMass,
        double landerMass,
        double fuelBurnRate,
        double maxThrust);

//  GETTERS (MEMBER VARIABLE ACCESSOR FUNCTIONS)
    double GetThrottlePercentage();
    double GetVerticalVelocity();
    double GetAltitude();
    double GetFuelMass();
    double GetLanderMass();
    double GetFuelBurnRate();
    double GetMaxThrust();

//  SETTERS (MUTATOR FUNCTIONS)
    void SetThrottlePercentage(double throttlePercentage);
    void PassTime(double t);

private:
    double m_throttlePercentage;
    double m_verticalVelocity;
    double m_altitude;
    double m_fuelMass;
    double m_landerMass;
    double m_fuelBurnRate;
    double m_maxThrust;
    double m_gravitationalConstantOfTheMoon;
};

// GAME ENTRY POINT
void main() 
{
    bool fContinue = true;
    bool fPlayGame = false;
    char c;

    cout << "----------------------- LUNAR LANDER -----------------------" << endl;
    cout << "------------------------------------------------------------" << endl;

    while (fContinue)
    {
        cout << "Main Menu:" << endl;
        cout << "S: (S)tart a new game" << endl;
        cout << "H: (H)elp with the game" << endl;
        cout << "Q: (Q)uit" << endl;

        cin >> c;
        IgnoreRestOfLine(cin);
        c = tolower(c);

        switch (c)
        {
        case 's':
            fPlayGame = true;
            break;

        case 'h':
            Usage();
            break;

        case 'q':
            fPlayGame = false;
            fContinue = false;
            break;

        default:
            cout << "Invalid entry, please try again." << endl;
            break;
        }

        while (fPlayGame)
        {
            PlayGame();

            cout << "Play again? (Y/N)" << endl;
            cin >> c;
            IgnoreRestOfLine(cin);
            c = tolower(c);

            switch (c)
            {
                case 'y':
                    ClearScreen();
                    break;

                case 'n':
                    fPlayGame = false;
                    fContinue = false;
                    break;
            }
        }
    }

    system("pause");
}

void PlayGame()
{
    const int warningAltitude = 500;
    const int decelerateAltitude = 35;
    const int warningFps = -17;
    const double terminalVelocity = -3.0;
    double throttlePercentage = 0.0;
    double verticalVelocity = 0.0;
    double altitude = 1000.0;
    double fuelMass = 1700.0;
    double landerMass = 900;
    double fuelBurnRate = 10.0;
    double maxThrust = 5000.0;

    cout << "Please enter the starting throttle percentage (Enter 'd' for a default of " << (throttlePercentage * 100) << "%)." << endl;
    cin >> throttlePercentage;
    IgnoreRestOfLine(cin);

    cout << "Please enter the starting vertical velocity (Enter 'd' for a default of " << verticalVelocity << " m/s)." << endl;
    cin >> verticalVelocity;
    IgnoreRestOfLine(cin);

    cout << "Please enter the starting altitude (Enter 'd' for a default of " << altitude << " m)." << endl;
    cin >> altitude;
    IgnoreRestOfLine(cin);

    cout << "Please enter the starting fuel mass (Enter 'd' for a default of " << fuelMass << " kg)." << endl;
    cin >> fuelMass;
    IgnoreRestOfLine(cin);

    cout << "Please enter the spacecraft mass (Enter 'd' for a default of " << landerMass << " kg)." << endl;
    cin >> landerMass;
    IgnoreRestOfLine(cin);

    cout << "Please enter the fuel consumption rate at 100% throttle (Enter 'd' for a default of " << fuelBurnRate << " kg/s)." << endl;
    cin >> fuelBurnRate;
    IgnoreRestOfLine(cin);

    cout << "Please enter the maximum engine thrust (Enter 'd' for a default of " << maxThrust << " newtons)." << endl;
    cin >> maxThrust;
    IgnoreRestOfLine(cin);

// CONSTRUCT THE OBJECT
    CLunarLander lander(throttlePercentage, verticalVelocity, altitude, fuelMass, landerMass, fuelBurnRate, maxThrust);

// THE GAME STARTS WITH A SPACECRAFT AT A SPECIFIC ALTITUDE. IT ENDS WHEN THE CRAFT REACHES
// 0 FEET OF ALTITUDE. IF THE ENDING VERTICAL VELOCITY WAS <= -3 FPS AT THE TIME, THE PLAYER
// WINS. OTHERWISE, THE PLAYER LOSES.
// NASA SAYS THAT THE LUNAR MODULE DESCENDED AT THE RATE OF 3 FPS TO TOUCH DOWN
// SAFELY ON THE LANDING PAD. SEE http://history.nasa.gov/SP-287/ch4.htm. INTERNATIONALLY,
// DESCENT RATES BETWEEN 3 AND 5 FPS APPEAR TO BE WITHIN THE TOLERANCES REQUIRED BY THE
// DELICATE MACHINERY OF BOTH THE SPACECRAFT AND THE HUMAN BODY.
    while (lander.GetAltitude() > 0)
    {
        double burnTime = 1;

        ClearScreen();

        cout << "------------------------------------------------------------" << endl;

// THE PLAYER SHOULD BE NOTIFIED WHEN THE CRAFT IS ON A 'DANGEROUS APPROACH' TRAJECTORY, 
// DEFINED AS FPS > (17 - n) WHEN ALTITUDE < (500 - 35n).
// THIS MAPS TO THE HISTORICAL APPROACH VERTICAL VELOCITY OF NASA'S LUNAR LANDER.
        if (altitude <= warningAltitude)
        {
            int i = 0;

// CALCULATE THE SAFE APPROACH VELOCITY AND DISPLAY A WARNING IF IT'S EXCEEDED.
            while ((warningAltitude - (decelerateAltitude * i)) > altitude)
            {
                i++;
            }

            if (verticalVelocity < (warningFps + i) && verticalVelocity < terminalVelocity)
            {
                cout << "WARNING: CURRENT VELOCITY " << verticalVelocity << " f/s EXCEEDS MAXIMUM SAFE VELOCITY " << (warningFps + i) << " f/s." << endl;
            }
        }

// THE PLAYER SHOULD BE NOTIFIED WHEN THE CRAFT IS OUT OF FUEL.
        if (fuelMass <= 0)
        {
            cout << "WARNING: OUT OF FUEL. GOOD LUCK, CAPTAIN..." << endl;
        }

// THE PLAYER SHOULD BE NOTIFIED OF ALL RELEVANT STATS.
        cout << "Altitude: " << altitude << " f. Vertical velocity: " << verticalVelocity << " f/s." << endl;
        cout << "Fuel remaining: " << fuelMass << " kg. Throttle: " << (throttlePercentage * 100) << "%." << endl;
        cout << "------------------------------------------------------------" << endl;

// IF THERE IS FUEL REMAINING, THE PLAYER SHOULD BE GIVEN THE OPPORTUNITY TO 
// ADJUST THE THROTTLE PERCENTAGE AND THE BURN TIME
        if (fuelMass > 0)
        {
            cout << "Enter throttle percentage (a decimal value between 0 and 1)." << endl;
            cin >> throttlePercentage;
            IgnoreRestOfLine(cin);

            cout << "Enter burn time, in seconds (fractional seconds OK)." << endl;
            cin >> burnTime;
            IgnoreRestOfLine(cin);

            lander.SetThrottlePercentage(throttlePercentage);
        }

        lander.PassTime(burnTime);

        throttlePercentage = lander.GetThrottlePercentage();       
        verticalVelocity = lander.GetVerticalVelocity();
        altitude = lander.GetAltitude();
        fuelMass = lander.GetFuelMass();
    }

    if (verticalVelocity >= terminalVelocity)
    {
        cout << "Congratulations, Captain! Your country salutes your brilliant flight and safe landing." << endl;
    }
    else
    {
        cout << "******* YOU HAVE DIED. *******" << endl;
        cout << "At the time of your crash, your velocity was " << verticalVelocity << " f/s." << endl << endl;

        if (verticalVelocity <= -1000)
        {
            cout << "Your spacecraft pulverized into tiny dust particles," << endl;
            cout << "invisible among the other craters and lunar dust." << endl;
        }
        else if (verticalVelocity <= -500)
        {
            cout << "Your spacecraft crumpled like a tin can - unfortunately, you were inside it." << endl;
        }
        else if (verticalVelocity <= -100)
        {
            cout << "Sadly, the airbags did not go off as planned." << endl;
            cout << "Stupid GM parts..." << endl;
        }
        else if (verticalVelocity <= -11)
        {
            cout << "The jarring impact was enough to break the airlock seals, " << endl;
            cout << "and the merciless vacuum of space sealed your fate." << endl;
        }
        else if (verticalVelocity <= -10)
        {
            cout << "The spacecraft hull survived the crash intact, but the return" << endl;
            cout << "boosters failed. You starve to death after finishing the final" << endl;
            cout << "helping of Tang." << endl;
        }
    }
}

void Usage()
{
    ClearScreen();

    cout << "This game simulates a simple lunar lander." << endl;
    cout << "The goal is to land the spacecraft without crashing it." << endl << endl;
    cout << "This can be done by achieving a landing velocity of 3 feet per second" << endl;
    cout << "by the time the craft reaches 0 feet in altitude." << endl;
    cout << "You supply the spacecraft weight, fuel weight, starting altitude," << endl;
    cout << "starting vertical velocity, fuel burn rate, and maximum thrust." << endl;
    cout << "After that, you control the thruster burn rate and duration. " << endl;
    cout << "You can burn at a desired percentage of total thrust capacity " << endl;
    cout << "for a specific number of seconds." << endl << endl;
    cout << "Good luck, captain. The future of manned lunar exploration depends on you!" << endl << endl;
}

CLunarLander::CLunarLander(
    double throttlePercentage,
    double verticalVelocity,
    double altitude,
    double fuelMass,
    double landerMass,
    double fuelBurnRate,
    double maxThrust)
{
    m_throttlePercentage = throttlePercentage;
    m_verticalVelocity = verticalVelocity;
    m_altitude = altitude;
    m_fuelMass = fuelMass;
    m_landerMass = landerMass;
    m_fuelBurnRate = fuelBurnRate;
    m_maxThrust = maxThrust;
    m_gravitationalConstantOfTheMoon = 1.62;
}

double CLunarLander::GetThrottlePercentage()
{
    return m_throttlePercentage;
}

double CLunarLander::GetVerticalVelocity()
{
    return m_verticalVelocity;
}

double CLunarLander::GetAltitude()
{
    return m_altitude;
}

double CLunarLander::GetFuelMass()
{
    return m_fuelMass;
}

double CLunarLander::GetLanderMass()
{
    return m_landerMass;
}

double CLunarLander::GetFuelBurnRate()
{
    return m_fuelBurnRate;
}

double CLunarLander::GetMaxThrust()
{
    return m_maxThrust;
}


// OPEN THE SPACE CRAFT'S THROTTLE A GIVEN PERCENT. ANY NUMBER GREATER THAN
// ZERO MEANS THE CRAFT IS BURNING FUEL, WHICH SLOWS THE DESCENT.
void CLunarLander::SetThrottlePercentage(double throttlePercentage)
{
// THIS NUMBER IS A DECIMAL PERCENTAGE VALUE; IT CAN'T BE SMALLER THAN 0 OR GREATER THAN 1.
    if (throttlePercentage < 0 || throttlePercentage > 1.0)
    {
        return;
    }

    m_throttlePercentage = throttlePercentage;
}

// SIMULATE THE STATE WITHIN THE SPACECRAFT DURING THE PASSAGE OF t SECONDS
void CLunarLander::PassTime(double t)
{
    double loopIterations, timeInterval;

// ENSURE THAT THE INTERVAL IS POSITIVE AND NON-ZERO.
    if (t <= 0)
    {
        return;
    }

// 't' IS TIME IN SECONDS, BUT WE CALCULATE IN HUNDRETHS OF A SECOND TO
// INCREASE THE ACCURACY OF THE SIMULATION (CALCULUS COULD PROBABLY
// HELP US HERE, BUT THIS ISN'T A MATH CLASS...)
    loopIterations = t * 100;
    timeInterval = t / 100;

    for (int i = 0; i < loopIterations; i++)
    {
// ADJUST THE FUEL FLOW RATE IF WE ARE OUT OF FUEL
        if (m_fuelMass <= 0)
        {
            m_throttlePercentage = 0;
        }

// CALCULATE THE CHANGE IN VERTICAL SPEED BASED ON FUEL BEING BURNED, THE CURRENT WEIGHT 
// OF THE SPACECRAFT, AND THE PULL OF THE MOON'S GRAVITATION.
        m_verticalVelocity += (timeInterval * 
            (((m_throttlePercentage * m_maxThrust) / 
            (m_landerMass + m_fuelMass)) - m_gravitationalConstantOfTheMoon));

// CALCULATE THE CHANGE IN ALTITUDE BASED ON THE CURRENT VERTICAL SPEED
        m_altitude += timeInterval * m_verticalVelocity;

// CALCULATE THE FUEL BURNED, BASED ON THE CURRENT FLOW RATE AND THE MAXIMUM BURN RATE
        m_fuelMass -= timeInterval * m_throttlePercentage * m_fuelBurnRate;

// CHECK IF THE EAGLE HAS LANDED
        if (m_altitude < 0)
        {
            m_altitude = 0;
        }

// SEE IF WE RAN OUT OF GAS
        if (m_fuelMass < 0)
        {
            m_fuelMass = 0;
        }
    }
}

// IGNORE THE REST OF THE DATA IN A STREAM, UP TO THE NEWLINE
void IgnoreRestOfLine(istream& stream)
{
    stream.clear();
    stream.ignore(numeric_limits<streamsize>::max(), '\n');
}

// CLEAR THE SCREEN
void ClearScreen()
{
    system("cls");
}
