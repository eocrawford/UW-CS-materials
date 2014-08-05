#ifndef GRAPHL_H
#define GRAPHL_H

//-----------------------------------------------------------------------------
// graphl.h
// Partial graph class implemented with an adjacency list
// Author(s): Ethan Crawford
// Date: Oct 29, 2007
//-----------------------------------------------------------------------------
// Graph: stores a graph (nodes and the paths connecting them), 
// implemented as an adjacency list, an array of linked lists
//   Includes the following features:
//   -- Build a graph from an input file
//   -- Insert and remove edges
//   -- Perform a depth-first search on the graph
//   -- Display the graph
//
// Assumptions:
//   -- Input files are formatted correctly, as defined by the importer.
//-----------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include "nodedata.h"

using namespace std;

struct EdgeNode 
{
    EdgeNode() : 
        pNext(NULL),
        nGraphNodeSubscript(0)
    {
    }

    // subscript of the adjacent graph node
    int nGraphNodeSubscript;
    EdgeNode* pNext;
};

struct GraphNode
{
    GraphNode() : 
        pNodeData(NULL),
        pHead(NULL),
        fVisited(false)
    {
    }

    NodeData* pNodeData;
    EdgeNode* pHead;
    bool fVisited;
};

class GraphL
{
public:
    // Zero-parameter constructor
    GraphL();

    // destructor
    ~GraphL();

    // Builds the graph (stores edge costs) from an input file
    void buildGraph(ifstream& infile);

    // Display the graph
    void displayGraph();

    // Perform a depth-first search
    void depthFirstSearch();    

private:
    // Inserts an edge into the graph
    void insertEdge(int src, int dst);

    // Performs a recursive depth-first search
    void depthFirstSearchRecursive(int i);

    // Clear memory and reset variables
    void reset();
    
    GraphNode* m_rgNodes;
    int m_cNodes;
};

#endif // GRAPHL_H
