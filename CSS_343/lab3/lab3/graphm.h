#ifndef GRAPHM_H
#define GRAPHM_H

//-----------------------------------------------------------------------------
// graphm.h
// Partial graph class implemented with an adjacency matrix
// Author(s): Ethan Crawford
// Date: Oct 29, 2007
//-----------------------------------------------------------------------------
// Graph: stores a graph (nodes and the paths connecting them), 
// implemented as an adjacency matrix, a two-dimensional array.
//   Includes the following features:
//   -- Build a graph from an input file
//   -- Insert and remove edges
//   -- Find the shortest path from one node to every other node
//   -- Display the graph
//
// Assumptions:
//   -- Input files are formatted correctly, as defined by the importer.
//-----------------------------------------------------------------------------

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include "nodedata.h"

using namespace std;
using std::setw;

const int MAXNODES = 100;
const int INF = MAXNODES + 1;

class GraphM
{
public:
    // Zero-parameter constructor
    GraphM();

    // Builds the graph (stores edge costs) from an input file
    void buildGraph(ifstream& infile);

    // Inserts an edge into the graph
    void insertEdge(int src, int dst, int cost);

    // Removes an edge from the graph
    void removeEdge(int src, int dst);

    // Uses Dijkstra's shortest path algorithm to find the shortest path 
    // between all nodes.
    void findShortestPath();

    // Displays all shortest paths
    void displayAll();

    // Displays the shortest path from srcNode to dstNode
    void display(int srcNode, int dstNode);

private:
    struct TableType
    {
        // Node has been visited
        bool visited;

        // shortest cost from source known so far
        int cost;

        // previous node in path of min dist
        int path;
    };

    // Initialize the source vertex in the shortest path array T
    void initSourceVertex(int v);

    // Find the shortest path to all other nodes from source node 'src'
    void findShortestPathFromSrc(int srcIndex);

    // Print the path from src to dst
    void printPathInfo(int srcIndex, int dstIndex, bool fPrintNames = false);

    // Recursively print the path from src to dst
    void printPostOrderPath(int srcIndex, int dstIndex, bool fPrintNames);

    // number of nodes in the graph
    int size;

    // data for graph nodes
    NodeData data[MAXNODES];

    // Cost array, the adjacency matrix
    int C[MAXNODES][MAXNODES];

    // T is used to keep the current shortest distance (and associated 
    // path info) known at any point in the algorithm.
    // stores visited, distance, path
    TableType T[MAXNODES][MAXNODES];
};

#endif // GRAPHM_H
