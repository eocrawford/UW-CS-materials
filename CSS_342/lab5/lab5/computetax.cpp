#include "computetax.h"

//-----------------------------------------------------------------------------
// ComputeTax constructor
//-----------------------------------------------------------------------------
ComputeTax::ComputeTax()
{
    m_type = "calc. tax";
    m_subscript = -1;
    m_account = 0;
    m_amount = 0;
	m_tax = .08; // Tax is valued at 8% or .08
}

//-----------------------------------------------------------------------------
// perform
// Perform transaction on provided account
//-----------------------------------------------------------------------------
bool ComputeTax::Perform(const BSTree* pTree)
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
            // Calculate and display the tax
            double balance = accounts[m_subscript].amount;

            cout << "Tax on " << accounts[m_subscript].friendlyName 
                << " for client " << m_account << " is $" 
                << (balance * m_tax) << endl << endl;

            m_detail = " for " + accounts[m_subscript].friendlyName;
            ret = true;
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// display
// Displays transaction output (successful transactions do not output anything)
// For simplicity, uses cout
//-----------------------------------------------------------------------------
void ComputeTax::Display() const
{
    cout << left << setw(10) << m_type << " " << right << setw(6) << " " 
        << m_detail << endl;
}

//-----------------------------------------------------------------------------
// setData
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool ComputeTax::SetData(ifstream& input)
{
    input >> m_account;
    m_subscript = m_account % 10;
    m_account /= 10;
    return input.good();
}
