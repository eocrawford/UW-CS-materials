#include "transaction.h"

//-----------------------------------------------------------------------------
// constructor
// Creates a new transaction record
//-----------------------------------------------------------------------------
Transaction::Transaction(Movie* pMovie, char type, char format)
{
    m_pMovie = pMovie;
    m_type = type;
    m_format = format;
}

//-----------------------------------------------------------------------------
// display()
// Displays a formatted transaction record
//-----------------------------------------------------------------------------
void Transaction::display()
{
    m_pMovie->display();
    cout << (m_format == 'D' ? "DVD " : "VHS ") 
        << (m_type == 'B' ? "Borrow" : "Return") << endl;
}
