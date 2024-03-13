#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.h>

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

namespace {

bool IsDifferentFiles (const string& theFile1, const string& theFile2, size_t theBlockSize, const IHashGenerator& theGenerator)
{
    bool aRes = false;
    FileReader aR1, aR2;

    aR1.OpenFile (theFile1);
    aR2.OpenFile (theFile2);
    
    string aB1;
    string aB2;

    while (aR1.ReadBlock (aB1, theBlockSize) && aR2.ReadBlock (aB2, theBlockSize)) {
        const auto aHash1 = theGenerator.GenerateHash (aB1);
        const auto aHash2 = theGenerator.GenerateHash (aB2);
        if (aHash1 != aHash2) {
            aRes = true;
            break;
        }
    }
    return aRes;
}

vector<set<string>> ReadAndSplit (const vector<string>& theFiles, const size_t theBlockSize, const IHashGenerator& theGenerator)
{
    vector<set<string>> aRes;

    for (size_t i = 0; i < theFiles.size(); ++i) {
        set<string> aSet;
        aSet.insert (theFiles[i]);
        for (size_t j = i + 1; j < theFiles.size(); ++j) {
            if (IsDifferentFiles (theFiles[i], theFiles[j], theBlockSize, theGenerator)) {
                break;
            }
            aSet.insert (theFiles[j]); // same files
        }
        aRes.push_back (aSet);
        i = i + aSet.size() - 1;
    }
    return aRes;
}

}

void PrintFiles (const vector<set<string>>& theFiles)
{
    for (const auto& aSetOfSameFiles : theFiles) {
        for (const auto& aFile : aSetOfSameFiles) {
            cout << aFile << endl;
        }
        cout << endl;
    }
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
            ("mask", po::value<string>()->default_value(".+"), "regular expresion to search files")
            ("hash", po::value<string>()->default_value("md5"), "hash alghoritm (crc32, md5)");

        po::variables_map aVariableMap;
        po::store (parse_command_line (argc, argv, aDesc), aVariableMap);

        FileCommander aCommander;
        IHashGenerator* aHashGenerator;
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
        if (aVariableMap.count("mask")) {
            const auto aMask = aVariableMap["mask"].as<string>();
            aCommander.SetMask (aMask);
        }
        if (aVariableMap.count ("hash")) {
            if (aVariableMap["hash"].as<string>() == "md5") {
                aHashGenerator = new MD5Service();
            }
            else if (aVariableMap["hash"].as<string>() == "crc32") {
                aHashGenerator = new CRC32Service();
            }
        }

        auto aFilesMap = aCommander.FindSameSizeFiles();

        for (auto& it = aFilesMap.begin(); it != aFilesMap.end(); ++it) {
            size_t aSize = it->first;
            vector<string> aFiles;

            while (it != aFilesMap.end() && it->first == aSize) {
                aFiles.push_back (it->second.string());
                ++it;
            }

            auto aSplitedFiles = ReadAndSplit (aFiles, aBlockSize, *aHashGenerator);
            PrintFiles (aSplitedFiles);

            if (it == aFilesMap.end()) {
                break;
            }
        }

    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}