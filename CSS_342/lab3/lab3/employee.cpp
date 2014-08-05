#include "employee.h"

// incomplete class and not fully documented

//--------------------------  constructor  -----------------------------------
Employee::Employee(string last, string first, int id, int sal) {
   lastName = last;
   firstName = first;
   IDnumber = id;
   salary = sal;
}  

//-------------------------  setNodeData  ------------------------------------
// set data from file
void Employee::setNodeData(ifstream& inFile) {
   inFile >> lastName >> firstName >> IDnumber >> salary;
}

//-------------------------------  <  ----------------------------------------
// < defined by value of lastName; if lastName equal, firstName is used
bool Employee::operator<(const Employee& E) const { 
   if (lastName < E.lastName) return true;
   if (lastName == E.lastName) return firstName < E.firstName;
   return false; 
}

//-------------------------------  >  ----------------------------------------
// > defined by value of lastName; if lastName equal, firstName is used
bool Employee::operator>(const Employee& E) const { 
   if (lastName > E.lastName) return true;
   if (lastName == E.lastName) return firstName > E.firstName;
   return false;
}

//----------------- operator == (equality) ----------------
// if all data members of calling and passed object are equal,
//   return true, otherwise false
//
bool Employee::operator==(const Employee& E) const {
   return (lastName == E.lastName && firstName == E.firstName);
}

//----------------- operator != (inequality) ----------------
// if all data members of calling and passed object are equal,
//   return false, otherwise true
//
bool Employee::operator!=(const Employee& E) const {
   return !(*this == E);
}

//-------------------------------  <<  ---------------------------------------
// display Employee object

ostream& operator<<(ostream& output, const Employee& E) { 
   output << E.IDnumber << "  " << E.salary << "  " << E.lastName 
          << " " << E.firstName << endl; 
   return output;
}

