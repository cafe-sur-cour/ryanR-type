#!/usr/bin/env python3

import os
import sys

def generate_asset_header(asset_dir, output_file):
    assets = {}
    for root, dirs, files in os.walk(asset_dir):
        for file in files:
            filepath = os.path.join(root, file)
            rel_path = os.path.relpath(filepath, asset_dir)
            with open(filepath, 'rb') as f:
                data = f.read()
            assets[rel_path] = data

    with open(output_file + '.hpp', 'w') as f:
        f.write('#ifndef EMBEDDED_ASSETS_HPP_\n')
        f.write('#define EMBEDDED_ASSETS_HPP_\n\n')
        f.write('#include "../../../client/graphicals/assets/AssetManager.hpp"\n')
        f.write('#include <unordered_map>\n')
        f.write('#include <string>\n')
        f.write('#include <vector>\n\n')
        f.write('namespace assets {\n\n')
        f.write('extern std::unordered_map<std::string, AssetData> embeddedAssets;\n\n')
        f.write('} // namespace assets\n\n')
        f.write('#endif // EMBEDDED_ASSETS_HPP_\n')

    with open(output_file + '.cpp', 'w') as f:
        f.write('#include "EmbeddedAssets.hpp"\n\n')
        f.write('namespace assets {\n\n')
        f.write('std::unordered_map<std::string, AssetData> embeddedAssets = {\n')
        for path, data in assets.items():
            f.write('    {"' + path.replace('\\', '/') + '", {\n')
            f.write('        {' + ', '.join(f'0x{b:02x}' for b in data) + '},\n')
            f.write('        ' + str(len(data)) + '\n')
            f.write('    }},\n')
        f.write('};\n\n')
        f.write('} // namespace assets\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python generate_embedded_assets.py <asset_dir> <output_header>")
        sys.exit(1)
    asset_dir = sys.argv[1]
    output_file = sys.argv[2]
    generate_asset_header(asset_dir, output_file)
