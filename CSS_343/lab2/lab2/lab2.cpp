// I will test your BSTree class using either this main or
// a very similar one, and this buildTree.  
// You will want to do thorough testing on your own, 
// which is done by altering the data.

// presumably bstree.h includes nodedata.h so the include is not needed here
#include "bstree.h"
#include <fstream>
#include <iostream>
using namespace std;

void buildTree(BSTree&, ifstream&);         // prototype for buildTree
void initArray(NodeData* []); 

int main() {
   // create file object infile and open it
   // in unix the .. looks for a file up a directory and the .txt is not needed
   // for testing, call your data file something appropriate, e.g., data2.txt
   ifstream infile("../data2");
   if (!infile) {
      cerr << "File could not be opened." << endl;
      return 1;
   }

   // the NodeData class must have a constructor that takes a string
   NodeData notND("not");
   NodeData andND("and");
   NodeData sssND("sss");
   NodeData eND("e");
   NodeData mND("m");
   NodeData tND("t");
   NodeData pp("pp");

   BSTree T, T2, dup;
   NodeData* ndArray[100];
   initArray(ndArray);
   cout << "Initial data:" << endl << "  ";
   buildTree(T, infile);              // builds and displays initial data
   int ppDepth = T.getDepth(pp);
   cout << endl << "T: " << T << endl; 
   cout << endl << "depth of " << pp << ": " << ppDepth << endl; 
   cout << endl; 
   BSTree first(T);                   // test copy constructor
   dup = dup = T;                     // test operator=, self-assignment
   while(!infile.eof()) {
      cout << "Tree Inorder:" << endl << T;             // operator<< does endl

      // test retrieve 
      NodeData* const p1 = T.retrieve(andND);           // constant pointer
      cout << "Retrieve --> and:  " << (p1 == NULL ? "not found":"found") 
           << endl;
      NodeData* const p2 = T.retrieve(notND);
      cout << "Retrieve --> not:  " << (p2 == NULL ? "not found":"found") 
           << endl;
      NodeData* const p3 = T.retrieve(sssND);
      cout << "Retrieve --> sss:  " << (p3 == NULL ? "not found":"found") 
           << endl;

      // test getDepth 
      cout << "Depth    --> and:  " << T.getDepth(andND) << endl;
      cout << "Depth    --> not:  " << T.getDepth(notND) << endl;
      cout << "Depth    --> sss:  " << T.getDepth(sssND) << endl;

      // test ==, and != 
      T2 = T;
      cout << "T == T2?     " << (T == T2 ? "equal" : "not equal") << endl;
      cout << "T != first?  " << (T != first ? "not equal" : "equal") << endl;
      cout << "T == dup?    " << (T == dup ? "equal" : "not equal") << endl;
      dup = T;

      // somewhat test bstreeToArray and arrayToBSTree
      BSTree oneItem;
      oneItem.insert(new NodeData("foo"));
      oneItem.bstreeToArray(ndArray);
      oneItem.arrayToBSTree(ndArray);
      initArray(ndArray);             // empty out the array
      cout << "Tree Inorder:" << endl << oneItem;             // operator<< does endl

      BSTree empty;
      empty.bstreeToArray(ndArray);
      empty.arrayToBSTree(ndArray);
      initArray(ndArray);             // empty out the array
      cout << "Tree Inorder:" << endl << empty;             // operator<< does endl

      T.bstreeToArray(ndArray);
      T.arrayToBSTree(ndArray);
      cout << "Tree Inorder:" << endl << T;             // operator<< does endl

      T.makeEmpty();                  // empty out the tree
      initArray(ndArray);             // empty out the array

      cout << "---------------------------------------------------------------" 
           << endl;
      cout << "Initial data:" << endl << "  ";
      buildTree(T, infile);
      cout << endl; 
   }

   return 0;
}

//------------------------------- buildTree ----------------------------------
// you comment

// To build a tree, read strings from a line, terminating when "$$" is
// encountered. Since there is some work to do before the actual insert that is
// specific to the client problem, it's best that building a tree is not a 
// member function. It's a global function. 

void buildTree(BSTree& T, ifstream& infile) {
   string s;

   for (;;) {
      infile >> s;
      cout << s << ' ';
      if (s == "$$") break;                // at end of one line
      if (infile.eof()) break;             // no more lines of data
      NodeData* ptr = new NodeData(s);     // NodeData constructor takes string
      // would do a setData if there were more than a string

      bool success = T.insert(ptr);   
      if (!success)
         delete ptr;                       // duplicate case, not inserted 
   }
}

//------------------------------- initArray ----------------------------------
// initialize the array of NodeData* to NULL pointers

void initArray(NodeData* ndArray[]) {
   for(int i = 0; i < 100; i++)
      ndArray[i] = NULL;
}

