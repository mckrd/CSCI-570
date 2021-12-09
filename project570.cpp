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

bool validateLength(string finalString, string baseString, int n)
{
	int validLength = pow(2, n) * baseString.length();
	return (finalString.length() == validLength);
}

bool isNumber(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;

	return true;
}

//int transform(char c)
//{
//	int i = -1;
//	if (c == 'A') i = 0;
//	if (c == 'C') i = 1;
//	if (c == 'G') i = 2;
//	if (c == 'T') i = 3;
//	return i;
//}

const size_t alphabets = 26;

int align(string finalString1, string finalString2, int gap_penalty,
	int mismatchPenalty[alphabets][alphabets], string& alignedString1, string& alignedString2)
{
	int n = finalString1.size();
	int m = finalString2.size();

	vector<vector<int> > A(n + 1, vector<int>(m + 1));

	int i, j;

	for (i = 0; i <= m; ++i)
		A[0][i] = gap_penalty * i;
	for (i = 0; i <= n; ++i)
		A[i][0] = gap_penalty * i;

	for (i = 1; i <= n; ++i)
	{
		for (j = 1; j <= m; ++j)
		{
			char x_i = finalString1[i - 1];
			char y_j = finalString2[j - 1];
			A[i][j] = min( A[i - 1][j - 1] + mismatchPenalty[x_i - 'A'][y_j - 'A'],
						  min(A[i - 1][j] + gap_penalty, A[i][j - 1] + gap_penalty) );
		}
	}


	//alignedString1 = "";
	//alignedString2 = "";
	i = n;
	j = m;


	while (i >= 1 && j >= 1)
	{
		char x_i = finalString1[i - 1];
		char y_j = finalString2[j - 1];
		if (A[i][j] == A[i - 1][j - 1] + mismatchPenalty[x_i - 'A'][y_j - 'A'])
		{
			alignedString1 = x_i + alignedString1;
			alignedString2 = y_j + alignedString2;
			i--;
			j--;
			//
		}
		else if (A[i][j] == A[i - 1][j] + gap_penalty)
		{
			alignedString1 = x_i + alignedString1;
			alignedString2 = '-' + alignedString2;
			//
			i--;
		}
		else if (A[i][j] == A[i][j - 1] + gap_penalty)
		{
			alignedString1 = '-' + alignedString1;
			alignedString2 = y_j + alignedString2;
			j--;
		}
		else
			cout << "Something Wrong \n";
	}

	while (i >= 1 && j < 1)
	{
		alignedString1 = finalString1[i - 1] + alignedString1;
		alignedString2 = '-' + alignedString2;
		--i;
	}
	while (j >= 1 && i < 1)
	{
		alignedString1 = '-' + alignedString1;
		alignedString2 = finalString2[j - 1] + alignedString2;
		--j;
	}

	return A[n][m];
}

//int sequence_alignment(string a1, string b1, int delta, int alpha[4][4], string& a2, string& b2, int flag)
//{
//	int m = a1.size();
//	int n = b1.size();
//
//	if (flag == 0) 
//	{
//		// m * 2 array
//		vector<vector<int>> OPT(m + 1, vector<int>(2, 0));
//
//		/*
//			Base case:
//			OPT[i][0] = i * delta for i = 0 to m
//		*/
//		for (int i = 0; i <= m; ++i)
//			OPT[i][0] = i * delta;
//
//		/*
//			Recurrence:
//		*/
//		for (int j = 1; j <= n; ++j) 
//		{
//			OPT[0][1] = j * delta;
//			for(int i = 1; i <= m; ++i)
//			{
//				int x_i = transform(a1[i - 1]);
//				int y_j = transform(b1[j - 1]);
//				OPT[i][1] = min(OPT[i - 1][0] + alpha[x_i][y_j],
//					min(OPT[i - 1][1] + delta, OPT[i][0] + delta));
//			}
//			for (int i = 0; i <= m; ++i)
//				OPT[i][0] = OPT[i][1];
//		}
//
//		/*
//			Back-trace:
//			NEEDS TO BE FINISHED!!!!!!!
//		*/
//
//		return OPT[m][1];
//	}
//
//
//	else 
//	{
//		// m * n array
//		vector<vector<int>> OPT(m + 1, vector<int>(n + 1, 0));
//
//		/*
//			Base case:
//			OPT[0][j] = j * delta for j = 0 to n
//			OPT[i][0] = i * delta for i = 0 to m
//		*/
//		for (int j = 0; j <= n; ++j)
//			OPT[0][j] = j * delta;
//		for (int i = 0; i <= m; ++i)
//			OPT[i][0] = i * delta;
//
//		/*
//			Recurrence:
//		*/
//		for (int j = 1; j <= n; ++j)
//		{
//			for (int i = 1; i <= m; ++i)
//			{
//				int x_i = transform(a1[i - 1]);
//				int y_j = transform(b1[j - 1]);
//
//				OPT[i][j] = min(OPT[i - 1][j - 1] + alpha[x_i][y_j],
//					min(OPT[i - 1][j] + delta, OPT[i][j - 1] + delta));
//			}
//		}
//
//		/*
//			Back-trace:
//			NOTE THIS IS WRONG!!!!!!!
//		*/
//		int i = m;
//		int j = n;
//		while (i >= 1 && j >= 1)
//		{
//			int x_i = transform(a1[i - 1]);
//			int y_j = transform(b1[j - 1]);
//
//			if (OPT[i][j] == OPT[i - 1][j - 1] + alpha[x_i][y_j])
//			{
//				a2 = a1[i - 1] + a2;
//				b2 = b1[j - 1] + b2;
//				i--;
//				j--;
//			}
//
//			else if (OPT[i][j] == OPT[i - 1][j] + delta) //up
//			{
//				a2 = a1[i - 1] + a2;
//				b2 = '-' + b2;
//				i--;
//			}
//			else if (OPT[i][j] == OPT[i][j - 1] + delta) // left
//			{
//				a2 = '-' + a2;
//				b2 = b1[j - 1] + b2;
//				j--;
//			}
//		}
//
//		while (i >= 1 && j < 1)
//		{
//			a2 = a1[i - 1] + a2;
//			b2 = '-' + b2;
//			--i;
//		}
//		while (j >= 1 && i < 1)
//		{
//			a2 = '-' + a2;
//			b2 = b1[j - 1] + b2;
//			--j;
//		}
//		return OPT[m][n];
//	}
//
//	return -1;
//}

void memEff_sequence_alignment(vector<int> &cost, string a1, string b1, int delta, int mismatchPenalty[alphabets][alphabets]) {
	int m = a1.size();
	int n = b1.size();

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
		for (int i = 1; i <= m; ++i)
		{
			char x_i = a1[i - 1];
			char y_j = b1[j - 1];
			OPT[i][1] = min( OPT[i - 1][0] + mismatchPenalty[x_i - 'A'][y_j - 'A'],
						  min(OPT[i - 1][1] + delta, OPT[i][0] + delta) );

			/*int x_i = transform(a1[i - 1]);
			int y_j = transform(b1[j - 1]);
			OPT[i][1] = min(OPT[i - 1][0] + alpha[x_i][y_j], 
							min(OPT[i - 1][1] + delta, OPT[i][0] + delta));*/
		}
		for (int i = 0; i <= m; ++i)
			OPT[i][0] = OPT[i][1];
	}

	for (int i = 0; i <= m; i++) {
		cost[i] = OPT[i][1];
	}

	//return OPT[m][1];

}

void D_and_C_Alignment(string a1, string b1, int delta, int mismatchPenalty[alphabets][alphabets],
						string & a2, string & b2, int & penalty) {
	int m = a1.size();
	int n = b1.size();
	int nMid = n / 2;
	int mMid = 0;

	if (m <= 2 || n <= 2) penalty += align(a1, b1, delta, mismatchPenalty, a2, b2); 
	else {
		string bLowerHalf = b1.substr(0, nMid);
		string bUpperHalf = b1.substr(nMid);
		vector<int> tcost(a1.size() +1), lcost(a1.size() +1), rcost(a1.size() +1);

		string a1Rev = a1;
		string bUpperHalfRev = bUpperHalf;

		reverse(a1Rev.begin(), a1Rev.end());
		reverse(bUpperHalfRev.begin(), bUpperHalfRev.end());

		memEff_sequence_alignment(lcost, a1, bLowerHalf, delta, mismatchPenalty);
		memEff_sequence_alignment(rcost, a1Rev, bUpperHalfRev, delta, mismatchPenalty);


		for (int i = 0; i <= a1.size(); i++) {
			tcost[i] = lcost[i] + rcost[a1.size() - i];
			if (tcost[i] < tcost[mMid]) {
				mMid = i;
			}
		}

		//a2 += a1.at(mMid);
		//b2 += b1.at(nMid);

		string aLowerHalf = a1.substr(0, mMid);
		string aUpperHalf = a1.substr(mMid);


		D_and_C_Alignment(aUpperHalf, bUpperHalf, delta, mismatchPenalty, a2, b2, penalty);
		D_and_C_Alignment(aLowerHalf, bLowerHalf, delta, mismatchPenalty, a2, b2, penalty);


		
	}
}

int main(int argc, char *argv[])
{
	//PMC is used to detect how much RAM is used. May or may not be a windows-only command.
	PROCESS_MEMORY_COUNTERS pmc;

	clock_t startTime = clock();

	string inputFile = "";

	if (argc > 1)
		inputFile = argv[1];
	else
		cout << "No input file provided \n";

	string baseString1 = "", baseString2 = ""; // The base string in the input file
	string finalString1 = "", finalString2 = ""; // The final strings generated from the base strings

	int j = 0, k = 0;
	int counter = 0;

	fstream myFile;
	ofstream outputFile("output.txt");

	//myFile.open("input2.txt", ios::in); // Read 
	myFile.open(inputFile, ios::in);

	if (myFile.is_open())
	{
		string line;
		while (getline(myFile, line))
		{
			if (counter == 0)
			{
				baseString1 += line;
				finalString1 += line;
				counter++;
			}

			else if (counter == 1)
			{
				if (isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					finalString1.insert(indexToAppend, finalString1);
					j++;
				}
				else counter++;
			}

			if (counter == 2)
			{
				baseString2 += line;
				finalString2 += line;
				counter++;
			}

			else if (counter == 3)
			{
				if (isNumber(line))
				{
					int indexToAppend = stoi(line);
					indexToAppend++;
					finalString2.insert(indexToAppend, finalString2);
					k++;
				}
			}

		}
		myFile.close();
	}
	int m, n;
	n = validateLength(finalString1, baseString1, j); // Validate 1st generated string is of length (2^j)*str1.length 
	m = validateLength(finalString2, baseString2, k); // Validate 2nd generated string is of length (2^k)*str2.length

	if (n == -1 || m == -1)
		cout << "Error in string generation \n";

	// The description asks us to validate the length of the generated strings. Just in case, the function returns false, what do we do?
	/*
	cout<<"base string 1 is: "<<a<<"\n";
	cout<<"j is: "<<j<<"\n";
	cout<<"final generated string 1 is: "<<a1<<"\n";
	cout<<"base string 2 is: "<<b<<"\n";
	cout<<"k is: "<<k<<"\n";
	cout<<"final generated string 2 is: "<<b1<<"\n";
	*/
	/*int alpha[4][4];
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
	alpha[3][2] = 110;*/

	/*
	The mismatch penalty (alpha) table
	*/

	int mismatchPenalty[alphabets][alphabets];

	for (int i = 0; i < alphabets; ++i)
	{
		for (int j = 0; j < alphabets; ++j)
		{
			if (i == j)
				mismatchPenalty[i][j] = 0;
			else
				mismatchPenalty[i][j] = 1;
		}
	}

	mismatchPenalty['A' - 'A']['C' - 'A'] = 110;
	mismatchPenalty['A' - 'A']['G' - 'A'] = 48;
	mismatchPenalty['A' - 'A']['T' - 'A'] = 94;

	mismatchPenalty['C' - 'A']['A' - 'A'] = 110;
	mismatchPenalty['C' - 'A']['G' - 'A'] = 118;
	mismatchPenalty['C' - 'A']['T' - 'A'] = 48;

	mismatchPenalty['G' - 'A']['A' - 'A'] = 48;
	mismatchPenalty['G' - 'A']['C' - 'A'] = 118;
	mismatchPenalty['G' - 'A']['T' - 'A'] = 110;

	mismatchPenalty['T' - 'A']['A' - 'A'] = 94;
	mismatchPenalty['T' - 'A']['C' - 'A'] = 48;
	mismatchPenalty['T' - 'A']['G' - 'A'] = 110;

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
	int penalty = 0;


	D_and_C_Alignment(finalString1, finalString2, delta, mismatchPenalty, a2, b2, penalty);
	//penalty = align(finalString1, finalString2, delta, mismatchPenalty, a2, b2);

	//Outputs RAM being used. This code is specific to Windows Machines:
	bool result = K32GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

	double msTimer = (clock() - startTime) / (double)(CLOCKS_PER_SEC / 1000);
	
	if (outputFile.is_open()) {
		outputFile << "a: " << finalString1 << endl;
		outputFile << "b: " << finalString2 << endl;
		outputFile << "Penalty: " << penalty << endl;
		outputFile << "Memory Used: " << physMemUsedByMe / 1024 << endl;
		outputFile << "Seconds Passed: " << msTimer / 1000 << endl << endl;
		outputFile << "Aligned sequences: " << endl;

		if (a2.size() <= 50 && b2.size() <= 50) {
			outputFile << a2 << endl;
			outputFile << b2 << endl;
		}
		else {
			string afirst50, alast50 = "";
			string bfirst50, blast50 = "";

			for (int i = 0; i <= 50; i++) {
				afirst50 += a2.at(i);
				alast50 += a2.at((a2.size() - 1) - i);
			}

			for (int i = 0; i <= 50; i++) {
				bfirst50 += b2.at(i);
				blast50 += b2.at((b2.size() - 1) - i);
			}

			reverse(alast50.begin(), alast50.end());
			reverse(blast50.begin(), blast50.end());

			outputFile << "First 50: " << afirst50 << endl;
			outputFile << "First 50: " << bfirst50 << endl;
			
			outputFile << "Last 50: " << alast50 << endl;
			outputFile << "Last 50: " << blast50 << endl;
		}
	}


	return 0;
}