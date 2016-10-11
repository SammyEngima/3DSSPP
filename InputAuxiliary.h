#ifndef INPUTAUXILIARY_H
#define INPUTAUXILIARY_H
#include <string>

using std::string;
// File created by Kevin Shi
//	3/7/13

// The "INPUTAUXILIARY" class is used as a function object.
//
//

// The purpose of the object is to check whether the string meet the requirement.
// Also it contains function to convert string into other type. 

class InputAuxiliary
{
public:

   bool isBool(string input);
   bool isInteger (string input);
   bool isDouble (string input);
   bool CheckRange (double input, double lowbound, double highbound); //contain the boundary
   bool CheckRange (int input, int lowbound, int highbound);	  //contain the boundary
   bool CheckLowBound (double input, double lowbound);				 //contain the boundary
   bool CheckLowBound (int input, int lowbound);					 //contain the boundary
   bool CheckHighBound (double input, double highbound);			 //contain the boundary
   bool CheckHighBound (int input, int highbound);				 //contain the boundary
   int  StringtoInt (string input);
   double StringtoDouble (string input);

};

#endif