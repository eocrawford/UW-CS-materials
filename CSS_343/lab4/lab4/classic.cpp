#include "classic.h"

//-----------------------------------------------------------------------------
// display()
// Displays formatted info
//-----------------------------------------------------------------------------
void Classic::display()
{
    displayFormatted(m_title, m_director, m_year, m_month, m_info);
}

//-----------------------------------------------------------------------------
// create()
// Creates a new instance of a NodeData object
//-----------------------------------------------------------------------------
NodeData* Classic::create()
{
    return new Classic();
}

//-----------------------------------------------------------------------------
// setKey()
// Sets a key to be used in a hashtable
//-----------------------------------------------------------------------------
void Classic::setKey(const string& hashKey)
{
    stringstream ss;
    string first, last;

    // Parse off month, year, and actor name from the hash key
    ss << hashKey;
    ss >> m_month >> m_year >> first >> last;

    m_info = first + ' ' + last;
    m_hashKey = hashKey;
}

//-----------------------------------------------------------------------------
// setData(ifstream& input)
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Classic::setData(ifstream& input)
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
    input.get();

    // First Name
    input.get(buf, ARRAYSIZE(buf), ' ');
    m_info = buf;
    input.get();
    m_info += ' ';

    // Last Name
    input.get(buf, ARRAYSIZE(buf), ' ');
    m_info += buf;

    // Month
    input >> m_month;

    // Year
    input >> m_year;

    // itoa
    ss << m_month << " " << m_year << " " << m_info;

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
bool Classic::operator<(const NodeData& b) const
{
    return (compare(*this, b) < 0);
}

//-----------------------------------------------------------------------------
// operator>(const NodeData& b) const
// overloaded > operator
//-----------------------------------------------------------------------------
bool Classic::operator>(const NodeData& b) const
{
    return (compare(*this, b) > 0);
}

//-----------------------------------------------------------------------------
// operator==(const NodeData& b) const
// overloaded == operator
//-----------------------------------------------------------------------------
bool Classic::operator==(const NodeData& b) const
{
    return (compare(*this, b) == 0);
}

//-----------------------------------------------------------------------------
// operator!=(const NodeData& b) const
// overloaded != operator
//-----------------------------------------------------------------------------
bool Classic::operator!=(const NodeData& b) const
{
    return !(*this == b);
}

//-----------------------------------------------------------------------------
// compare(const NodeData& a, const NodeData& b) const
// compare method, used to implement comparison operators
//-----------------------------------------------------------------------------
int Classic::compare(const NodeData& a, const NodeData& b) const
{
    int ret = 0;
    const Classic* pA = dynamic_cast<const Classic*>(&a);
    const Classic* pB = dynamic_cast<const Classic*>(&b);

    if (pA->m_year > pB->m_year)
    {
        ret = 1;
    }
    else if (pA->m_year < pB->m_year)
    {
        ret = -1;
    }
    else
    {
        if (pA->m_month > pB->m_month)
        {
            ret = 1;
        }
        else if (pA->m_month < pB->m_month)
        {
            ret = -1;
        }
        else
        {
            if (pA->m_info > pB->m_info)
            {
                ret = 1;
            }
            else if (pA->m_info < pB->m_info)
            {
                ret = -1;
            }
        }
    }

    return ret;
}
