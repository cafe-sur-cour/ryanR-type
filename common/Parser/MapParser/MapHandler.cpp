/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapHandler
*/

#include "MapHandler.hpp"
#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <vector>
#include "../../constants.hpp"

MapHandler::MapHandler() : _currentMapIndex(0), _completedAllMaps(false) {
}

MapHandler::~MapHandler() {
}

void MapHandler::parseAllLevels(const std::string& directoryPath) {
    _maps.clear();
    _currentMapIndex = 0;
    _completedAllMaps = false;

    std::regex levelPattern("^level[0-9]+\\.json$");

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (!entry.is_regular_file())
                continue;

            std::string filename = entry.path().filename().string();

            if (!std::regex_match(filename, levelPattern))
                continue;

            std::ifstream file(entry.path());
            if (!file.is_open()) {
                std::cerr << "[MapHandler] Failed to open file: " << entry.path() << std::endl;
                continue;
            }

            try {
                nlohmann::json jsonData = nlohmann::json::parse(file);

                MapData mapData;
                mapData.filePath = entry.path().string();
                mapData.jsonData = jsonData;

                if (jsonData.contains(constants::INDEX_FIELD)) {
                    mapData.index = jsonData[constants::INDEX_FIELD].get<int>();
                } else {
                    std::regex indexExtractor("level([0-9]+)\\.json");
                    std::smatch match;
                    if (std::regex_match(filename, match, indexExtractor)) {
                        mapData.index = std::stoi(match[1].str());
                    } else {
                        mapData.index = 0;
                    }
                }

                if (jsonData.contains(constants::NAME_FIELD)) {
                    mapData.name = jsonData[constants::NAME_FIELD].get<std::string>();
                } else {
                    mapData.name = "Level " + std::to_string(mapData.index);
                }

                _maps.push_back(mapData);
            } catch (const nlohmann::json::exception& e) {
                std::cerr << "[MapHandler] JSON parse error in " << entry.path()
                          << ": " << e.what() << std::endl;
            }

            file.close();
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[MapHandler] Filesystem error: " << e.what() << std::endl;
    }

    sortMapsByIndex();
}

void MapHandler::sortMapsByIndex() {
    std::sort(_maps.begin(), _maps.end(),
        [](const MapData& a, const MapData& b) {
            return a.index < b.index;
        });
}

const MapData& MapHandler::getCurrentMap() const {
    static MapData emptyMap;
    if (_maps.empty())
        return emptyMap;
    return _maps[_currentMapIndex];
}

nlohmann::json MapHandler::getCurrentMapJson() const {
    if (_maps.empty())
        return nlohmann::json();
    return _maps[_currentMapIndex].jsonData;
}

size_t MapHandler::getCurrentMapIndex() const {
    return _currentMapIndex;
}

size_t MapHandler::getTotalMaps() const {
    return _maps.size();
}

bool MapHandler::hasMaps() const {
    return !_maps.empty();
}

bool MapHandler::isLastMap() const {
    if (_maps.empty())
        return true;
    return _currentMapIndex >= _maps.size() - 1;
}

bool MapHandler::advanceToNextMap() {
    if (_maps.empty())
        return false;

    if (_currentMapIndex < _maps.size() - 1) {
        _currentMapIndex++;
        return true;
    }

    _completedAllMaps = true;
    return false;
}

bool MapHandler::hasCompletedAllMaps() const {
    return _completedAllMaps;
}

void MapHandler::resetToFirstMap() {
    _currentMapIndex = 0;
    _completedAllMaps = false;
}

const std::vector<MapData>& MapHandler::getAllMaps() const {
    return _maps;
}
