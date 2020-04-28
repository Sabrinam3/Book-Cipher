/*
Name: Sabrina Tessier
Date: July 12, 2018
Purpose: This program takes a text file and encodes it using a book file. This program then writes the results to another file.
Version: 1.0
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

int checkStream(ios &stream, char* argv[], int);
int validateChar(vector<vector<pair<int, int>>> &v, unsigned int, char, char* argv[]);
pair<int, int> findValidPair(vector<vector<pair<int, int>>> &v, unsigned int);


int main(int argc, char * argv[]) {
	//Print my name and name of program
	cout << "bcencode (c) 2018, Sabrina Tessier" << endl;

	//Check to make sure the user has entered 4 arguments. 
	//If user has entered anything other than 4, tell the user this.
	//Also tell the user the desired input format
	if (argc != 4) {
		cerr << "Incorrect number of arguments <" << argc << ">" << endl;
		cerr << "Usage: BookCipherEncode bookfile messagefile codedfile" << endl;
		return EXIT_FAILURE;
	}

	//Create the file streaming objects that we will need to use to read and write 

	//ifstream object for reading the bookfile
	ifstream bookReader(argv[1]);
	//check to make sure book file was opened for reading
	if (checkStream(bookReader, argv, 1) == 1) {
		return EXIT_FAILURE;
	}
	//ifstream object for reading the message
	ifstream messageReader(argv[2]);
	//Check to make sure the file was opened
	if (checkStream(messageReader, argv, 2) == 1) {
		return EXIT_FAILURE;
	}
	//ofstream object for writing the encoded message to file
	ofstream messageCoder(argv[3]);
	//check to make sure file was created
	if (checkStream(messageCoder, argv, 3) == 1) {
		return EXIT_FAILURE;
	}

	//Create a 2D vector to hold the code table
	vector <vector<pair<int, int>>> codeTable;
	codeTable.resize(256);

	//Look through the book file and create a table of each character's coordinates
	//Char to hold each character from the book file
	char bookChar;

	//Value to keep track of line number and character count of the bookfile
    int lineNumber = 0;
	int charNumber = 0;

	//Iterate through the bookfile using a while loop
	while (bookReader.get(bookChar)) {

		//Cast the character to an unsigned char
		unsigned char ch = static_cast<unsigned char>(bookChar);

		//Get the ASCII value of the bookChar so that we know what index of the codeTable to place value in
		unsigned int bookKey = static_cast<unsigned int>(ch);

		//Make a pair for each character, using the lineNumber and charNumber
		pair <int,int> coordinates = make_pair(lineNumber, charNumber);

		//Add this pair to the corresponding index of the codeTable
		codeTable[bookKey].push_back(coordinates);

		//if the character is a new line, we should increment the line number and reset char number
		if (bookKey == 10) {
			lineNumber++;
			charNumber = -1;
		}

		//Increment the charNumber
		charNumber++;
	}// end while


	//Now that the code table is populated, we can go through the message file and choose coordinates for each character in the message
	//We need a char to read the characters from the message
	char messageChar;

	//Now we can read the message file
	while (messageReader.get(messageChar)) {

		//Cast char to unsigned
		unsigned char umessageChar = static_cast<unsigned char>(messageChar);

		//Get the ASCII value of the char, so we know the key of the code table to look for pairs
		unsigned int messageKey = static_cast<unsigned int>(umessageChar);

		//If the messageChar is not found in the book file, we need to tell the user this and then exit the program
		if (validateChar(codeTable, messageKey, umessageChar, argv) == 1) {
			return EXIT_FAILURE;
		}

		//We will now find a valid pair of coordinates to output to our coded file
		pair<int, int> coordinates = findValidPair(codeTable, messageKey);
		messageCoder << coordinates.first << " " << coordinates.second << " ";	
	}//end while
	cout << "Message encoded. Check directory to find " << argv[3] << endl;
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
	}//end if
	else {
		return 0;
	}
}//end function

 /*
 @fn int validateChar (vector<vector<pair<int, int>>> &v, unsigned int key, char ch, char* argv[])
 @brief checks if a character is found within the book file
 @param vector<vector<pair<int,int>>> &v[in]: a vector of vector of pairs of integers representing
 the code table, unsigned int key: the ASCII value of the character to be encoded,
 char ch[in] : a char representation of the character to be encoded, char* argv[][in]: the character
 array representing the command line arguments
 @return 1 if the character cannot be encoded, or 0 if the character can be encoded
 */
int validateChar(vector<vector<pair<int, int>>> &v, unsigned int key, char ch, char* argv[]) {
	if (v[key].size() == 0) {
		cerr << "Error: message cannot be encoded using this book" << endl;
		cerr << ch << " was not found in " << argv[1] << endl;
		return 1;
	}
	else {
		return 0;
	}
}//end function

 /*
 @fn pair<int,int> findValidPair(vector<vector<pair<int, int>>> &v, unsigned int key)
 @brief looks through a vector of pairs and chooses one that has not been used before
 @param vector<vector<pair<int,int>>> &v[in]: a vector of vector of pairs of integers representing
 the code table, unsigned int key[in]: the ASCII value representing the current character
 to be encoded.
 @return a pair of integers, once a valid pair has been found.
 */
pair<int, int> findValidPair(vector<vector<pair<int, int>>> &v, unsigned int key) {
	int num1 = 0;
	int num2 = 0;
	//Look through the inner vector to find a valid pair
	for (unsigned int i = 0; i < v[key].size(); ++i) {
		if (v[key][i].first != -1) {
			//Choose this pair
			num1 = v[key][i].first;
			num2 = v[key][i].second;

			//if the pair we are using is not the one at the end, we will change the first value to -1 so that these coordinates become
			//invalid and won't be selected again
			if (i != v[key].size() - 1) {
				v[key][i].first = -1;
				break;
			}//end if
		}//end if	
	}//end for
	return make_pair(num1, num2);
}//end function
