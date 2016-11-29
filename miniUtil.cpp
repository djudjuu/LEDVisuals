#include<vector>
#include<fstream>
#include<iostream>
#include<math.h>

using namespace std;

vector<int> readLEDmap(char* pathToLEDKarte)
{    
	std::ifstream inFile(pathToLEDKarte);
	//std::ifstream inFile("../LEDkarte.txt");
	//Create an input file stream
	//ifstream in(pathToLEDKarte, ios::in);

	//this used to work outside ardiuno
	//ifstream in("./LEDkarte.txt".)pathToLEDKarte, ios::in);
	vector<int>numbers;
	//Read number using the extraction (>>) operator
	int number;	//Variable to hold each number as it is read
	// As long as we haven't reached the end of the file, keep reading entries.
	while( inFile  >> number)
	{
		numbers.push_back(number);
	}
	//Close the file stream
	inFile.close();
	//uncommment this code for testing by printing out the first k= 20 entries
	cout << "Numbers:\n";
	int k = 20;
	for (int i=0; i<numbers.size(); i++) 
	{
		cout << numbers[i] << ',';
	}
	/**/
	return numbers;
} 

int NUM_LEDS =31;
int LEDkarte [31] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15};
const int nSegs = 15;

//---------- initialization ------------
//create a lookupTable for LEDpositions and fill it
// list of pointers to vector<int>s
vector<int>* seg2ledsLookup [nSegs];

vector<int>* mapSeg(int seg)
{
	//returns a pointer to a vector of ints that represent all indeces of the LEDs in the given segement
	return seg2ledsLookup[seg];
} 

void fillSegLookUp(){
	//create one vector for each segment
	for (int s=0; s <= nSegs; s++) 
	{
		cout << "segment"<< s<< "holds LEDS: ";
		vector<int>* segVec = new vector<int>(); //create new empty vector for the segment//TODO use new operation
		//now go over the LEDS and push their locations on the segment-vector
		for (int j=0; j < NUM_LEDS; j++)
		{
		if (LEDkarte[j]==s){
				segVec->push_back(j);
				cout << segVec->back()<<',';
			}
		}
		//now save the adress of the segVec in the lookupTable
		seg2ledsLookup[s] = segVec;

		cout << segVec->size()<<endl;
	}
	cout << "firstnumber is"<<(*seg2ledsLookup[0])[0]<<endl;
	//return 1;
} 


int a = 5;
int b =2;
int main()
{
	//learning about c++ addition
	cout <<"5/2"<<a/b<<"ee"<< max(0, 20-10)<<endl;
	int breite = 3;
	cout << "ballreadius"<<floor(256/(floor(breite/2) +1))<< endl;
	//cout << "ahhhhhhhhhhhh"<< endl;
	fillSegLookUp();
	//cout << "bhhhhhhhhhhhh"<< nSegs<<endl;

	vector<int>* segleds;
	for ( int i=0; i <= nSegs; i++)
	{
		//cout << "bbb" << endl;
		segleds = mapSeg(i);
		//cout << "seg"<< i<< "holds"<<segleds->size();
		//cout << "elements and its LEDS-indeces are:";
		for (int i : *segleds)
			cout << i<<',';
			//leds[ i ] = CRGB::Red;
		//cout <<"ende segment"<< endl;
	}
	//cout << "ende"<<endl;
	return 1;
}




