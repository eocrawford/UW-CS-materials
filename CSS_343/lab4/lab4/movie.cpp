#include "movie.h"

//-----------------------------------------------------------------------------
// returnMovie(int customerId, MovieFormat format)
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// Movie class: Provides a template and common methods for implementing a 
// specific movie type.
//   Includes the following abstract (pure virtual) methods:
//   -- Do transaction
//   -- Display transaction output
//   -- Set transaction data
//
// Assumptions:
//   -- Input data is a valid format
//
// Remarks:
//      multiple copies of the film are represented in one Movie object.
//      This distinction is represented by an array of MovieStatus objects
//      within a single Movie.

//---------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Movie::Movie() :
    m_width(18)
{
}

//---------------------------------------------------------------------------
// Borrows one copy of this movie
//-----------------------------------------------------------------------------
bool Movie::returnMovie(int customerId, char format)
{
    return toggleStatus(true, customerId, getFormat(format));
}

//-----------------------------------------------------------------------------
// borrowMovie(int customerId, MovieFormat format)
// Returns one copy of this movie
//-----------------------------------------------------------------------------
bool Movie::borrowMovie(int customerId, char format)
{
    return toggleStatus(false, customerId, getFormat(format));
}

//-----------------------------------------------------------------------------
// toggleStatus(bool state, int customerId, MovieFormat format)
// Toggles the checked-out status of one copy of the desired format
//-----------------------------------------------------------------------------
bool Movie::toggleStatus(bool state, int customerId, MovieFormat format)
{
    MovieStatus* rgStatus = (MovieFormatDVD == format) ? m_availDVD :
        m_availVideo;
    int before = state ? customerId : 0;
    int after = state ? 0 : customerId;

    if (format >= MovieFormatLast)
    {
        cerr << "Invalid movie format" << endl;
        return false;
    }

    for (int i = 0; i < MOVIECOPIES; i++)
    {
        if (rgStatus[i].customerId == before && 
            rgStatus[i].isCheckedOut == state)
        {
            rgStatus[i].isCheckedOut = !state;
            rgStatus[i].customerId = after;
            return true;
        }
    }

    if (!state)
    {
        cerr << "No copies of movie available for borrowing" << endl;
    }
    else
    {
        cerr << "Could not return non-checked-out movie" << endl;
    }

    return false;
}

//-----------------------------------------------------------------------------
// displayCurrentState() const
// Display the status of available copies
//-----------------------------------------------------------------------------
void Movie::displayCurrentState() const
{
    int availDVD, borrowedDVD, availVHS, borrowedVHS;

    getCount(MovieFormatDVD, availDVD, borrowedDVD);
    getCount(MovieFormatVideo, availVHS, borrowedVHS);

    cout << setw(4) << availDVD << setw(4) << availVHS << setw(4) << borrowedDVD << borrowedVHS;
}

//-----------------------------------------------------------------------------
// displayFormatted()
// Display a movie formatted with the provided data
//-----------------------------------------------------------------------------
void Movie::displayFormatted(const string& title, const string& director, 
    int year, int month, const string& actor) const
{
    // Print all known movie info, truncated to m_width
    cout << left << setw(m_width) << title.substr(0, m_width) << " " << 
        setw(m_width) << director.substr(0, m_width) << " " << year <<
        " " << right << setw(2);

    // Print the month, if available
    if (month > 0) cout << month; else cout << " ";

    // Print the actor, if available
    cout << " " << left << setw(m_width) << actor.substr(0, m_width) << " ";
}

//-----------------------------------------------------------------------------
// getCount(MovieFormat format, int& available, int& borrowed)
// Get the available and borrowed counts of the provided format
//-----------------------------------------------------------------------------
void Movie::getCount(MovieFormat format, int& available, int& borrowed) const
{
    available = 0;
    borrowed = 0;
    const MovieStatus* pMovies = NULL;

    switch (format)
    {
        case MovieFormatDVD:
            pMovies = m_availDVD;
            break;

        case MovieFormatVideo:
            pMovies = m_availVideo;
            break;
    }

    if (NULL != pMovies)
    {
        // Iterate each copy and check its borrowed status
        for (int i = 0; i < MOVIECOPIES; i++)
        {
            if (pMovies[i].isCheckedOut)
            {
                borrowed++;
            }
            else
            {
                available++;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// getFormat(char c)
// Gets the movieformat enum value that corresponds to the char
//-----------------------------------------------------------------------------
MovieFormat Movie::getFormat(char c)
{
    MovieFormat ret = MovieFormatLast;

    switch (c)
    {
        case 'V': 
            ret = MovieFormatVideo;
            break;

        case 'D': 
            ret = MovieFormatDVD;
            break;
    }

    return ret;
}
