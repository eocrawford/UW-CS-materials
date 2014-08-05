#include "graphl.h"

//-----------------------------------------------------------------------------
// GraphL
// Zero-parameter constructor
//-----------------------------------------------------------------------------
GraphL::GraphL() :
    m_rgNodes(NULL),
    m_cNodes(0)
{
}

//-----------------------------------------------------------------------------
// destructor
// Display the graph
//-----------------------------------------------------------------------------
GraphL::~GraphL()
{
    reset();
}

//-----------------------------------------------------------------------------
// buildGraph 
// Builds the graph (stores edge costs) from an input file
//-----------------------------------------------------------------------------
void GraphL::buildGraph(ifstream& infile)
{
    reset();

    // Read the number of nodes from the first line
    infile >> m_cNodes;
    infile.clear();
    infile.ignore(numeric_limits<streamsize>::max(), '\n');

    // Allocate that many nodes
    m_rgNodes = new GraphNode[m_cNodes];

    if (NULL == m_rgNodes)
    {
        return;
    }

    // Store the friendly name of each node
    for (int i = 0; i < m_cNodes; i++)
    {
        string s;
        getline(infile, s);
        m_rgNodes[i].pNodeData = new NodeData(s);
    }

    // Store the edges in the adjacency list
    // Exit if x is zero
    while (!infile.bad() && !infile.eof())
    {
        int x = 0, y = 0;

        infile >> x >> y;

        if (0 == x)
        {
            break;
        }

        insertEdge(x, y);
    }
}

//-----------------------------------------------------------------------------
// insertEdge
// Inserts an edge into the graph
// An edge is a src int and a dest int
//-----------------------------------------------------------------------------
void GraphL::insertEdge(int src, int dst)
{
    if (src < 1 || dst < 1 || src > m_cNodes || dst > m_cNodes)
    {
        return;
    }

    // Allocate a new edge
    EdgeNode* pEdge = new EdgeNode();
    if (NULL == pEdge)
    {
        return;
    }

    // Assign the edge adjacent value
    pEdge->nGraphNodeSubscript = dst;

    // Add the edge to the head of the linked list
    pEdge->pNext = m_rgNodes[src - 1].pHead;
    m_rgNodes[src - 1].pHead = pEdge;
}

//-----------------------------------------------------------------------------
// displayGraph
// Display the graph
//-----------------------------------------------------------------------------
void GraphL::displayGraph()
{
    cout << "Graph:  " << endl;

    for (int i = 0; i < m_cNodes; i++)
    {
        EdgeNode* pCurrent = m_rgNodes[i].pHead;

        // Print the node friendly name
        cout << "Node " << (i + 1) << "       " << *m_rgNodes[i].pNodeData 
            << endl;

        // Print the adjacent edges
        while (NULL != pCurrent)
        {
            cout << "  edge " << (i + 1) << " " 
                << pCurrent->nGraphNodeSubscript << endl;
            pCurrent = pCurrent->pNext;
        }
    }
}

//-----------------------------------------------------------------------------
// depthFirstSearch
// Perform a depth-first search
//-----------------------------------------------------------------------------
void GraphL::depthFirstSearch()
{
    cout << "Depth-first ordering: ";

    // Set all nodes to unvisited
    for (int i = 0; i < m_cNodes; i++)
    {
        m_rgNodes[i].fVisited = false;
    }

    // Check every node for connections
    for (int i = 0; i < m_cNodes; i++)
    {
        if (!m_rgNodes[i].fVisited)
        {
            depthFirstSearchRecursive(i);
        }
    }

    cout << endl;
}

//-----------------------------------------------------------------------------
// depthFirstSearchRecursive
// Performs a recursive depth-first search
//-----------------------------------------------------------------------------
void GraphL::depthFirstSearchRecursive(int i)
{
    EdgeNode* pCurrent = m_rgNodes[i].pHead;

    if (m_rgNodes[i].fVisited)
    {
        return;
    }

    // Set the node as visited
    m_rgNodes[i].fVisited = true;

    // print the node value
    cout << (i + 1) << " ";

    // Visit each adjacent node in the linked list
    while (pCurrent != NULL)
    {
        int n = pCurrent->nGraphNodeSubscript - 1;

        if (! m_rgNodes[n].fVisited)
        {
            depthFirstSearchRecursive(n);
        }

        pCurrent = pCurrent->pNext;
    }
}

//-----------------------------------------------------------------------------
// reset
// Clear memory and reset variables
//-----------------------------------------------------------------------------
void GraphL::reset()
{
    if (NULL != m_rgNodes)
    {
        // Empty the adjacency list
        for (int i = 0; i < m_cNodes; i++)
        {
            EdgeNode* pCurrent = m_rgNodes[i].pHead;
            while (NULL != pCurrent)
            {
                EdgeNode* pTemp = pCurrent->pNext;
                delete pCurrent;
                pCurrent = pTemp;
            }

            delete m_rgNodes[i].pNodeData;
        }

        // Delete the nodes
        delete[] m_rgNodes;
        m_rgNodes = NULL;
        m_cNodes = 0;
    }
}
