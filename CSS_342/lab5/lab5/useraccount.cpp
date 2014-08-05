#include "useraccount.h"

//-----------------------------------------------------------------------------
// overloaded << operator
// Output the account starting and ending balances
//-----------------------------------------------------------------------------
ostream& operator<<(ostream& output, const UserAccount& account)
{
    // Output account number, first name and last name
    output << account.m_account << "  " << account.m_firstName << " " << 
        account.m_lastName << endl;

    // Output initial account balances
    account.DisplayBalances(output, "Initial Balances: ", 
        account.m_initialBalances, ARRAYSIZE(account.m_initialBalances));

    // Output current account balances
    account.DisplayBalances(output, "Final Balances:   ", 
        account.m_currentBalances, ARRAYSIZE(account.m_currentBalances));

    output << endl;

    return output;
}

//-----------------------------------------------------------------------------
// UserAccount constructor
// Initialize default state
//-----------------------------------------------------------------------------
UserAccount::UserAccount()
{
    m_account = 0;
    InitializeFirmAccounts();
}

//-----------------------------------------------------------------------------
// UserAccount destructor
// Clean up dynamic memory
//-----------------------------------------------------------------------------
UserAccount::~UserAccount()
{
    // Empty the queue
    while (!m_transactionQueue.empty())
    {
        delete m_transactionQueue.front();
        m_transactionQueue.pop();
    }
}

//-----------------------------------------------------------------------------
// SetAccount
// Set account number. This is used by code that needs to create dummy
// user accounts for searching purposes in BSTree.Retrieve().
//-----------------------------------------------------------------------------
void UserAccount::SetAccount(int account)
{
    m_account = account;
}

//-----------------------------------------------------------------------------
// SetData
// Set internal state based on a single line in an input file
//-----------------------------------------------------------------------------
void UserAccount::SetData(ifstream& infile)
{
    infile >> m_lastName >> m_firstName >> m_account;
    
    for (int i = 0; i < ARRAYSIZE(m_initialBalances); i++)
    {
        infile >> m_initialBalances[i].amount;
        m_currentBalances[i].amount = m_initialBalances[i].amount;
    }
}

//-----------------------------------------------------------------------------
// DisplayTransactionHistory
// Print the transaction queue
//-----------------------------------------------------------------------------
void UserAccount::DisplayTransactionHistory()
{
    cout << endl << "History of transactions for client " << m_firstName << 
        " " << m_lastName << ", client ID = " << m_account << endl;
    cout << "Type      Amount  Detail" << endl;
    cout << "--------- ------- --------------------------------------" << endl;

    if (! m_transactionQueue.empty())
    {
        Transaction* pTransFront = m_transactionQueue.front();
        Transaction* pTransCurrent = pTransFront;

        // Print every element in the queue, rebuilding it in-place
        // to make the action non-destructive.
        do
        {
            pTransCurrent->Display();
            m_transactionQueue.push(pTransCurrent);
            m_transactionQueue.pop();
            pTransCurrent = m_transactionQueue.front();
        }
        while (pTransCurrent != pTransFront);
    }
}

//-----------------------------------------------------------------------------
// InitializeFirmAccounts
// Set default values for firm accounts
//-----------------------------------------------------------------------------
void UserAccount::InitializeFirmAccounts()
{
    m_initialBalances[0].friendlyName = "Money Market fund";
    m_initialBalances[0].type = EAccountTypeMoneyMarket;
    m_initialBalances[0].amount = 0;
    m_currentBalances[0] = m_initialBalances[0];

    m_initialBalances[1].friendlyName = "Prime Money Market fund";
    m_initialBalances[1].type = EAccountTypeMoneyMarket;
    m_initialBalances[1].amount = 0;
    m_currentBalances[1] = m_initialBalances[1];

    m_initialBalances[2].friendlyName = "Long-Term Bond fund";
    m_initialBalances[2].type = EAccountTypeBond;
    m_initialBalances[2].amount = 0;
    m_currentBalances[2] = m_initialBalances[2];

    m_initialBalances[3].friendlyName = "Short-Term Bond fund";
    m_initialBalances[3].type = EAccountTypeBond;
    m_initialBalances[3].amount = 0;
    m_currentBalances[3] = m_initialBalances[3];

    m_initialBalances[4].friendlyName = "500 Index fund";
    m_initialBalances[4].type = EAccountTypeMutualFund;
    m_initialBalances[4].amount = 0;
    m_currentBalances[4] = m_initialBalances[4];

    m_initialBalances[5].friendlyName = "Capital Value fund";
    m_initialBalances[5].type = EAccountTypeMutualFund;
    m_initialBalances[5].amount = 0;
    m_currentBalances[5] = m_initialBalances[5];

    m_initialBalances[6].friendlyName = "Growth Equity fund";
    m_initialBalances[6].type = EAccountTypeMutualFund;
    m_initialBalances[6].amount = 0;
    m_currentBalances[6] = m_initialBalances[6];

    m_initialBalances[7].friendlyName = "Growth Index fund";
    m_initialBalances[7].type = EAccountTypeMutualFund;
    m_initialBalances[7].amount = 0;
    m_currentBalances[7] = m_initialBalances[7];

    m_initialBalances[8].friendlyName = "Value fund";
    m_initialBalances[8].type = EAccountTypeMutualFund;
    m_initialBalances[8].amount = 0;
    m_currentBalances[8] = m_initialBalances[8];

    m_initialBalances[9].friendlyName = "Value Stock Index fund";
    m_initialBalances[9].type = EAccountTypeMutualFund;
    m_initialBalances[9].amount = 0;
    m_currentBalances[9] = m_initialBalances[9];
}

//-----------------------------------------------------------------------------
// AddTransaction
// Add a transaction to the transaction queue
//-----------------------------------------------------------------------------
void UserAccount::AddTransaction(Transaction* pTransaction)
{
    if (pTransaction != NULL)
    {
        m_transactionQueue.push(pTransaction);
    }
}

//-----------------------------------------------------------------------------
// DisplayBalances
// Display a formatted list of account balances
//-----------------------------------------------------------------------------
void UserAccount::DisplayBalances(ostream& output, const char* text, 
    const FIRMACCOUNT accounts[], int arraySize) const
{
    output << text;
    for (int i = 0; i < arraySize; i++)
    {
        output << setw(6) << accounts[i].amount;
    }
    output << endl;
}

//-----------------------------------------------------------------------------
// GetFirmAccounts
// Get the FIRMACCOUNT struct array
//-----------------------------------------------------------------------------
void UserAccount::GetFirmAccounts(FIRMACCOUNT*& accounts, int& arraySize)
{
    accounts = m_currentBalances;
    arraySize = ARRAYSIZE(m_currentBalances);
}

//-----------------------------------------------------------------------------
// GetAccount
// Get account number
//-----------------------------------------------------------------------------
int UserAccount::GetAccount() const
{
    return m_account;
}
