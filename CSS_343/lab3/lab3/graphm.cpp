#include "graphm.h"

//-----------------------------------------------------------------------------
// GraphM
// Zero-parameter constructor
//-----------------------------------------------------------------------------
GraphM::GraphM()
{
    size = 0;

    // Initialize the graph
    for (int i = 0; i < MAXNODES; i++)
    {
        for (int j = 0; j < MAXNODES; j++)
        {
            C[i][j] = INF;
        }
    }
}

//-----------------------------------------------------------------------------
// findShortestPath
// Uses Dijkstra's shortest path algorithm to find the shortest path 
// between all nodes.
//-----------------------------------------------------------------------------
void GraphM::findShortestPath()
{
    // Find the shortest path from each node to every other node
    for (int i = 0; i < size; i++)
    {
        findShortestPathFromSrc(i);
    }
}

//-----------------------------------------------------------------------------
// findShortestPathFromSrc
// Find the shortest path to all other nodes from source node 'src'
//-----------------------------------------------------------------------------
void GraphM::findShortestPathFromSrc(int srcIndex)
{
    int v = -1;

    // Initialize the shortest path table for the source vertex
    initSourceVertex(srcIndex);

    while (true)
    {
        int minCost = INF;

        // Use source node graph data to calculate the minimum unvisited 
        // vertex.
        for (int i = 0; i < size; i++)
        {
            if (!T[srcIndex][i].visited && (minCost > T[srcIndex][i].cost))
            {
                minCost = T[srcIndex][i].cost;
                v = i;
            }
        }

        // If none could be found, exit the loop.
        if (minCost == INF)
        {
            break;
        }

        // Calculate the least cost for each path connected to the selected
        // vertex
        for (int w = 0; w < size; w++)
        {
            int currentCost = T[srcIndex][w].cost;
            int newCost = T[srcIndex][v].cost + C[v][w];

            // If a path between the nodes exists and a better path exists
            // than what was previously known, update the stored values
            if ((C[v][w] < INF) && (currentCost > newCost))
            {
                T[srcIndex][w].cost = newCost;

                // Save the path for path-finding purposes
                T[srcIndex][w].path = v;
            }
        }

        // Tag the current vertex as visited
        T[srcIndex][v].visited = true;
    }
}

//-----------------------------------------------------------------------------
// displayAll
// Displays all shortest paths
//-----------------------------------------------------------------------------
void GraphM::displayAll()
{
    cout << "Description               From node  To node  Dijkstra's  Path" 
         << endl;

    // Display each source vertex
    for (int v = 0; v < size; v++)
    {
        // Print the node friendly name
        cout << data[v] << endl;

        // Display each path in each source vertex
        for (int w = 0; w < size; w++)
        {
            if (v == w)
            {
                continue;
            }

            // print leading spaces
            cout << "                            ";

            printPathInfo(v, w);
        }
    }
}

//-----------------------------------------------------------------------------
// display
// Displays the shortest path from srcNode to dstNode
//-----------------------------------------------------------------------------
void GraphM::display(int srcNode, int dstNode)
{
    if (srcNode < 1 || dstNode < 1 || srcNode > size || dstNode > size)
    {
        return;
    }
    
    printPathInfo(srcNode - 1, dstNode - 1, true);
}

//-----------------------------------------------------------------------------
// printPathInfo
// Print the path from src to dst
//-----------------------------------------------------------------------------
void GraphM::printPathInfo(int srcIndex, int dstIndex, bool fPrintNames)
{
    // print 'From'
    cout << "   " << (srcIndex + 1);

    // print 'To'
    cout << "       " << (dstIndex + 1);

    // Print dashs to represent 'no path'
    if (T[srcIndex][dstIndex].cost == INF)
    {
        cout << right << setw(9) << "---";
    }
    else
    {
        // print the cost
        cout << right << setw(8) << T[srcIndex][dstIndex].cost << "         ";

        printPostOrderPath(srcIndex, dstIndex, false);

        if (fPrintNames)
        {
            cout << endl;
            printPostOrderPath(srcIndex, dstIndex, fPrintNames);
        }
    }

    cout << endl;
}

//-----------------------------------------------------------------------------
// printPostOrderPath
// Recursively print the path from src to dst, either as integer values or
// text names
//-----------------------------------------------------------------------------
void GraphM::printPostOrderPath(int srcIndex, int dstIndex, bool fPrintNames)
{
    // Base case - node is asking for the path to itself
    if (srcIndex == dstIndex)
    {
        if (! fPrintNames)
        {
            cout << " " << (srcIndex + 1);
        }
        else
        {
            cout << data[srcIndex] << endl;
        }

        return;
    }

    printPostOrderPath(srcIndex, T[srcIndex][dstIndex].path, fPrintNames);

    if (! fPrintNames)
    {
        cout << " " << (dstIndex + 1);
    }
    else
    {
        cout << data[dstIndex] << endl;
    }
}


//-----------------------------------------------------------------------------
// init
// Initialize the source vertex in the shortest path array T
//-----------------------------------------------------------------------------
void GraphM::initSourceVertex(int v)
{
    // Initialize the srcIndex row to defaults
    for (int w = 0; w < size; w++)
    {
        T[v][w].cost = INF;
        T[v][w].path = 0;
        T[v][w].visited = false;
    }

    // All nodes have infinite cost except the src node, which has 0 cost
    T[v][v].cost = 0;
}

//-----------------------------------------------------------------------------
// buildGraph 
// Builds the graph (stores edge costs) from an input file
//-----------------------------------------------------------------------------
void GraphM::buildGraph(ifstream& infile)
{
    // Read the number of nodes from the first line
    infile >> size;
    infile.clear();
    infile.ignore(numeric_limits<streamsize>::max(), '\n');

    // Store the friendly name of the node
    for (int i = 0; i < size; i++)
    {
        string s;
        getline(infile, s);
        data[i] = s;
    }

    // Store the node path info in the adjacency matrix
    // Exit if x is zero
    while (!infile.bad() && !infile.eof())
    {
        int x = 0, y = 0, cost = INF;

        infile >> x >> y >> cost;

        if (0 == x)
        {
            break;
        }

        insertEdge(x, y, cost);
    }
}

//-----------------------------------------------------------------------------
// insertEdge
// Inserts an edge into the graph
// An edge is a source int, a dest int, and a cost
//-----------------------------------------------------------------------------
void GraphM::insertEdge(int src, int dst, int cost)
{
    if (src < 1 || dst < 1 || src > MAXNODES || dst > MAXNODES)
    {
        return;
    }

    // Override any provided cost, if the node is itself
    if (src == dst)
    {
        cost = 0;
    }

    C[src - 1][dst - 1] = cost;

    // Update the size of the array to reflect the new value. This is
    // necessary because of the fixed size of the internal data structure.
    size = max(max(src, dst), size);

    // Recalculate the shortest paths
    findShortestPath();
}

//-----------------------------------------------------------------------------
// removeEdge
// Removes an edge from the graph
//-----------------------------------------------------------------------------
void GraphM::removeEdge(int src, int dst)
{
    if (src < 1 || dst < 1 || src > MAXNODES || dst > MAXNODES)
    {
        return;
    }

    // Remove the edge by setting the cost to infinity
    C[src - 1][dst - 1] = INF;

    // Recalculate the shortest paths
    findShortestPath();
}
