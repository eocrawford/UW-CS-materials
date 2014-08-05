#include "withdraw.h"

//-----------------------------------------------------------------------------
// Withdraw constructor
//-----------------------------------------------------------------------------
Withdraw::Withdraw()
{
    m_type = "withdraw";
    m_subscript = -1;
    m_account = 0;
    m_amount = 0;
}

//-----------------------------------------------------------------------------
// perform
// Perform transaction on provided account
//-----------------------------------------------------------------------------
bool Withdraw::Perform(const BSTree* pTree)
{
    bool ret = false;
    UserAccount target;
    UserAccount* pAccount = NULL;

    target.SetAccount(m_account);

    if (pTree->Retrieve(target, pAccount))
    {
        FIRMACCOUNT* accounts = NULL;
        int arraySize = 0;

        pAccount->GetFirmAccounts(accounts, arraySize);

        if (m_subscript < arraySize)
        {
            int requiredAmount = m_amount;
            int availableAmount = 0;
            EAccountType type = accounts[m_subscript].type;

            // Check all related accounts for sufficient funds.
            for (int i = 0; i < arraySize; i++)
            {
                if (accounts[i].type == type)
                {
                    availableAmount += accounts[i].amount;

                    if (availableAmount >= requiredAmount)
                    {
                        break;
                    }
                }
            }

            // If required funds are present, complete the withdrawal.
            if (availableAmount >= requiredAmount)
            {
                // Withdraw as much as possible from the primary
                int withdrawAmount = ((accounts[m_subscript].amount >= 
                    requiredAmount) ? requiredAmount : 
                    accounts[m_subscript].amount);
                int i = 0;

                accounts[m_subscript].amount -= withdrawAmount;
                requiredAmount -= withdrawAmount;

                m_detail = " from " + accounts[m_subscript].friendlyName;

                // Overdraft protection - withdraw from each qualified account
                // until the required amount is 0.
                while (requiredAmount > 0 && i < arraySize)
                {
                    if (accounts[i].type == type)
                    {
                        // Do not withdraw any more than is present in each
                        // account.
                        withdrawAmount = (accounts[i].amount >= 
                            requiredAmount ? requiredAmount : 
                            accounts[i].amount);

                        accounts[i].amount -= withdrawAmount;
                        requiredAmount -= withdrawAmount;
                    }

                    i++;
                }

                ret = true;
            }
            // If not, throw an error.
            else
            {
                cout << "Withdrawal not performed on " << 
                    accounts[m_subscript].friendlyName << " for client " 
                    << m_account << ";" << endl << 
                    "insufficient funds" << endl << endl;
            }
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// display
// Displays transaction output (successful transactions do not output anything)
// For simplicity, uses cout
//-----------------------------------------------------------------------------
void Withdraw::Display() const
{
    cout << left << setw(10) << m_type << "$" << right << setw(6) << m_amount 
        << m_detail << endl;
}

//-----------------------------------------------------------------------------
// setData
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Withdraw::SetData(ifstream& input)
{
    input >> m_account >> m_amount;
    m_subscript = m_account % 10;
    m_account /= 10;
    return input.good();
}
