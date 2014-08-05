#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

// incomplete class and not documented
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Employee {
   friend ostream& operator<<(ostream &, const Employee &);

public:
   Employee(string = "dummyLast", string = "dummyFirst", int = 0, int = 0);
   void setNodeData(ifstream&);      
   bool operator<(const Employee&) const;      
   bool operator>(const Employee&) const;
   bool operator==(const Employee&) const;
   bool operator!=(const Employee&) const;

private:
   string lastName;
   string firstName;
   int IDnumber;
   int salary;
};

#endif

