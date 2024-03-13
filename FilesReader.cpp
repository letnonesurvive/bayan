#include "FilesReader.hpp"
#include <exception>
#include <iostream>

using namespace std;

FileReader::FileReader() 
{
}

bool FileReader::OpenFile (const std::string& theFileName)
{
	myFile = std::ifstream (theFileName, std::ios::binary);
	if (!myFile.is_open()) {
		return false;
	}
	return true;
}

bool FileReader::ReadBlock (string& theBlock, size_t theBlockSize)
{
	if (!myFile.eof()) {
		char* aBuffer = new char [theBlockSize + 1] {};
		myFile.read (aBuffer, theBlockSize);
		size_t aBytesRead = static_cast<size_t> (myFile.gcount()); // get bytes count red in fact 
		if (aBytesRead < theBlockSize) {
			for (size_t i = aBytesRead; i < theBlockSize; ++i) {
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


