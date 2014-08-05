#include "drama.h"

//-----------------------------------------------------------------------------
// display()
// Displays formatted info
//-----------------------------------------------------------------------------
void Drama::display()
{
    string empty;
    displayFormatted(m_title, m_director, m_year, 0, empty);
}

//-----------------------------------------------------------------------------
// create()
// Creates a new instance of a NodeData object
//-----------------------------------------------------------------------------
NodeData* Drama::create()
{
    return new Drama();
}

//-----------------------------------------------------------------------------
// setKey()
// Sets a key to be used in a hashtable
//-----------------------------------------------------------------------------
void Drama::setKey(const string& hashKey)
{
    stringstream ss;
    char buf[256];

    ss << hashKey;
    ss.get(buf, ARRAYSIZE(buf), ',');
    ss.get();
    ss.get();

    m_director = buf;

    ss.get(buf, ARRAYSIZE(buf), ',');
    m_title = buf;

    m_hashKey = hashKey;
}

//-----------------------------------------------------------------------------
// setData(ifstream& input)
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Drama::setData(ifstream& input)
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
    ss << m_year;

    // Set hash key
    m_hashKey = m_director + ", " + m_title + ",";

    // Clean up
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');

    return input.good();
}

//-----------------------------------------------------------------------------
// operator<(const NodeData& b) const
// overloaded < operator
//-----------------------------------------------------------------------------
bool Drama::operator<(const NodeData& b) const
{
    return (compare(*this, b) < 0);
}

//-----------------------------------------------------------------------------
// operator>(const NodeData& b) const
// overloaded > operator
//-----------------------------------------------------------------------------
bool Drama::operator>(const NodeData& b) const
{
    return (compare(*this, b) > 0);
}

//-----------------------------------------------------------------------------
// operator==(const NodeData& b) const
// overloaded == operator
//-----------------------------------------------------------------------------
bool Drama::operator==(const NodeData& b) const
{
    return (compare(*this, b) == 0);
}

//-----------------------------------------------------------------------------
// operator!=(const NodeData& b) const
// overloaded != operator
//-----------------------------------------------------------------------------
bool Drama::operator!=(const NodeData& b) const
{
    return !(*this == b);
}

//-----------------------------------------------------------------------------
// compare(const NodeData& a, const NodeData& b) const
// compare method, used to implement comparison operators
//-----------------------------------------------------------------------------
int Drama::compare(const NodeData& a, const NodeData& b) const
{
    int ret = 0;
    const Drama* pA = dynamic_cast<const Drama*>(&a);
    const Drama* pB = dynamic_cast<const Drama*>(&b);

    if (pA->m_director > pB->m_director)
    {
        ret = 1;
    }
    else if (pA->m_director < pB->m_director)
    {
        ret = -1;
    }
    else
    {
        if (pA->m_title > pB->m_title)
        {
            ret = 1;
        }
        else if (pA->m_title < pB->m_title)
        {
            ret = -1;
        }
    }

    return ret;
}
