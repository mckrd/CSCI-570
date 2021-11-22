#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <ctype.h>

using namespace std;

bool validateLength(string finalString, string baseString, int n)
{
	int validLength = pow(2,n) * baseString.length(); 
	return (finalString.length() == validLength);
}

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;

}

int main()
{

	string baseString1 = "", baseString2 = ""; // The base string in the input file
	string finalString1 = "", finalString2 = ""; // The final strings generated from the base strings
	
	int j = 0, k = 0; 
	int counter = 0;
	
	fstream myFile;
	myFile.open("input.txt", ios::in); // Read
	
	if ( myFile.is_open())
	{
		string line;
		while ( getline( myFile, line))
		{
			if ( counter == 0)
			{
				baseString1 += line;
				finalString1 += line;
				counter++;
			}
			
			else if ( counter == 1)
			{
				if ( isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					finalString1.insert(indexToAppend,finalString1);
					j++;
				}
				else counter++;
			}

			if ( counter == 2)
			{
				baseString2 += line;
				finalString2 += line;
				counter++;
			}

			else if (counter == 3)
			{
				if ( isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					finalString2.insert(indexToAppend,finalString2);
					k++;
				}
			}
		}
		myFile.close();
	}

	validateLength(finalString1, baseString1, j); // Validate 1st generated string is of length (2^j)*str1.length 
	validateLength(finalString2, baseString2, k); // Validate 2nd generated string is of length (2^k)*str2.length

	// The description asks us to validate the length of the generated strings. Just in case, the function returns false, what do we do?
	/*
	cout<<"base string 1 is: "<<baseString1<<"\n";
	cout<<"j is: "<<j<<"\n";
	cout<<"final generated string 1 is: "<<finalString1<<"\n";
	cout<<"base string 2 is: "<<baseString2<<"\n";
	cout<<"k is: "<<k<<"\n";
	cout<<"final generated string 2 is: "<<finalString2<<"\n";
	*/
	


	return 0;
}