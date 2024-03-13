#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <vector>
#include <string>
#include <map>
#include <set>


namespace fs = boost::filesystem;

class FileCommander
{
public:
	FileCommander();

	void SetMinFileSize (size_t theMinFileSize);
	void SetScanLevel (bool theScanLevel);
	void SetDirectories (std::vector <fs::path> theDirs);
	void SetExcludeDirectories (std::vector <fs::path> theExcludeDirs);
	void SetMask(const std::string& theMask);
	std::multimap <size_t, fs::path> FindSameSizeFiles() const;
	
private:

	template<bool isRecursive>
	void TraverseDirectory (const fs::path& theDir, std::multimap <size_t, fs::path>& aFiles) const;

	size_t myMinFileSize;
	bool myScanLevel;
	std::vector <fs::path> myDirectories;
	std::vector <fs::path> myExcludeDirectories;
	boost::regex myMask;
};

template <bool isRecursive>
void FileCommander::TraverseDirectory (const fs::path& theDir, std::multimap <size_t, fs::path>& aFiles) const
{
	typename std::conditional <isRecursive, 
							   fs::recursive_directory_iterator, 
							   fs::directory_iterator>::type  aDirectoryIterator (theDir);

	for (const auto& anEntry : aDirectoryIterator) {
		if (std::count(myExcludeDirectories.begin(), myExcludeDirectories.end(), anEntry) > 0) {
			if constexpr (std::is_same_v<fs::recursive_directory_iterator, decltype (aDirectoryIterator)>) {
				aDirectoryIterator.no_push();
			} else {
				continue;
			}
		}
		if (!boost::regex_match(anEntry.path().string(), boost::smatch(), myMask)) {
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