#include "comedy.h"

//-----------------------------------------------------------------------------
// display()
// Displays formatted info
//-----------------------------------------------------------------------------
void Comedy::display()
{
    string empty;
    displayFormatted(m_title, m_director, m_year, 0, empty);
}

//-----------------------------------------------------------------------------
// create()
// Creates a new instance of a NodeData object
//-----------------------------------------------------------------------------
NodeData* Comedy::create()
{
    return new Comedy();
}

//-----------------------------------------------------------------------------
// setKey()
// Sets a key to be used in a hashtable
//-----------------------------------------------------------------------------
void Comedy::setKey(const string& hashKey)
{
    stringstream ss;
    char buf[256];

    ss << hashKey;
    ss.get(buf, ARRAYSIZE(buf), ',');
    ss.get();
    ss.get();

    ss >> m_year;
    m_title = buf;

    m_hashKey = hashKey;
}

//-----------------------------------------------------------------------------
// setData(ifstream& input)
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Comedy::setData(ifstream& input)
{
    char buf[256];
    stringstream ss;

    // Director
    input.get(buf, ARRAYSIZE(buf), ',');
    m_director = buf;
    input.get();
    input.get();

    // Title
    input.get(buf, ARRAYSIZE(buf), ',');
    m_title = buf;
    input.get();

    // Year
    input >> m_year;

    // itoa
    ss << m_title << ", " << m_year;

    // Set hash key
    m_hashKey = ss.str();

    // Clean up
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');

    return input.good();
}

//-----------------------------------------------------------------------------
// operator<(const NodeData& b) const
// overloaded < operator
//-----------------------------------------------------------------------------
bool Comedy::operator<(const NodeData& b) const
{
    return (compare(*this, b) < 0);
}

//-----------------------------------------------------------------------------
// operator>(const NodeData& b) const
// overloaded > operator
//-----------------------------------------------------------------------------
bool Comedy::operator>(const NodeData& b) const
{
    return (compare(*this, b) > 0);
}

//-----------------------------------------------------------------------------
// operator==(const NodeData& b) const
// overloaded == operator
//-----------------------------------------------------------------------------
bool Comedy::operator==(const NodeData& b) const
{
    return (compare(*this, b) == 0);
}

//-----------------------------------------------------------------------------
// operator!=(const NodeData& b) const
// overloaded != operator
//-----------------------------------------------------------------------------
bool Comedy::operator!=(const NodeData& b) const
{
    return !(*this == b);
}

//-----------------------------------------------------------------------------
// compare(const NodeData& a, const NodeData& b) const
// compare method, used to implement comparison operators
//-----------------------------------------------------------------------------
int Comedy::compare(const NodeData& a, const NodeData& b) const
{
    int ret;
    const Comedy* pA = dynamic_cast<const Comedy*>(&a);
    const Comedy* pB = dynamic_cast<const Comedy*>(&b);

    ret = pA->m_title.compare(pB->m_title);

    if (0 == ret && pA->m_year != pB->m_year)
    {
        if (pA->m_year > pB->m_year)
        {
            ret = 1;
        }
        else if (pA->m_year < pB->m_year)
        {
            ret = -1;
        }
    }

    return ret;
}
