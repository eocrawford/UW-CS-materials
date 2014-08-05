//////////////////////////////  nodedata.cpp  ////////////////////////////////

#include "nodedata.h"

//--------------------------  constructor  -----------------------------------
NodeData::NodeData(int n, char c)  { num = n; ch = c; }  

//-------------------------  setNodeData  ------------------------------------
// set data by prompting user
void NodeData::setNodeData() { 
   cout << "Please enter an integer: ";   cin >> num;
   cout << "Please enter a character: ";  cin >> ch; 
}

//-------------------------  setNodeData  ------------------------------------
// set data from file
void NodeData::setNodeData(ifstream& infile)  { infile >> num >> ch; }

//-------------------------------  <  ----------------------------------------
// < defined by value of num; if nums equal, ch is used
bool NodeData::operator<(const NodeData& N) const { 
   if (num < N.num) return true;
   if (num == N.num) return ch < N.ch;
   return false; 
}

//-------------------------------  >  ----------------------------------------
// > defined by value of num; if nums equal, ch is used
bool NodeData::operator>(const NodeData& N) const { 
   if (num > N.num) return true;
   if (num == N.num) return ch > N.ch;
   return false;
}

//-------------------------------  ==  ---------------------------------------
// > defined by value of num; if nums equal, ch is used
bool NodeData::operator==(const NodeData& N) const { 
   return num == N.num && ch == N.ch;
}

//-------------------------------  !=  ---------------------------------------
// > defined by value of num; if nums equal, ch is used
bool NodeData::operator!=(const NodeData& N) const { 
   return !(*this == N);
}

//-------------------------------  <<  ---------------------------------------
// display NodeData object
ostream& operator<<(ostream& output, const NodeData& N) { 
   output << N.num << ' ' << N.ch << endl;  
   return output;
}

