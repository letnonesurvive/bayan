#pragma once

#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <map>

namespace fs = boost::filesystem;

class FileCommander
{
public:
	FileCommander();
	void SetBlockSize (size_t theBlockSize);
	void SetMinFileSize (size_t theMinFileSize);
	void SetScanLevel (bool theScanLevel);
	void SetDirectories (std::vector <fs::path> theDirs);
	void SetExcludeDirectories (std::vector <fs::path> theExcludeDirs);
	void SetHash (std::string theHash);

	std::multimap <size_t, fs::path> FindSameSizeFiles() const;
private:
	size_t myBlockSize;
	size_t myMinFileSize;
	bool myScanLevel;
	std::vector <fs::path> myDirectories;
	std::vector <fs::path> myExcludeDirectories;
	std::string myHashAlghoritm;

	//std::vector <fs::path> myFiles;
};