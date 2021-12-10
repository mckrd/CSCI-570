#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <ctype.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int validateLength(string finalString, string baseString, int n)
{
	int validLength = pow(2,n) * baseString.length(); 
	if ( finalString.length() == validLength)
		return validLength;
	else return -1;
}

/*
bool isNumber(string s)
{	
    for (int i = 0; i < s.length() && i != '\n'; i++)
    {
    	cout<<"returning true for "<<s[i]<<"\n";
        if (isdigit(s[i]) == false)
        {
        	cout<<"returning false for "<<s[i]<<"\n";
            return false;
        }
    }
    	        
    return true;

}
*/
bool isNumber(string s)
{
	//cout<<"s is:"<<s<<":\n";
	if( isdigit(s[0]))
	{
		//cout<<"returning true for "<<s[0]<<"\n";
		return true;
	}
	else return false;
}
/*
bool isNumber(string s)
{	
    for (int i = 0; i < s.length() && i != '\n'; i++)
    {
    	cout<<"returning true for "<<s[i]<<"\n";
        if (isdigit(s[i]) == true)
        {
        	return true;
        }
    }
    return false;
}
*/
const size_t letters = 26;

int alignSequence(string finalString1, string finalString2, int gap_penalty, 
		int mismatchPenalty[letters][letters], string &alignedString1, string &alignedString2)
{
    int n = finalString1.size();
    int m = finalString2.size();

    vector<vector<int> > OPT(n+1, vector<int>(m+1));

    int i, j;

    for (i = 0; i <= m; i++)
        OPT[0][i] = gap_penalty*i;
    for (i = 0; i <= n; i++)
        OPT[i][0] = gap_penalty*i;

    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= m; j++)
        {
            char x_i = finalString1[i-1];
            char y_j = finalString2[j-1];
            OPT[i][j] = min({OPT[i-1][j-1] + mismatchPenalty[x_i - 'A'][y_j - 'A'], OPT[i-1][j] + gap_penalty,
                          OPT[i][j-1] + gap_penalty});
        }
    }


    alignedString1 = "";
    alignedString2 = "";
    i = n;
    j = m;

	
	while ( i >= 1 && j >= 1)
    {
        char x_i = finalString1[i-1];
        char y_j = finalString2[j-1];

        if (OPT[i][j] == OPT[i-1][j-1] + mismatchPenalty[x_i - 'A'][y_j - 'A'])
        {
            alignedString1 = x_i + alignedString1;
            alignedString2 = y_j + alignedString2;
            i--;
            j--;
            //
        }
        else if (OPT[i][j] == OPT[i][j-1] + gap_penalty)
        {
            alignedString1 = '_' + alignedString1;
            alignedString2 = y_j + alignedString2;
            j--;
        }
        else if (OPT[i][j] == OPT[i-1][j] + gap_penalty)
        {
            alignedString1 = x_i + alignedString1;
            alignedString2 = '_' + alignedString2;
			//
			i--;        
        }
        else
        	cout<<"Something Wrong \n";
    }

    while (i >= 1 && j < 1)
    {
        alignedString1 = finalString1[i-1] + alignedString1;
        alignedString2 = '_' + alignedString2;
        --i;
    }
    while (j >= 1 && i < 1)
    {
        alignedString1 = '_' + alignedString1;
        alignedString2 = finalString2[j-1] + alignedString2;
        --j;
    }
	
    return OPT[n][m];
}


int main(int argc, char *argv[])
{
	string inputFile = "";

	if (argc>1)
		inputFile = argv[1];
	else 
		cout<<"No input file provided \n";

	string baseString1 = "", baseString2 = ""; // The base string in the input file
	string finalString1 = "", finalString2 = ""; // The final strings generated from the base strings
	
	int j = 0, k = 0; 
	int counter = 0;

	fstream myFile;
	
	//myFile.open("input2.txt", ios::in); // Read 
	myFile.open(inputFile, ios::in);
	
	if ( myFile.is_open())
	{
		string line;
		//************
		string command;
		//*************
		while ( getline( myFile, line))
		{
			//***********
			istringstream iss(line);
			iss >> command;
			//***********
			if ( counter == 0)
			{
				//baseString1 += line;
				//finalString1 += line;
				baseString1 += command;
				finalString1 += command;
				counter++;
			}
			
			else if ( counter == 1)
			{
				//************
				if (isNumber(command))
				//*************
				//if ( isNumber(line))
				{
					//int indexToAppend = stoi(line);
					int indexToAppend = stoi(command);
					indexToAppend++;
					finalString1.insert(indexToAppend,finalString1);
					j++;
				}
				else counter++;
			}

			if ( counter == 2)
			{
				//baseString2 += line;
				//finalString2 += line;
				baseString2 += command;
				finalString2 += command;
				counter++;
			}

			else if (counter == 3)
			{
				//************
				if ( isNumber(command))
				//*************
				//if ( isNumber(line))
				{
					//int indexToAppend = stoi(line);
					int indexToAppend = stoi(command);
					indexToAppend++;
					finalString2.insert(indexToAppend,finalString2);
					k++;	
					
				}
			}

		}
		myFile.close();
	}
	int m, n;
	n = validateLength(finalString1, baseString1, j); // Validate 1st generated string is of length (2^j)*str1.length 
	m = validateLength(finalString2, baseString2, k); // Validate 2nd generated string is of length (2^k)*str2.length

	if ( n == -1 || m == -1)
		cout<<"Error in string generation \n";
	
	/*
	cout<<"base string 1 is: "<<baseString1<<"\n";
	cout<<"j is: "<<j<<"\n";
	cout<<"final generated string 1 is: "<<finalString1<<"\n";
	cout<<"base string 2 is: "<<baseString2<<"\n";
	cout<<"k is: "<<k<<"\n";
	cout<<"final generated string 2 is: "<<finalString2<<"\n";
	cout<<"n is :"<<n<<"\n";
	cout<<"m is :"<<m<<"\n";
	*/

	int gap_penalty = 30;
    int mismatchPenalty[letters][letters];
    
    for (int i = 0; i < letters; i++)
    {
        for (int j = 0; j < letters; j++)
        {
            if (i == j) 
                mismatchPenalty[i][j] = 0;
            else 
                mismatchPenalty[i][j] = 1;
        }
    }

    mismatchPenalty['A'-'A']['C'-'A'] = 110;
    mismatchPenalty['A'-'A']['G'-'A'] = 48;
    mismatchPenalty['A'-'A']['T'-'A'] = 94;

    mismatchPenalty['C'-'A']['A'-'A'] = 110;
    mismatchPenalty['C'-'A']['G'-'A'] = 118;
    mismatchPenalty['C'-'A']['T'-'A'] = 48;

    mismatchPenalty['G'-'A']['A'-'A'] = 48;
    mismatchPenalty['G'-'A']['C'-'A'] = 118;
    mismatchPenalty['G'-'A']['T'-'A'] = 110;
    
    mismatchPenalty['T'-'A']['A'-'A'] = 94;
    mismatchPenalty['T'-'A']['C'-'A'] = 48;
    mismatchPenalty['T'-'A']['G'-'A'] = 110;

    string alignedString1, alignedString2;

    int penalty = alignSequence(finalString1, finalString2, gap_penalty, mismatchPenalty, alignedString1, alignedString2);

    cout << "finalString1: " << finalString1 << endl;
    cout << "finalString2: " << finalString2 << endl;
    cout << "Overall penalty " << penalty << endl;
    cout << "Aligned sequences: " << endl;
    cout << alignedString1 << endl;
    cout << alignedString2 << endl;

	return 0;
}




