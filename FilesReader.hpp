#pragma once
#include <fstream>
#include <string>
#include <memory>
#include <vector>

class FileReader
{
public:
	FileReader();

	bool ReadBlock (std::string& theBlock, size_t theBlockSize);

	bool OpenFile (const std::string& theFileName);


private:

	std::ifstream myFile;
};