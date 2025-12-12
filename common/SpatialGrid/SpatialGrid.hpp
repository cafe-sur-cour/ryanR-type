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
#include "../constants.hpp"

namespace ecs {

using Entity = size_t;

class SpatialGrid {
    public:
        SpatialGrid(
            float worldWidth = constants::MAX_WIDTH,
            float worldHeight = constants::MAX_HEIGHT,
            float cellSize = constants::SPATIAL_GRID_CELL_SIZE,
            float padding = constants::SPATIAL_GRID_PADDING
        );
        ~SpatialGrid() = default;

        void clear();
        void insert(Entity entityId, const math::FRect& bounds);
        std::vector<Entity> query(const math::FRect& bounds) const;
        std::vector<std::pair<Entity, Entity>> getPotentialPairs() const;
        void setCellSize(float cellSize);
        void setOffset(float offsetX, float offsetY);

        float getCellSize() const { return _cellSize; }
        size_t getNumCols() const { return _numCols; }
        size_t getNumRows() const { return _numRows; }
        float getOffsetX() const { return _offsetX; }
        float getOffsetY() const { return _offsetY; }

    private:
        size_t getCellIndex(float x, float y) const;
        std::vector<size_t> getCellIndices(const math::FRect& bounds) const;

        float _worldWidth;
        float _worldHeight;
        float _cellSize;
        float _padding;
        float _offsetX;
        float _offsetY;
        size_t _numCols;
        size_t _numRows;
        std::vector<std::vector<Entity>> _cells;
};

}

#endif /* !SPATIALGRID_HPP_ */
