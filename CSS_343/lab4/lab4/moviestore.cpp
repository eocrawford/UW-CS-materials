#include "moviestore.h"

//-----------------------------------------------------------------------------
// MovieStore()
// Initialize default movie store state
//-----------------------------------------------------------------------------
MovieStore::MovieStore()
{
    m_prgMovies = new BSTree*[MovieGenreLast];
    memset(m_prgMovies, 0, sizeof(BSTree*) * MovieGenreLast);

    m_prgMoviesHash = new HashTable*[MovieGenreLast];
    memset(m_prgMoviesHash, 0, sizeof(HashTable*) * MovieGenreLast);
}

//-----------------------------------------------------------------------------
// ~MovieStore()
// Clean up memory
//-----------------------------------------------------------------------------
MovieStore::~MovieStore()
{
    for (int i = 0; i < MovieGenreLast; i++)
    {
        if (m_prgMovies[i])
        {
            delete m_prgMovies[i];
        }
    }

    delete[] m_prgMovies;

    for (int i = 0; i < MovieGenreLast; i++)
    {
        if (m_prgMoviesHash[i])
        {
            delete m_prgMoviesHash[i];
        }
    }

    delete[] m_prgMoviesHash;
}

//-----------------------------------------------------------------------------
// initAccounts(ifstream& infile) const
// Initialize customer list
//-----------------------------------------------------------------------------
void MovieStore::initAccounts(ifstream& infile)
{
    while (true)
    {
        Customer* pCust = new Customer();

        if (infile.eof() || infile.bad() || !pCust->setData(infile))
        {
            delete pCust;
            break;
        }

        m_customers.insert(pCust);
        m_customersHash.insert(pCust);
    }
}

//-----------------------------------------------------------------------------
// initMovies(ifstream& infile) const
// Initialize movie list
//-----------------------------------------------------------------------------
void MovieStore::initMovies(ifstream& infile)
{
    while (true)
    {
        if (infile.eof() || infile.bad())
        {
            break;
        }

        m_movieManager.add(infile);
    }
}

//-----------------------------------------------------------------------------
// displayMovies() const
// Display list of movies contained in the store
//-----------------------------------------------------------------------------
void MovieStore::displayMovies() const
{
}

//-----------------------------------------------------------------------------
// displayHistory(ifstream& infile) const
// Display customer history
//-----------------------------------------------------------------------------
void MovieStore::displayHistory(ifstream& infile) const
{
}

//-----------------------------------------------------------------------------
// processTransactions(ifstream& infile) const
// Process a list of transactions from an input file
//-----------------------------------------------------------------------------
void MovieStore::processTransactions(ifstream& infile)
{
    while (true)
    {
        if (infile.eof() || !infile.good())
        {
            break;
        }

        char c;
        infile >> c;
        string key;
        stringstream ss;
        Customer target;

        switch (c)
        {
            // borrow and return
            case 'B':
            case 'R':
                int custId;
                char format;
                char genre;
                char sz[256];
                Customer* pCust;
                Movie* pMovie;

                infile >> custId >> format >> genre;
                infile.get();
                infile.get(sz, ARRAYSIZE(sz));
                key = sz;

                ss << custId;
                target.setKey(ss.str());

                // Clean up
                infile.clear();
                infile.ignore(numeric_limits<streamsize>::max(), '\n');

                // Validate genre
                if (!m_movieManager.isValidGenre(genre))
                {
                    cerr << "Invalid genre '" << genre << "'" << endl;
                    continue;
                }

                // Retrieve customer
                pCust = dynamic_cast<Customer*>(m_customersHash.retrieve(target));

                // Retrieve movie
                pMovie = m_movieManager.find(genre, key);

                // Validate customer
                if (NULL == pCust)
                {
                    cerr << "Customer '" << custId << "' not found" << endl;
                    continue;
                }
                // Validate movie
                else if (NULL == pMovie)
                {
                    cerr << "Movie with key '" << key << "' not found" << endl;
                    continue;
                }
                // Validate checkout
                else if (! toggleCheckoutState(('B' == c), pMovie, custId, format))
                {
                    continue;
                }
                else
                {
                    // Add transaction record to customer
                    Transaction* pTrans = new Transaction(pMovie, c, format);
                    pCust->addTransaction(pTrans);
                }

                break;

            // show movie inventory
            case 'S':
                m_movieManager.print();
                break;

            // Display transaction history by customer
            case 'H':
                cout << m_customers;
                break;

            default:
                cout << "Invalid transaction code " << c << endl;

                // Clean up
                infile.clear();
                infile.ignore(numeric_limits<streamsize>::max(), '\n');

                break;
        }
    }
}

//-----------------------------------------------------------------------------
// toggleCheckoutState()
// Borrows or returns a movie
// Returns true if transaction was successful, false otherwise
//-----------------------------------------------------------------------------
bool MovieStore::toggleCheckoutState(bool fBorrow, Movie* pMovie, 
    int custId, char format)
{
    if (fBorrow)
    {
        return pMovie->borrowMovie(custId, format);
    }
    else
    {
        return pMovie->returnMovie(custId, format);
    }

    return false;
}
