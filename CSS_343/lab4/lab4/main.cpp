#include "moviestore.h"

int main()
{
    MovieStore movieStore;

    // Open three text files, the list of accounts, 
    // the list of movies, 
    // and the commands to be executed 
    ifstream accounts("../data4customers");
    ifstream movies("../data4movies");
    ifstream commands("../data4commands");

    // Make sure the files are valid
    if (!accounts || !commands || !movies)
    {
        cerr << "File could not be opened." << endl;
        return 1;
    }

    // import movies
    movieStore.initMovies(movies);

    // Import accounts
    movieStore.initAccounts(accounts);

    // Process a list of transactions
    movieStore.processTransactions(commands);

    return 0;
}
