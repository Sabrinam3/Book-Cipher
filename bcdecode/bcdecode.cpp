/*
Name: Sabrina Tessier
Date: July 12, 2018
Purpose: This program takes a coded file and uses a book file to decode a message. This program then writes the results to another file.
Version: 1.0
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int checkStream(ios &stream, char* argv[], int);

int main(int argc, char * argv[]) {
	//Print my name and name of program
	cout << "bcdecode (c) 2018, Sabrina Tessier" << endl;

	//Check to make sure the user has entered 4 arguments. 
	//If user has entered anything other than 4, tell the user this.
	//Also tell the user the desired input format
	if (argc != 4) {
		cerr << "Incorrect number of arguments <" << argc << ">" << endl;
		cerr << "Usage: BookCipherDecode bookfile codedfile resultfile" << endl;
		return EXIT_FAILURE;
	}

	//Create the file streaming objects that we will need to use to read and write 

	//ifstream object for reading the bookfile
	ifstream bookReader(argv[1]);
	//check to make sure file was opened for reading
	if (checkStream(bookReader, argv, 1) == 1) {
		return EXIT_FAILURE;
	}

	//ifstream object for reading the coded file
	ifstream messageReader(argv[2]);
	//check to make sure file was opened for reading
	if (checkStream(messageReader, argv, 2) == 1) {
		return EXIT_FAILURE;
	}

	//ofstream object for writing the results to file
	ofstream messageDecoder(argv[3]);
	//check to make sure file was created
	if (checkStream(messageDecoder, argv, 3) == 1) {
		return EXIT_FAILURE;
	}

	//CREATE A CHAR REPRESENTATION OF THE BOOK

	//Vector to hold the book
	vector<vector<unsigned char>> bookVector;

	//Variable to keep track of the line number of the book 
	unsigned int lineNumber = 0;

	//Varible to hold the character
	char ch;

	//Iterate through the book
	while (bookReader.get(ch)) {
		//Resize the outer vector to the current value of lineNumber
		bookVector.resize(lineNumber+1);
		//Cast to unsigned char
		unsigned char bookChar = static_cast<unsigned char>(ch);
		//Add the character to the vector
		bookVector[lineNumber].push_back(bookChar);
		//If the line is a newline, increment the size of the outer vector
		if (bookChar == '\n')
			lineNumber++;
	}//end while
	
	//NOW WE WILL READ THROUGH OUR CODED FILE AND GET THE CHARACTERS FROM THE BOOKVECTOR

	//Variables to hold each value read in from the coded file
	int lineNum;
	int charNum;
	
	//Read until the end of the coded file 
	while (messageReader >> lineNum >> charNum) {
		//write the decoded character to the result file
		messageDecoder << bookVector[lineNum][charNum];
	}//end while
	cout << "Message decoded. Check directory to find " << argv[3] << endl;
}//end main

 /*
 @fn int checkStream(ios &stream, char* argv[], int num)
 @brief Checks to ensure stream was opened for reading/writing
 @param ios &stream[in]: the stream object, char* argv[] [in]: the char array, num: an integer
 reprsenting the file being opened/created
 @return 1 if the file open failed, or 0 if the file open succeeded
 */
int checkStream(ios &stream, char* argv[], int num) {
	if (!stream) {
		if (num == 1) {
			cerr << "Could not open bookfile: " << argv[1] << "." << endl;
		}
		else if (num == 2) {
			cerr << "Could not open message file: " << argv[2] << "." << endl;
		}
		else if (num == 3) {
			cerr << "Could not create file: " << argv[3] << "." << endl;
		}
		return 1;
	}
	else {
		return 0;
	}
}





