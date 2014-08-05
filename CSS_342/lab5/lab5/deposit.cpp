#include "deposit.h"

//-----------------------------------------------------------------------------
// Deposit constructor
//-----------------------------------------------------------------------------
Deposit::Deposit()
{
    m_type = "deposit";
    m_subscript = -1;
    m_account = 0;
    m_amount = 0;
}

//-----------------------------------------------------------------------------
// perform
// Perform transaction on provided account
//-----------------------------------------------------------------------------
bool Deposit::Perform(const BSTree* pTree)
{
    bool ret = false;
    UserAccount target;
    UserAccount* pAccount = NULL;

    // Find the target account.
    target.SetAccount(m_account);
    if (pTree->Retrieve(target, pAccount))
    {
        FIRMACCOUNT* accounts = NULL;
        int arraySize = 0;

        // Get the FIRMACCOUNTs array
        pAccount->GetFirmAccounts(accounts, arraySize);

        if (m_subscript < arraySize)
        {
            // Perform the deposit
            accounts[m_subscript].amount += m_amount;
            m_detail = " into " + accounts[m_subscript].friendlyName;
        }

        ret = true;
    }

    return ret;
}

//-----------------------------------------------------------------------------
// display
// Displays transaction output (successful transactions do not output anything)
// For simplicity, uses cout
//-----------------------------------------------------------------------------
void Deposit::Display() const
{
    cout << left << setw(10) << m_type << "$" << right << setw(6) << m_amount 
        << m_detail << endl;
}

//-----------------------------------------------------------------------------
// setData
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Deposit::SetData(ifstream& input)
{
    input >> m_account >> m_amount;
    m_subscript = m_account % 10;
    m_account /= 10;
    return input.good();
}
