#include "moviemanager.h"

//-----------------------------------------------------------------------------
// MovieManager()
//-----------------------------------------------------------------------------
MovieManager::MovieManager()
{
    // Allocate an array of MovieMapping pointers as big as the number
    // of genres.
    m_prgMappings = new MovieMapping*[MovieGenreLast];
    memset(m_prgMappings, 0, sizeof(MovieMapping*) * MovieGenreLast);

    // Initialize the MovieMapping array
    for (int i = 0; i < MovieGenreLast; i++)
    {
        Movie* pFactory;
        string collectionName;

        getMovieData((MovieGenre)i, pFactory, collectionName);

        m_prgMappings[i] = new MovieMapping();

        m_prgMappings[i]->pFactory = pFactory;

        m_prgMappings[i]->pHash = new HashTable();

        m_prgMappings[i]->pTree = new BSTree();
        m_prgMappings[i]->pTree->setName(collectionName);
    }
}

//-----------------------------------------------------------------------------
// ~MovieManager()
//-----------------------------------------------------------------------------
MovieManager::~MovieManager()
{
    for (int i = 0; i < MovieGenreLast; i++)
    {
        if (NULL != m_prgMappings[i])
        {
            delete m_prgMappings[i]->pFactory;
            delete m_prgMappings[i]->pHash;
            delete m_prgMappings[i]->pTree;

            m_prgMappings[i]->pFactory = NULL;
            m_prgMappings[i]->pHash = NULL;
            m_prgMappings[i]->pTree = NULL;

            delete m_prgMappings[i];
            m_prgMappings[i] = NULL;
        }
    }

    delete[] m_prgMappings;
    m_prgMappings = NULL;
}

//-----------------------------------------------------------------------------
// create(char genre)
// Create a new movie, based on genre char
//-----------------------------------------------------------------------------
Movie* MovieManager::create(int index) const
{
    return dynamic_cast<Movie*>(m_prgMappings[index]->pFactory->create());
}

//-----------------------------------------------------------------------------
// add(ifstream& infile)
// Add a new movie using data provided in an ifstream
//-----------------------------------------------------------------------------
bool MovieManager::add(ifstream& infile)
{
    char mapChar;
    int i;
    Movie* pMovie;

    if (infile.eof() || !infile.good())
    {
        return false;
    }

    // Extract genre char
    infile >> mapChar;
    infile.get();
    mapChar -= 'A';

    // Validate genre
    if (!isValidMapChar(mapChar))
    {
        mapChar += 'A';
        cerr << "Invalid movie genre " << mapChar << endl;

        // Clean up
        infile.clear();
        infile.ignore(numeric_limits<streamsize>::max(), '\n');

        return false;
    }

    // Use the hash map to get the correct index of the MovieMapping array.
    i = genreMappings[mapChar];

    // Create a new movie of the appropriate genre
    pMovie = create(i);

    // setData can assume that data is properly formatted
    pMovie->setData(infile);

    // Store the movie
    m_prgMappings[i]->pHash->insert(pMovie);
    m_prgMappings[i]->pTree->insert(pMovie);

    return true;
}

//-----------------------------------------------------------------------------
// find(char genre, const string& key) const
// Find a movie in the inventory using the provided target movie
//-----------------------------------------------------------------------------
Movie* MovieManager::find(char genre, const string& key)
{
    genre -= 'A';
    Movie* pTarget = NULL;
    Movie* pRet = NULL;

    if (! isValidMapChar(genre))
    {
        genre += 'A';
        cerr << "Invalid movie genre " << genre << endl;

        return NULL;
    }

    pTarget = dynamic_cast<Movie*>(
        m_prgMappings[genreMappings[genre]]->pFactory->create());
    pTarget->setKey(key);

    pRet = dynamic_cast<Movie*>(
        m_prgMappings[genreMappings[genre]]->pHash->retrieve(*pTarget));

    delete pTarget;
    return pRet;
}

//-----------------------------------------------------------------------------
// print()
// Print a list of movies, sorted first by genre and then by genre-specific
// sort criteria
//-----------------------------------------------------------------------------
void MovieManager::print()
{
    cout << 
        "                                                                 " << 
        "*IN*    *OUT*" << endl;
    cout << "TITLE              DIRECTOR           YEAR MO ACTOR          " <<
        "   DVD VHS DVD VHS" << endl;

    for (int i = 0; i < MovieGenreLast; i++)
    {
        int max = m_prgMappings[i]->pTree->getSize();
        cout << endl << m_prgMappings[i]->pTree->getName() << endl;

        for (int j = 0; j < max; j++)
        {
            const Movie* pMovie = dynamic_cast<const Movie*>(
                m_prgMappings[i]->pTree->getData(j));

            const_cast<Movie*>(pMovie)->display();

            pMovie->displayCurrentState();
            cout << endl;
        }
    }
}

//-----------------------------------------------------------------------------
// getMovieData(MovieGenre genre, Movie*& pFactory, string& collectionName)
// Returns movie data associated with MovieGenre.
// Note that this code is only used in the constructor; all other
// Movie creation is handled through the hash table, to make the class
// performant.
//-----------------------------------------------------------------------------
void MovieManager::getMovieData(MovieGenre genre, Movie*& pFactory, 
    string& collectionName)
{
    pFactory = NULL;

    switch (genre)
    {
        case MovieGenreClassic:
            pFactory = new Classic();
            collectionName = "  *** Classics ***";
            break;

        case MovieGenreComedy:
            pFactory = new Comedy();
            collectionName = "  *** Comedies ***";
            break;

        case MovieGenreDrama:
            pFactory = new Drama();
            collectionName = "  *** Dramas ***";
            break;
    }
}

//-----------------------------------------------------------------------------
// isValidGenre(char c)
// Returns true if the genre code is valid; false otherwise
//-----------------------------------------------------------------------------
bool MovieManager::isValidGenre(char c)
{
    return isValidMapChar(c - 'A');
}

//-----------------------------------------------------------------------------
// isValidMapChar(char c)
// Returns true if the provided char is valid for the hashmap,
// false otherwise.
//-----------------------------------------------------------------------------
bool MovieManager::isValidMapChar(char c) const
{
    return (c < cGenreMappings && c >= 0 && genreMappings[c] != noMapping);
}
