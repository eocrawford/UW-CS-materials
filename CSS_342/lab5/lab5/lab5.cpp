#include "firm.h"

//---------------------------------------------------------------------------
// lab5.cpp
// Driver for financial firm project
// Author(s): Ethan Crawford
// Date: May 30, 2007
//---------------------------------------------------------------------------
// Calls methods on the Firm object
//
// Assumptions:
// none
//---------------------------------------------------------------------------

int main()
{
    Firm firm;

    // Open two text files, the list of accounts and the commands to be executed 
    ifstream accounts("../lab5data.txt");
    ifstream commands("../lab5command.txt");

    // Make sure the files are valid
    if (!accounts || !commands)
    {
        cerr << "File could not be opened." << endl;
        return 1;
    }

    // Initialize the accounts data
    firm.InitializeAccounts(accounts);

    // Process a list of transactions
    firm.ProcessTransactions(commands);

    // Display the processed transactions
    firm.DisplayTransactions();

    return 0;
}
