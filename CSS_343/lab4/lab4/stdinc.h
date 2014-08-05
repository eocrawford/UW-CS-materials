#ifndef STDINC_H
#define STDINC_H

//---------------------------------------------------------------------------
// stdinc.h
// Standard includes for the entire project
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <queue>

// Useful macro for checking the size of fixed-width buffers
#define ARRAYSIZE(a)  (sizeof(a)/sizeof(a[0]))

using namespace std;
using std::setw;

// Movie Genre
enum MovieGenre
{
    MovieGenreClassic = 0,
    MovieGenreComedy,
    MovieGenreDrama,

    // This value should always be last in the enum
    MovieGenreLast,
};

#endif // STDINC_H
