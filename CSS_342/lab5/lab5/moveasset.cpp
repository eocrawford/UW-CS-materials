#include "moveasset.h"

//-----------------------------------------------------------------------------
// MoveAsset constructor
//-----------------------------------------------------------------------------
MoveAsset::MoveAsset()
{
    m_type = "move";
    m_subscriptFrom = -1;
    m_accountFrom = 0;
	m_subscriptTo = -1;
    m_accountTo = 0;
    m_amount = 0;
}

//-----------------------------------------------------------------------------
// perform
// Perform transaction on provided account
//-----------------------------------------------------------------------------
bool MoveAsset::Perform(const BSTree* pTree)
{
    bool ret = false;
    UserAccount target;
    UserAccount* pAccountFrom = NULL;
    UserAccount* pAccountTo = NULL;
    FIRMACCOUNT* accountsFrom = NULL;
    FIRMACCOUNT* accountsTo = NULL;
    int arraySizeFrom = 0;
    int arraySizeTo = 0;

    // Get the 'from' account
    target.SetAccount(m_accountFrom);
    pTree->Retrieve(target, pAccountFrom);

    // Get the 'to' account
    target.SetAccount(m_accountTo);
    pTree->Retrieve(target, pAccountTo);

    // Confirm that both accounts exist
    if (NULL == pAccountFrom || NULL == pAccountTo)
    {
        cout << "Unknown client ID " << m_accountFrom << " or " << m_accountTo 
            << " requested" << endl;
        return false;
    }

    // Get the FIRMACCOUNT arrays
    pAccountFrom->GetFirmAccounts(accountsFrom, arraySizeFrom);
    pAccountTo->GetFirmAccounts(accountsTo, arraySizeTo);

    // Confirm that the subscripts are valid
    if (m_subscriptFrom < arraySizeFrom && m_subscriptTo < arraySizeTo)
    {
        // Confirm that there are sufficient funds to transfer
        if (accountsFrom[m_subscriptFrom].amount >= m_amount)
        {
            // Remove the amount from the from and add it to the to
            accountsFrom[m_subscriptFrom].amount -= m_amount;
            accountsTo[m_subscriptTo].amount += m_amount;

            // Create the Display() strings
            m_detailFrom = " from " + 
                accountsFrom[m_subscriptFrom].friendlyName;

            m_detailTo = " to " + accountsTo[m_subscriptTo].friendlyName;

            // Convert int to string to create the cross-user transfer
            // text suffix.
            if (m_accountFrom != m_accountTo)
            {
                std::stringstream ss;
                std::string account;
                ss << m_accountTo;
                ss >> account;

                m_detailTo += " for client " + account;
            }

            ret = true;
        }
        else
        {
            cout << "Move not performed on " << 
                accountsFrom[m_subscriptFrom].friendlyName << " for client " 
                << m_accountFrom << ";" << endl << "insufficient funds" 
                << endl << endl;
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// display
// Displays transaction output (successful transactions do not output anything)
// For simplicity, uses cout
//-----------------------------------------------------------------------------
void MoveAsset::Display() const
{
    // Output the 'from' string
    cout << left << setw(10) << m_type << "$" << right << setw(6) << m_amount 
        << m_detailFrom << endl;

    // Output the 'to' string
    cout << left << setw(10) << " " << " " << right << setw(6) << " " 
        << m_detailTo << endl;
}

//-----------------------------------------------------------------------------
// setData
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool MoveAsset::SetData(ifstream& input)
{
    input >> m_accountFrom >> m_amount >> m_accountTo;

    m_subscriptFrom = m_accountFrom % 10;
    m_accountFrom /= 10;

    m_subscriptTo = m_accountTo % 10;
    m_accountTo /= 10;

    return input.good();
}
