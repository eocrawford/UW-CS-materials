#include "displayhistory.h"

//-----------------------------------------------------------------------------
// DisplayHistory constructor
//-----------------------------------------------------------------------------
DisplayHistory::DisplayHistory()
{
    m_type = "history";
    m_amount = 0;
    m_account = 0;
}

//-----------------------------------------------------------------------------
// perform
// Perform transaction on provided account
//-----------------------------------------------------------------------------
bool DisplayHistory::Perform(const BSTree* pTree)
{
    bool ret = false;
    UserAccount target;
    UserAccount* pAccount = NULL;

    // Find the target account.
    target.SetAccount(m_account);
    if (pTree->Retrieve(target, pAccount))
    {
        // Display the transaction history for the account
        pAccount->DisplayTransactionHistory();
        ret = true;
    }

    return ret;
}

//-----------------------------------------------------------------------------
// display
// Displays transaction output (successful transactions do not output anything)
// For simplicity, uses cout
//-----------------------------------------------------------------------------
void DisplayHistory::Display() const
{
    cout << m_type << endl;
}

//-----------------------------------------------------------------------------
// setData
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool DisplayHistory::SetData(ifstream& input)
{
    input >> m_account;
    return input.good();
}
