#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include "../../../libs/Multimedia/AssetManager/AssetManager.hpp"

namespace fs = std::filesystem;

/* This tool generates embedded asset files from a given asset directory.
 * Usage: ./generate_embedded_assets <asset_dir> <output_file>
 * It creates two files: <output_file>.hpp and <output_file>.cpp
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <asset_dir> <output_file>" << std::endl;
        return 1;
    }

    std::string asset_dir = argv[1];
    std::string output_file = argv[2];

    std::unordered_map<std::string, std::vector<unsigned char>> assets;

    for (const auto& entry :
         fs::recursive_directory_iterator(asset_dir)) {
        if (entry.is_regular_file()) {
            std::string rel_path =
                fs::relative(entry.path(), asset_dir).string();

            std::replace(rel_path.begin(), rel_path.end(), '\\', '/');

            std::ifstream file(entry.path(), std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open file: "
                          << entry.path() << std::endl;
                return 1;
            }
            std::vector<unsigned char> data(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            assets[rel_path] = data;
        }
    }

    /* Generate .hpp */
    {
        std::ofstream f(output_file + ".hpp");
        if (!f) {
            std::cerr << "Failed to create " << output_file
                      << ".hpp" << std::endl;
            return 1;
        }
        f << "#ifndef EMBEDDED_ASSETS_HPP_\n";
        f << "#define EMBEDDED_ASSETS_HPP_\n\n";
        f << "#include \"../../libs/Multimedia/"
             "AssetManager.hpp\"\n";
        f << "#include <unordered_map>\n";
        f << "#include <string>\n";
        f << "#include <vector>\n\n";
        f << "namespace assets {\n\n";
        f << "extern std::unordered_map<std::string, AssetData> "
             "embeddedAssets;\n\n";
        f << "} // namespace assets\n\n";
        f << "#endif // EMBEDDED_ASSETS_HPP_\n";
    }

    /* Generate .cpp */
    {
        std::ofstream f(output_file + ".cpp");
        if (!f) {
            std::cerr << "Failed to create " << output_file
                      << ".cpp" << std::endl;
            return 1;
        }
        f << "#include \"EmbeddedAssets.hpp\"\n\n";
        f << "namespace assets {\n\n";
        f << "std::unordered_map<std::string, AssetData> embeddedAssets = {\n";
        for (const auto& [path, data] : assets) {
            f << "    {\"" << path << "\", {\n";
            f << "        {";
            for (size_t i = 0; i < data.size(); ++i) {
                if (i % 16 == 0) f << "\n            ";
                char buf[10];
                std::snprintf(buf, sizeof(buf), "0x%02x",
                              (unsigned char)data[i]);
                f << buf;
                if (i < data.size() - 1) f << ", ";
            }
            f << "\n        },\n";
            f << "        " << data.size() << "\n";
            f << "    }},\n";
        }
        f << "};\n\n";
        f << "} // namespace assets\n";
    }

    return 0;
}
