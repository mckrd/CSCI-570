#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <ctype.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

int transform(char c)
{
	int i = -1;
	if (c == 'A') i = 0;
	if (c == 'C') i = 1;
	if (c == 'G') i = 2;
	if (c == 'T') i = 3;
	return i;
}

int sequence_alignment(string a1, string b1, int delta, int alpha[4][4], string& a2, string& b2, int flag)
{
	int m = a1.size();
	int n = b1.size();

	if (flag == 0) 
	{
		// m * 2 array
		vector<vector<int>> OPT(m + 1, vector<int>(2, 0));

		/*
			Base case:
			OPT[i][0] = i * delta for i = 0 to m
		*/
		for (int i = 0; i <= m; ++i)
			OPT[i][0] = i * delta;

		/*
			Recurrence:
		*/
		for (int j = 1; j <= n; ++j) 
		{
			OPT[0][1] = j * delta;
			for(int i = 1; i <= m; ++i)
			{
				int x_i = transform(a1[i - 1]);
				int y_j = transform(b1[j - 1]);
				OPT[i][1] = min(OPT[i - 1][0] + alpha[x_i][y_j],
					min(OPT[i - 1][1] + delta, OPT[i][0] + delta));
			}
			for (int i = 0; i <= m; ++i)
				OPT[i][0] = OPT[i][1];
		}

		/*
			Back-trace:
			NEEDS TO BE FINISHED!!!!!!!
		*/

		return OPT[m][1];
	}


	else 
	{
		// m * n array
		vector<vector<int>> OPT(m + 1, vector<int>(n + 1, 0));

		/*
			Base case:
			OPT[0][j] = j * delta for j = 0 to n
			OPT[i][0] = i * delta for i = 0 to m
		*/
		for (int j = 0; j <= n; ++j)
			OPT[0][j] = j * delta;
		for (int i = 0; i <= m; ++i)
			OPT[i][0] = i * delta;

		/*
			Recurrence:
		*/
		for (int j = 1; j <= n; ++j)
		{
			for (int i = 1; i <= m; ++i)
			{
				int x_i = transform(a1[i - 1]);
				int y_j = transform(b1[j - 1]);

				OPT[i][j] = min(OPT[i - 1][j - 1] + alpha[x_i][y_j],
					min(OPT[i - 1][j] + delta, OPT[i][j - 1] + delta));
			}
		}

		/*
			Back-trace:
			NOTE THIS IS WRONG!!!!!!!
		*/
		int i = m;
		int j = n;
		while (i >= 1 && j >= 1)
		{
			int x_i = transform(a1[i - 1]);
			int y_j = transform(b1[j - 1]);

			if (OPT[i][j] == OPT[i - 1][j - 1] + alpha[x_i][y_j])
			{
				a2 = a1[i - 1] + a2;
				b2 = b1[j - 1] + b2;
				i--;
				j--;
			}

			else if (OPT[i][j] == OPT[i - 1][j] + delta) //up
			{
				a2 = a1[i - 1] + a2;
				b2 = '-' + b2;
				i--;
			}
			else if (OPT[i][j] == OPT[i][j - 1] + delta) // left
			{
				a2 = '-' + a2;
				b2 = b1[j - 1] + b2;
				j--;
			}
		}

		while (i >= 1 && j < 1)
		{
			a2 = a1[i - 1] + a2;
			b2 = '-' + b2;
			--i;
		}
		while (j >= 1 && i < 1)
		{
			a2 = '-' + a2;
			b2 = b1[j - 1] + b2;
			--j;
		}
		return OPT[m][n];
	}

	return -1;
}

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
	//PMC is used to detect how much RAM is used. May or may not be a windows-only command.
	PROCESS_MEMORY_COUNTERS pmc;

	string a = "", b = ""; // The base string a, b in the input file
	string a1 = "", b1 = ""; // The final strings a1, b1 generated from the base strings

	int j = 0, k = 0;
	int counter = 0;

	fstream myFile;
	myFile.open("input2.txt", ios::in); // Read

	ofstream outputFile("output.txt"); //File to write out to

	if (myFile.is_open())
	{
		string line;
		while (getline(myFile, line))
		{
			if (counter == 0)
			{
				a += line;
				a1 += line;
				counter++;
			}

			else if (counter == 1)
			{
				if (isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					a1.insert(indexToAppend, a1);
					j++;
				}
				else counter++;
			}

			if (counter == 2)
			{
				b += line;
				b1 += line;
				counter++;
			}

			else if (counter == 3)
			{
				if (isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					b1.insert(indexToAppend, b1);
					k++;
				}
			}
		}
		myFile.close();
	}

	validateLength(a1, a, j); // Validate 1st generated string is of length (2^j)*str1.length 
	validateLength(b1, b, k); // Validate 2nd generated string is of length (2^k)*str2.length

	// The description asks us to validate the length of the generated strings. Just in case, the function returns false, what do we do?
	/*
	cout<<"base string 1 is: "<<a<<"\n";
	cout<<"j is: "<<j<<"\n";
	cout<<"final generated string 1 is: "<<a1<<"\n";
	cout<<"base string 2 is: "<<b<<"\n";
	cout<<"k is: "<<k<<"\n";
	cout<<"final generated string 2 is: "<<b1<<"\n";
	*/

	/*
		The mismatch penalty (alpha) table
	*/
	int alpha[4][4];
	for (int i = 0; i < 4; ++i)
		for (size_t j = 0; j < 4; ++j)
			if (i == j) alpha[i][j] = 0;
	alpha[0][1] = 110;
	alpha[0][2] = 48;
	alpha[0][3] = 94;
	alpha[1][0] = 110;
	alpha[1][2] = 118;
	alpha[1][3] = 48;
	alpha[2][0] = 48;
	alpha[2][1] = 118;
	alpha[2][3] = 110;
	alpha[3][0] = 94;
	alpha[3][1] = 48;
	alpha[3][2] = 110;

	/*
		The gap penalty (delta)
	*/
	int delta = 30;

	/*
		The flag to choose basic/memory-efficient version of the algorithm
	*/
	int flag = 0;

	/*
		The resulting aligned strings
	*/
	string a2, b2 = "";
	int penalty = sequence_alignment(a1, b1, delta, alpha, a2, b2, flag);


	//temporary timer code starts here

	//clock_t startTime = clock();
	//float secondsPassed;
	//float secondsToDelay = 10;

	//bool flag = true;
	//while (flag) {
	//	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	//	if (secondsPassed >= secondsToDelay) {
	//		if (outputFile.is_open()) outputFile << secondsPassed << " seconds have passed.\n";
	//		flag = false;
	//	}
	//}
	
	if (outputFile.is_open()) {
		outputFile << "a: " << a1 << endl;
		outputFile << "b: " << b1 << endl;
		outputFile << "Penalty: " << penalty << endl;
		outputFile << "Aligned sequences: " << endl;
		outputFile << a2 << endl;
		outputFile << b2 << endl;
	}

	//Outputs RAM being used. However, not sure if this is specific to windows machines or not
	bool result = K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

	outputFile << physMemUsedByMe/1024 << " Kilobytes of memory used.\n";


	return 0;
}