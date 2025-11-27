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
 * Usage: ./generate_embedded_assets <asset_dir> <output_dir>
 * It creates multiple files to avoid MSVC buffer overflow:
 * - <output_dir>/EmbeddedAssets.hpp (main header)
 * - <output_dir>/EmbeddedAssets.cpp (main map)
 * - <output_dir>/EmbeddedAsset_<n>.cpp (individual assets)
 */

std::string sanitizeSymbolName(const std::string& path) {
    std::string result = path;
    std::replace(result.begin(), result.end(), '/', '_');
    std::replace(result.begin(), result.end(), '\\', '_');
    std::replace(result.begin(), result.end(), '.', '_');
    std::replace(result.begin(), result.end(), '-', '_');
    std::replace(result.begin(), result.end(), ' ', '_');
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <asset_dir> <output_file>" << std::endl;
        return 1;
    }

    std::string asset_dir = argv[1];
    std::string output_file = argv[2];

    fs::path output_path(output_file);
    fs::path embedded_dir = output_path.parent_path() / "embeddedAssets";
    fs::create_directories(embedded_dir);

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

    std::cout << "Generating " << assets.size() << " embedded assets..." << std::endl;

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
             "AssetManager/AssetManager.hpp\"\n";
        f << "#include <unordered_map>\n";
        f << "#include <string>\n";
        f << "#include <vector>\n\n";
        f << "namespace assets {\n\n";

        // Declare extern variables for each asset
        for (const auto& [path, data] : assets) {
            std::string symbol = sanitizeSymbolName(path);
            f << "extern const unsigned char asset_" << symbol << "[];\n";
            f << "extern const size_t asset_" << symbol << "_size;\n";
        }
        f << "\n";

        f << "extern std::unordered_map<std::string, AssetData> "
             "embeddedAssets;\n\n";
        f << "} // namespace assets\n\n";
        f << "#endif // EMBEDDED_ASSETS_HPP_\n";
    }

    /* Generate individual .cpp files for each asset */
    int file_index = 0;
    for (const auto& [path, data] : assets) {
        std::string symbol = sanitizeSymbolName(path);
        std::string cpp_filename = (
            embedded_dir / ("EmbeddedAsset_" + std::to_string(file_index) + ".cpp")
        ).string();

        std::ofstream f(cpp_filename);
        if (!f) {
            std::cerr << "Failed to create " << cpp_filename << std::endl;
            return 1;
        }

        f << "#include \"../EmbeddedAssets.hpp\"\n\n";
        f << "namespace assets {\n\n";
        f << "const unsigned char asset_" << symbol << "[] = {";

        for (size_t i = 0; i < data.size(); ++i) {
            if (i % 16 == 0) f << "\n    ";
            char buf[10];
            std::snprintf(buf, sizeof(buf), "0x%02x", (unsigned char)data[i]);
            f << buf;
            if (i < data.size() - 1) f << ", ";
        }

        f << "\n};\n\n";
        f << "const size_t asset_" << symbol << "_size = " << data.size() << ";\n\n";
        f << "} // namespace assets\n";

        file_index++;
    }

    std::cout << "Generated " << file_index << " individual asset files." << std::endl;

    /* Generate a simple list file with the count */
    {
        std::ofstream f(output_file + "_count.txt");
        if (!f) {
            std::cerr << "Failed to create count file" << std::endl;
            return 1;
        }
        f << file_index << "\n";
    }

    /* Generate main .cpp with the map */
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
            std::string symbol = sanitizeSymbolName(path);
            f << "    {\"" << path << "\", {\n";
            f << "        std::vector<unsigned char>(asset_" << symbol
              << ", asset_" << symbol << " + asset_" << symbol << "_size),\n";
            f << "        asset_" << symbol << "_size\n";
            f << "    }},\n";
        }
        f << "};\n\n";
        f << "} // namespace assets\n";
    }

    std::cout << "Asset generation complete!" << std::endl;
    return 0;
}
