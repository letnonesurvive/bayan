#include "FileCommander.hpp"
#include "FilesReader.hpp"
#include "HashGenerator.hpp"

#include <stdexcept>
#include <memory>
#include <algorithm>
#include <set>
#include <type_traits>

using namespace std;

FileCommander::FileCommander() :
	myMinFileSize(0), 
	myScanLevel(false), 
	myDirectories{}, 
	myExcludeDirectories{}
{

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

void FileCommander::SetExcludeDirectories (std::vector<fs::path> theExcludeDirs)
{
	myExcludeDirectories = theExcludeDirs;
}

void FileCommander::SetMask (const std::string& theMask)
{
	myMask = theMask;
}

namespace {

void EraseUnique (multimap <size_t, fs::path>& theFiles)
{
	for (auto it = theFiles.begin(); it != theFiles.end();) {
		auto aLast = theFiles.upper_bound(it->first);
		if (std::distance(it, aLast) == 1) {
			it = theFiles.erase(it);
		}
		it = aLast;
	}
}

}

multimap <size_t, fs::path> FileCommander::FindSameSizeFiles() const
{
	multimap <size_t, fs::path> aFiles;

	for (const auto& aDir : myDirectories) {
		if (!fs::exists (aDir) || !fs::is_directory (aDir)) {
			throw std::logic_error (aDir.string() + " - invalid directory");
		}
		
		if (myScanLevel) {
			TraverseDirectory<true> (aDir, aFiles);
		} else {
			TraverseDirectory<false> (aDir, aFiles);
		}
		
	}
	EraseUnique (aFiles);

	return aFiles;
}


