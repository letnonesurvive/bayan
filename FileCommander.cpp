#include "FileCommander.hpp"
#include <stdexcept>

#include <memory>
#include <algorithm>

using namespace std;

FileCommander::FileCommander() :
	myBlockSize(0), 
	myMinFileSize(0), 
	myScanLevel(false), 
	myDirectories{}, 
	myExcludeDirectories{}, 
	myHashAlghoritm("")
{

}

void FileCommander::SetBlockSize(size_t theBlockSize)
{
	theBlockSize = theBlockSize;
}

void FileCommander::SetMinFileSize(size_t theMinFileSize)
{
	myMinFileSize = theMinFileSize;
}

void FileCommander::SetScanLevel(bool theScanLevel)
{
	myScanLevel = theScanLevel;
}

void FileCommander::SetDirectories (std::vector<fs::path> theDirs)
{
	myDirectories = theDirs;
}

void FileCommander::SetExcludeDirectories(std::vector<fs::path> theExcludeDirs)
{
	myExcludeDirectories = theExcludeDirs;
}

void FileCommander::SetHash (std::string theHash)
{
	myHashAlghoritm = theHash;
}

namespace {
	void LeaveUnique (multimap <size_t, fs::path>& theFiles)
	{
		for (auto it = theFiles.begin(); it != theFiles.end();) {
			auto aLast = theFiles.upper_bound (it->first);
			if (std::distance (it, aLast) == 1) {
				it = theFiles.erase (it);
			}
			it = aLast;
		}
	}
}

multimap <size_t, fs::path> FileCommander::FindSameSizeFiles() const
{
	multimap <size_t, fs::path> aFiles;

	for (const auto& aDir : myDirectories) {
		if (!fs::exists(aDir) || !fs::is_directory(aDir)) {
			throw std::logic_error(aDir.string() + " - invalid directory");
		}
		
		if (myScanLevel == false) {
			fs::directory_iterator aDirectoryIterator (aDir);
			for (const auto& anEntry : aDirectoryIterator) {
				if (std::count (myExcludeDirectories.begin(), myExcludeDirectories.end(), anEntry) > 0) {
					continue;
				}
				else if (fs::is_regular_file(anEntry.path())) {
					const auto aSize = fs::file_size(anEntry);
					if (aSize >= myMinFileSize) {
						aFiles.emplace(aSize, anEntry);
					}
				}
			}
		}
		else {
			fs::recursive_directory_iterator aDirectoryIterator (aDir);
			for (const auto& anEntry : aDirectoryIterator) {
				if (std::count (myExcludeDirectories.begin(), myExcludeDirectories.end(), anEntry) > 0) {
					aDirectoryIterator.no_push();
				}
				else if (fs::is_regular_file(anEntry.path())) {
					const auto aSize = fs::file_size(anEntry);
					if (aSize >= myMinFileSize) {
						aFiles.emplace(aSize, anEntry);
					}
				}
			}
		}
		
	}
	LeaveUnique (aFiles);

	return aFiles;
}

