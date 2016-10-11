#include "InputAuxiliary.h"
using std::string;



bool InputAuxiliary::isBool(string input)
{
	if (input.size() != 1)
		return false;
	else if (input != "0" | input != "1")
		return false;
	else
		return true;

}
bool InputAuxiliary::isInteger (string input)
{
		char temp = input.at(0);
	if (input.at(0)!='-' && (input.at(0)<'0' || input.at(0)>'9')| input == "-")
	{

		return false;
	}

	for (int i=1; i<input.size(); i++)
	{
		if (input.at(i)<'0' ||  input.at(i)>'9')
			return false;
	}
	return true;

}
bool InputAuxiliary::isDouble (string input)
{
	if (input.at(0)!='-' && (input.at(0)<'0' || input.at(0)>'9')| input == "-")
		return false;
	else if (input.find('.') != input.rfind('.'))
		return false;
	for (int i=1; i<input.size(); i++)
	{
		if ((input.at(i)<'0' ||  input.at(i)>'9') && input.at(i) !='.')
			return false;
	}
	if (input.at(input.size()-1) == '.')
		return false;
	return true;

}
bool InputAuxiliary::CheckRange (int input, int lowbound, int highbound)
{
	if (input >= lowbound && input<= highbound)
		return true;
	return false;
	
}

bool InputAuxiliary::CheckRange (double input, double lowbound, double highbound)
{	
	if (input >= lowbound && input<= highbound)
		return true;
	return false;
}


bool InputAuxiliary::CheckLowBound (int input, int lowbound)
{
	if (input >= lowbound)
		return true;
	return false;
}
bool InputAuxiliary::CheckLowBound (double input, double lowbound)
{
	if (input >= lowbound)
		return true;
	return false;

}

bool InputAuxiliary::CheckHighBound (int input, int highbound)

{
	if (input <= highbound)
		return true;
	return false;

}
bool InputAuxiliary::CheckHighBound (double input, double highbound)

{
	if (input <= highbound)
		return true;
	return false;

}
int  InputAuxiliary::StringtoInt (string input)
{
	return atoi(input.c_str());

}
double InputAuxiliary::StringtoDouble (string input)
{
	return atof(input.c_str());
}