//---------------------------------------------------------------------------
// lab3.cpp
//---------------------------------------------------------------------------
// This code tests all of the basic functionality of the classes to
// perform Dijkstra's algorihms and depth-first search for CSS 343 Lab 3.
// It is not meant to exhaustively test the class.
//
// Assumptions:
//   -- students can follow the directions to interface with this file
//   -- text files "../data31" and "../data32" are formatted as described 
//      (you can use "data31.txt" and "data32.txt" to test)
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "graphl.h"
#include "graphm.h"
using namespace std;

//-------------------------- main -------------------------------------------
int main() {
   // part 1
   //ifstream infile1("data31.txt");
   ifstream infile1("../data31");
   if (!infile1) {
      cerr << "File could not be opened." << endl;
      return 1;
   }

   //for each graph, find the shortest path from every node to all other nodes
   for(;;){
      GraphM G;
      G.buildGraph(infile1);
      if (infile1.eof()) 
         break;
      G.findShortestPath();        
      G.displayAll();              // display shortest distance, path to cout
      G.display(3,1);              // display path from node 3 to 1 to cout
      G.display(1,4);              // display path from node 1 to 4 to cout
      G.display(3,3);              // display path from node 3 to 3 to cout

      // may be other calls to display 
   }

   GraphM emptyM;
   emptyM.findShortestPath();
   emptyM.display(4, 7);
   emptyM.displayAll();
   emptyM.insertEdge(3, 3, 69);
   emptyM.insertEdge(1, 3, 12);
   emptyM.insertEdge(1, 2, 18);
   emptyM.insertEdge(1, 4, 33);
   emptyM.insertEdge(3, 4, 8);
   emptyM.insertEdge(4, 5, 17);
   emptyM.display(1, 5);
   emptyM.displayAll();
   emptyM.removeEdge(4, 5);
   emptyM.display(1, 5);
   emptyM.displayAll();

   // part 2 
   //ifstream infile2("data32.txt");
   ifstream infile2("../data32");
   if (!infile2) {
      cerr << "File could not be opened." << endl;
      return 1;
   }

   //for each graph, find the depth-first search ordering
   for (;;) {
      GraphL G;
      G.buildGraph(infile2);
      if (infile2.eof()) 
         break;
      G.displayGraph();
      G.depthFirstSearch();    // find and display depth-first ordering to cout
   }

   GraphL emptyL;
   emptyL.depthFirstSearch();
   emptyL.displayGraph();
 
   cout << endl;
   return 0;
}

