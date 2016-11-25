#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

vector<int> readLEDmap(string pathToLEDKarte)
{    
	ifstream File;
	//Create an input file stream
	ifstream in(pathToLEDKarte, ios::in);
	vector<int>numbers;
	//Read number using the extraction (>>) operator
	int number;	//Variable to hold each number as it is read
	// As long as we haven't reached the end of the file, keep reading entries.
	while( in  >> number)
	{
		numbers.push_back(number);
	}
	//Close the file stream
	in.close();
	//cout << "aaaand...\n";
	//cout << numbers
	//cout << "done.\n.";

	cout << "Numbers:\n";
	for (int i=0; i<numbers.size()-400; i++) 
	{
		cout << numbers[i] << ',';
	}
	return numbers;
}

int main()
{
	readLEDmap();
}

