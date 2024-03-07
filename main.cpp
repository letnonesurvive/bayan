#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <iostream>
#include <map>
#include <set>

#include "FileCommander.hpp"
#include "FilesReader.hpp"
#include "HashGenerator.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

vector<set<string>> MakeSplit (const vector<string>& theFiles)
{
    size_t aBlockSize = 5; // bad code 
    vector<set<string>> aRes;

    for (size_t i = 0; i < theFiles.size(); ++i) {
        FilesReader aReaderI, aReaderJ;
        set<string> aSet;
        aSet.insert(theFiles[i]);
        for (size_t j = i + 1; j < theFiles.size(); ++j) {
            aReaderI.OpenFile (theFiles[i]);
            aReaderJ.OpenFile (theFiles[j]);
            string theBuffI;
            string theBuffJ;
            bool IsDifferentFiles = false;
            while (aReaderI.ReadBlock (theBuffI, aBlockSize) && aReaderJ.ReadBlock (theBuffJ, aBlockSize)) {
                const auto aHashI = HashGenerator::Calculate (theBuffI);
                const auto aHashJ = HashGenerator::Calculate (theBuffJ);
                if (aHashI != aHashJ) {
                    IsDifferentFiles = true;
                    break;
                }
            }
            if (IsDifferentFiles) {
                break;
            }
            aSet.insert (theFiles[j]); // same files
        }
        aRes.push_back(aSet);
        i = i + aSet.size() - 1;
    }

    return aRes;
}

int main(int argc, const char* argv[]) 
{
    try {
        po::options_description aDesc{ "Options" };

        aDesc.add_options()
            ("help,h", "This screen")
            ("dir,d", po::value<vector<fs::path>>()->multitoken()->composing(), "directory path to search")
            ("ex_dir", po::value<vector<fs::path>>()->multitoken()->composing(), "exclude directories")
            ("level", po::value<bool>(), "scan level : 0 - only specified directory, 1 - include nested directories")
            ("file_size", po::value<size_t>()->default_value (1), "minimal file size in bytes")
            ("block_size", po::value<size_t>()->default_value (1), "block size used to read files")
            ("hash", po::value<string>(), "hash alghoritm (crc32, md5)");

        po::variables_map aVariableMap;
        po::store (parse_command_line (argc, argv, aDesc), aVariableMap);

        FileCommander aCommander;
        FilesReader aReader;

        size_t aBlockSize;

        if (aVariableMap.count ("help")) {
            cout << aDesc << '\n';
        }
        if (aVariableMap.count ("dir")) {
            const auto& aDirs = aVariableMap["dir"].as<vector<fs::path>>();
            aCommander.SetDirectories (aDirs);
        }
        if (aVariableMap.count ("ex_dir")) {
            const auto& anExcludeDirs = aVariableMap["ex_dir"].as<vector<fs::path>>();
            aCommander.SetExcludeDirectories (anExcludeDirs);
        }
        if (aVariableMap.count ("level")) {
            const auto aLevel = aVariableMap["level"].as<bool>();
            aCommander.SetScanLevel (aLevel);
        }
        if (aVariableMap.count ("file_size")) {
            const auto aFileSize = aVariableMap["file_size"].as<size_t>();
            aCommander.SetMinFileSize (aFileSize);
        }
        if (aVariableMap.count ("block_size")) {
            aBlockSize = aVariableMap["block_size"].as<size_t>();
        }
        if (aVariableMap.count ("hash")) {
            const auto aHash = aVariableMap["hash"].as<string>();
            aCommander.SetHash (aHash);
        }

        auto aFilesMap = aCommander.FindSameSizeFiles();

        for (auto it = aFilesMap.begin(); it != aFilesMap.end(); ++it) {
            size_t aSize = it->first;
            vector<string> aFiles;

            while (it != aFilesMap.end() && it->first == aSize) {
                aFiles.push_back (it->second.string());
                ++it;
            }

            auto aSplitedFiles = MakeSplit (aFiles);

            for (const auto& aSetOfSameFiles : aSplitedFiles) {
                for (const auto& aFile : aSetOfSameFiles) {
                    cout << aFile << endl;
                }
                cout << endl;
            }

            if (it == aFilesMap.end()) {
                break;
            }
        }

    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}