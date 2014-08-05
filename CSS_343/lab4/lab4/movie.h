#ifndef MOVIE_H
#define MOVIE_H

//---------------------------------------------------------------------------
// movie.h
// Base class representing a movie
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
//   -- Each implementation of a Movie class represents a unique movie, but
//      multiple copies of the film are represented in one Movie object.
//      This distinction is represented by an array of MovieStatus objects
//      within a single Movie.
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "nodedata.h"

const int MOVIECOPIES = 5;

// Struct to store the checked-out state of a copy of the movie
struct MovieStatus
{
    MovieStatus() :
        isCheckedOut(false),
        customerId(0)
    {
    }

    bool isCheckedOut;
    int customerId;
};

// Available movie formats
enum MovieFormat
{
    MovieFormatVideo,
    MovieFormatDVD,
    MovieFormatLast,
};

class Movie : public NodeData
{
public:
    // Constructor
    Movie();

    // Borrows one copy of this movie
    bool returnMovie(int customerId, char format);

    // Returns one copy of this movie
    bool borrowMovie(int customerId, char format);

    // Display the status of available copies
    void displayCurrentState() const;

    // Displays formatted info
    virtual void display() = 0;

    // Creates a new instance of a NodeData object
    virtual NodeData* create() = 0;

    // Sets a key to be used in a hashtable
    virtual void setKey(const string& hashKey) = 0;

    // Reads a single input line, populating internal variables
    virtual bool setData(ifstream& input) = 0;

    // overloaded < operator
    virtual bool operator<(const NodeData& b) const = 0;

    // overloaded > operator
    virtual bool operator>(const NodeData& b) const = 0;

    // overloaded == operator
    virtual bool operator==(const NodeData& b) const = 0;

    // overloaded != operator
    virtual bool operator!=(const NodeData& b) const = 0;

protected:
    // display a movie with the provided info
    void displayFormatted(const string& title, const string& director, 
        int year, int month, const string& actor) const;

    string m_director;
    string m_title;
    MovieStatus m_availVideo[MOVIECOPIES];
    MovieStatus m_availDVD[MOVIECOPIES];
    int m_year;
    const int m_width;

private:
    // Toggles the checked-out status of one copy of the desired format
    bool toggleStatus(bool state, int customerId, MovieFormat format);

    // Char to enum
    MovieFormat getFormat(char c);

    // Get the available and borrowed counts of the provided format
    void getCount(MovieFormat format, int& available, int& borrowed) const;
};

#endif // MOVIE_H
