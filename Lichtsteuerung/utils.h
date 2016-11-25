#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main()
{    
	ifstream File;
	File.open("LEDkarte.txt");
	std::vector<int>numbers;
	int number;
	while(InFile >> number)
		numbers.push_back(number);
	cout << "aaaand...\n";
	cout << numbers
	cout << "done.\n.";

}
