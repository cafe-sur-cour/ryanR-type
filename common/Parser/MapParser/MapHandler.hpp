/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapHandler
*/

#ifndef MAPHANDLER_HPP_
#define MAPHANDLER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

struct MapData {
    int index;
    std::string name;
    std::string filePath;
    nlohmann::json jsonData;
};

class MapHandler {
    public:
        MapHandler();
        ~MapHandler();

        void parseAllLevels(const std::string& directoryPath);

        const MapData& getCurrentMap() const;

        nlohmann::json getCurrentMapJson() const;

        size_t getCurrentMapIndex() const;

        size_t getTotalMaps() const;

        bool hasMaps() const;

        bool isLastMap() const;

        bool advanceToNextMap();

        bool hasCompletedAllMaps() const;

        void resetToFirstMap();

        const std::vector<MapData>& getAllMaps() const;

    private:
        std::vector<MapData> _maps;
        size_t _currentMapIndex;
        bool _completedAllMaps;

        void sortMapsByIndex();
};

#endif /* !MAPHANDLER_HPP_ */
