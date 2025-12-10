/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpatialGrid
*/

#ifndef SPATIALGRID_HPP_
#define SPATIALGRID_HPP_

#include <vector>
#include <unordered_set>
#include <memory>
#include <cmath>
#include <cstddef>
#include "../types/FRect.hpp"
#include "../types/Vector2f.hpp"

namespace ecs {

using Entity = size_t;

class SpatialGrid {
    public:
        SpatialGrid(float worldWidth = 1920.0f, float worldHeight = 1080.0f, float cellSize = 128.0f);
        ~SpatialGrid() = default;

        void clear();
        void insert(Entity entityId, const math::FRect& bounds);
        std::vector<Entity> query(const math::FRect& bounds) const;
        std::vector<std::pair<Entity, Entity>> getPotentialPairs() const;
        void setCellSize(float cellSize);

        float getCellSize() const { return _cellSize; }
        size_t getNumCols() const { return _numCols; }
        size_t getNumRows() const { return _numRows; }

    private:
        size_t getCellIndex(float x, float y) const;
        std::vector<size_t> getCellIndices(const math::FRect& bounds) const;

        float _worldWidth;
        float _worldHeight;
        float _cellSize;
        size_t _numCols;
        size_t _numRows;
        std::vector<std::vector<Entity>> _cells;
};

}

#endif /* !SPATIALGRID_HPP_ */
