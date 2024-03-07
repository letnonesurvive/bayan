
#include <fstream>
#include <string>
#include <memory>
#include <vector>

class FilesReader
{
public:
	FilesReader();

	//bool Read (const std::string& theFileName);

	bool ReadBlock(std::string& theBlock, size_t theBlockSize);

	bool OpenFile(const std::string& theFileName);

	void SetBlockSize (size_t theBlockSize);

	void SetPair (const std::string& theFileName1, const std::string& theFileName2);

	std::vector<std::string> Blocks() const;

private:

	std::string myFileName1;
	std::string myFileName2;
	char myBuffer [255] = {};
	//size_t myBlockSize;
	std::vector<std::string> myBlocks;
	std::ifstream myFile;
};