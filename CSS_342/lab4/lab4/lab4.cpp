
#include "prefix.h"               // assumes prefix.h includes iostream
using namespace std;

int main()
{
   // declare file object and open the datafile 
//   ifstream infile("data4.txt"); 
   ifstream infile("../data4.txt");               // used electronically 
   if (!infile)  {
      cerr << "File could not be opened." << endl;
      return 1;
   }

   // for each prefix expression, evaluate, print answer, and print as postfix
   cout << "Prefix Expression = Answer" << endl;
   cout << "--------------------------" << endl;
   for (;;) {
      Prefix expr;
      expr.setPrefix(infile);
      if (infile.eof()) 
         break;
      cout << expr << " = " << expr.evaluatePrefix();
      cout << "       As postfix:  ";
      expr.outputAsPostfix(cout);
      cout << endl << endl;
   }

   return 0;
}

