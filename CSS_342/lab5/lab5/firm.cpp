#include "firm.h"

//-----------------------------------------------------------------------------
// Firm constructor
// Initialize default firm state
//-----------------------------------------------------------------------------
Firm::Firm()
{
    m_pAccountRoot = new BSTree();
}

//-----------------------------------------------------------------------------
// Firm destructor
// Print the day's firm report and clean up memory
//-----------------------------------------------------------------------------
Firm::~Firm()
{
    delete m_pAccountRoot;
}

//-----------------------------------------------------------------------------
// DisplayTransactions
// Display all the queued transations
//-----------------------------------------------------------------------------
void Firm::DisplayTransactions() const
{
    m_pAccountRoot->Display();
}

//-----------------------------------------------------------------------------
// Initialize
// Initialize firm accounts from an input file
//-----------------------------------------------------------------------------
void Firm::InitializeAccounts(ifstream& infile) const
{
    m_pAccountRoot->BuildTree(infile);
}

//-----------------------------------------------------------------------------
// ProcessTransactions
// Process a list of transactions from an input file
//-----------------------------------------------------------------------------
void Firm::ProcessTransactions(ifstream& infile) const
{
    while (true)
    {
        // If the infile is not in a good state or 
        // is at eof, exit the loop
        if (!infile.good() || infile.eof())
        {
            break;
        }

        ProcessTransaction(infile);
    }
}

//-----------------------------------------------------------------------------
// ProcessTransaction
// Process a single transaction from an input file
//-----------------------------------------------------------------------------
void Firm::ProcessTransaction(ifstream& infile) const
{
    UserAccount target;
    UserAccount* pAccount = NULL;
    Transaction* pTransaction = NULL;
    char transactionCode = '\0';
    int accountNumber = 0;
    bool isHistory = false;

    // Get the transaction code
    infile >> transactionCode;

    // Read, but do not extract, the account number from the input stream.
    // This permits us to validate the existence of the account number
    // and emit the appropriate error before passing the transaction
    // to the string.
    streampos before = infile.tellg();
    infile >> accountNumber;
    infile.seekg(before);

    // History is a special case. If the user wants history,
    // do not calculate the actual account number, but instead
    // set a flag for later special-casing.
    if (transactionCode == 'H')
    {
        isHistory = true;
    }
    else
    {
        accountNumber /= 10;
    }

    // Get a transaction object
    if (!GetTransaction(transactionCode, pTransaction))
    {
        // The transaction code is invalid, so skip the transaction.
        cout << "Unknown transaction type '" << transactionCode << 
            "' requested" << endl;
        infile.clear();
        infile.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Set transaction parameters
    if (pTransaction && !pTransaction->SetData(infile))
    {
        if (!infile.eof())
        {
            // Invalid transaction parameters, so skip the transaction.
            infile.clear();
            infile.ignore(numeric_limits<streamsize>::max(), '\n');

            // Clean up the invalid transaction object
            delete pTransaction;
            pTransaction = NULL;
        }
    }

    // Find the account in the tree
    target.SetAccount(accountNumber);
    if (!m_pAccountRoot->Retrieve(target, pAccount))
    {
        // The user account on this line can't be identified, so skip the
        // transaction
        cout << "Unknown client ID " << accountNumber << " requested" << endl;
        infile.clear();
        infile.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Valid account and valid transaction; execute the transaction.
    if (NULL != pAccount && NULL != pTransaction)
    {
        // If history is called, add it unconditionally to the
        // list before performing the transaction, to ensure that
        // the transaction shows up in the history list.
        if (isHistory)
        {
            // Add the transaction unconditionally to the list.
            pAccount->AddTransaction(pTransaction);

            // Execute the transaction, passing in the account tree
            pTransaction->Perform(m_pAccountRoot);
        }
        else
        {
            // Execute the transaction, passing in the account tree
            bool ret = pTransaction->Perform(m_pAccountRoot);

            // If the transaction was successful, store it in the account 
            // queue. Otherwise, delete the transaction and don't store it.
            if (ret)
            {
                pAccount->AddTransaction(pTransaction);
            }
            else
            {
                delete pTransaction;
                pTransaction = NULL;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// GetTransaction
// Get the Transaction object that corresponds to the provided transaction
// code. Returns true if the transaction can be found, false otherwise.
//-----------------------------------------------------------------------------
bool Firm::GetTransaction(char transactionCode, Transaction*& pTransaction) const
{
    Transaction* pTrans = NULL;

    switch (transactionCode)
    {
        case 'D':
            pTrans = new Deposit();
            break;

        case 'H':
            pTrans = new DisplayHistory();
            break;

        case 'W':
            pTrans = new Withdraw();
            break;

        case 'T':
            pTrans = new ComputeTax();
            break;

        case 'M':
            pTrans = new MoveAsset();
            break;

        default:
            break;
    }

    // Set the out pointer
    pTransaction = pTrans;

    // If a new object was created, the function succeeded.
    return (NULL != pTrans);
}
