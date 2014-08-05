#ifndef MOVIEMANAGER_H
#define MOVIEMANAGER_H

//---------------------------------------------------------------------------
// moviemanager.h
// Movie manager class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// MovieManager: creates and manages Movie objects, implementing 
// the Factory pattern and a perfect hashtable mapping genre code to 
// array index.
//
// Assumptions:
//   -- Input files are in the valid format for the type of object loading
//      them.
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "movie.h"
#include "classic.h"
#include "comedy.h"
#include "drama.h"
#include "hashtable.h"
#include "bstree.h"

const int noMapping = -1;

// Hash map between genre char and struct array index
const int genreMappings[] = 
{
    // 'A': no movie mapping
    noMapping,

    // 'B': no movie mapping
    noMapping,

    // 'C': Classic
    MovieGenreClassic,

    // 'D': Drama
    MovieGenreDrama,

    // 'E': no movie mapping
    noMapping,

    // 'F': Comedy
    MovieGenreComedy,
};

// Count of GenreMapping structs in array of structs
const int cGenreMappings = ARRAYSIZE(genreMappings);

class MovieManager
{
public:
    // Constructor
    MovieManager();

    // Destructor
    ~MovieManager();

    // Adds a new movie to the inventory using data provided in an ifstream
    bool add(ifstream& infile);

    // Finds a movie in the inventory using the provided target movie
    Movie* find(char genre, const string& key);

    // Prints a list of movies, sorted first by genre and then by 
    // genre-specific sort criteria
    void print();

    // Returns true if the genre code is valid; false otherwise
    bool isValidGenre(char c);

private:

    // Struct to group movie genre factory objects, and the
    // data stuctures that will hold collections of that genre.
    // The genre is determined by position in an array of 
    // MovieMappings, derived from the GenreMappings array.
    struct MovieMapping
    {
        MovieMapping() :
            pFactory(NULL),
            pHash(NULL),
            pTree(NULL)
        {
        }

        Movie* pFactory;
        HashTable* pHash;
        BSTree* pTree;
    };

    // Given a genre, gets a factory object and a collection name
    void getMovieData(MovieGenre genre, Movie*& pFactory, string& 
        collectionName);

    // Create a new movie, based on MovieMapping index
    Movie* create(int index) const;

    // Returns true if the provided char is valid for the hashmap,
    // false otherwise.
    bool isValidMapChar(char c) const;

    MovieMapping** m_prgMappings;
};

#endif // MOVIEMANAGER_H
