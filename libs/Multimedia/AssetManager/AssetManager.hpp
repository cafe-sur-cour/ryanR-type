/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AssetManager
*/

#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_

#include <string>
#include <vector>
#include <memory>

namespace assets {

struct AssetData {
    std::vector<unsigned char> data;
    size_t size;
};

class AssetManager {
    public:
        AssetManager();
        ~AssetManager() = default;

        std::shared_ptr<AssetData> getAsset(const std::string& path);
};

}  // namespace assets

#endif /* !ASSETMANAGER_HPP_ */
