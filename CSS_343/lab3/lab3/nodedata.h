#ifndef NODEDATA_H
#define NODEDATA_H
#include <string>
using namespace std;

class NodeData {
   friend ostream & operator<<(ostream &, NodeData &);

public:
   NodeData();          // default constructor, data is set to an empty string
   NodeData(const string &);      // data is set equal to parameter
   NodeData(const NodeData &);    // copy constructor

   string getData();
   void setData(string);
   const NodeData & operator=(const NodeData &);

   bool operator==(const NodeData &) const;
   bool operator!=(const NodeData &) const;
   bool operator<(const NodeData &) const;
   bool operator>(const NodeData &) const;
   bool operator<=(const NodeData &) const;
   bool operator>=(const NodeData &) const;

private:
   string data;          
};

#endif
