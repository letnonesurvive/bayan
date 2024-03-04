#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <iostream>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

void set_bulk(size_t bulk) {
    //std::cout << "bulk size is " << bulk << std::endl;
}

int main(int argc, const char* argv[]) 
{
    try {
        po::options_description aDesc{ "Options" };

        aDesc.add_options()
            ("help,h", "This screen")
            ("path,d", po::value<vector<fs::path>>()->multitoken()->composing(), "directory path to search")
            ("level", po::value<bool>(), "scan level : 0 - only specified directory, 1 - include nested directories")
            ("size", po::value<size_t>()->default_value(1), "minimal file size in bytes")
            ("S", po::value<size_t>(), "block size used to read files")
            ("hash", po::value<string>(), "hash alghoritm (crc32, md5)");

        po::variables_map aVariableMap;
        po::store(parse_command_line(argc, argv, aDesc), aVariableMap);
        //po::notify(vm);

        if (aVariableMap.count("help")) {
            cout << aDesc << '\n';
        }
        if (aVariableMap.count ("directory_path")) {
            auto aPathVector = aVariableMap["directory_path"].as<vector<fs::path>>();
            for (auto aPath : aPathVector) {
                if (!fs::exists (aPath)) {
                    cout << "Directory does not exist: " << aPath << std::endl;
                    return 1;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}