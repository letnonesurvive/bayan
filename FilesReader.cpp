#include "FilesReader.hpp"
#include <exception>
#include <iostream>

using namespace std;

FilesReader::FilesReader() : myBuffer{}, myFileName1(""), myFileName2("")
{
}

void FilesReader::SetBlockSize (size_t theBlockSize) 
{
	//myBlockSize = theBlockSize;
}

void FilesReader::SetPair (const std::string& theFileName1, const std::string& theFileName2)
{
	myFileName1 = theFileName1;
	myFileName2 = theFileName2;
}

std::vector<std::string> FilesReader::Blocks() const
{
	return myBlocks;
}

bool FilesReader::OpenFile (const std::string& theFileName)
{
	myFile = std::ifstream (theFileName, std::ios::binary);
	if (!myFile.is_open()) {
		return false;
	}
	return true;
}

bool FilesReader::ReadBlock (string& theBlock, size_t theBlockSize)
{
	if (!myFile.eof()) {
		char* aBuffer = new char [theBlockSize + 1] {};
		myFile.read (aBuffer, theBlockSize);
		int aBytesRead = myFile.gcount(); // get bytes count red in fact 
		if (aBytesRead < theBlockSize) {
			for (int i = aBytesRead; i < theBlockSize; ++i) {
				aBuffer[i] = '\0';
			}
		}
		aBuffer[theBlockSize] = '\0';
		theBlock = aBuffer;
		return true;
	}
	myFile.close();
	return false;
}

//bool FilesReader::Read (const std::string& theFileName)
//{
//	std::ifstream aFile(theFileName, std::ios::binary);
//	if (!aFile.is_open()) {
//		return false;
//	}
//
//	while (!aFile.eof()) {
//		aFile.read(myBuffer, myBlockSize);
//		int aBytesRead = aFile.gcount(); // get bytes count red in fact 
//		if (aBytesRead < myBlockSize) {
//			for (int i = aBytesRead; i < myBlockSize; ++i) {
//				myBuffer[i] = '\0';
//			}
//		}
//		myBlocks.push_back (myBuffer);
//	}
//	aFile.close();
//
//	return true;
//}

