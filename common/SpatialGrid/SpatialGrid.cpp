/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpatialGrid
*/

#include "SpatialGrid.hpp"
#include <algorithm>
#include <unordered_set>

namespace ecs {

SpatialGrid::SpatialGrid(
    float worldWidth, float worldHeight, float cellSize, float padding
) : _worldWidth(worldWidth), _worldHeight(worldHeight),
    _cellSize(cellSize), _padding(padding) {
    float totalWidth = _worldWidth + 2 * _padding;
    float totalHeight = _worldHeight + 2 * _padding;
    _numCols = static_cast<size_t>(std::ceil(totalWidth / _cellSize)) + 1;
    _numRows = static_cast<size_t>(std::ceil(totalHeight / _cellSize)) + 1;
    _cells.resize(_numCols * _numRows);
}

void SpatialGrid::clear() {
    for (auto& cell : _cells) {
        cell.clear();
    }
}

void SpatialGrid::insert(Entity entityId, const math::FRect& bounds) {
    auto indices = getCellIndices(bounds);
    for (size_t idx : indices) {
        if (idx < _cells.size()) {
            _cells[idx].push_back(entityId);
        }
    }
}

std::vector<Entity> SpatialGrid::query(const math::FRect& bounds) const {
    std::unordered_set<Entity> uniqueEntities;
    auto indices = getCellIndices(bounds);

    for (size_t idx : indices) {
        if (idx < _cells.size()) {
            for (Entity e : _cells[idx]) {
                uniqueEntities.insert(e);
            }
        }
    }

    return std::vector<Entity>(uniqueEntities.begin(), uniqueEntities.end());
}

std::vector<std::pair<Entity, Entity>> SpatialGrid::getPotentialPairs() const {
    std::unordered_set<uint64_t> seenPairs;
    std::vector<std::pair<Entity, Entity>> pairs;

    for (const auto& cell : _cells) {
        size_t cellSize = cell.size();
        for (size_t i = 0; i < cellSize; ++i) {
            for (size_t j = i + 1; j < cellSize; ++j) {
                Entity e1 = cell[i];
                Entity e2 = cell[j];

                if (e1 > e2) std::swap(e1, e2);

                uint64_t pairKey = (
                    static_cast<uint64_t>(e1) << 32
                ) | static_cast<uint64_t>(e2);

                if (seenPairs.find(pairKey) == seenPairs.end()) {
                    seenPairs.insert(pairKey);
                    pairs.emplace_back(e1, e2);
                }
            }
        }
    }

    return pairs;
}

void SpatialGrid::setCellSize(float cellSize) {
    _cellSize = cellSize;
    float totalWidth = _worldWidth + 2 * _padding;
    float totalHeight = _worldHeight + 2 * _padding;
    _numCols = static_cast<size_t>(std::ceil(totalWidth / _cellSize)) + 1;
    _numRows = static_cast<size_t>(std::ceil(totalHeight / _cellSize)) + 1;
    _cells.clear();
    _cells.resize(_numCols * _numRows);
}

size_t SpatialGrid::getCellIndex(float x, float y) const {
    int col = static_cast<int>(std::floor((x + _padding) / _cellSize));
    int row = static_cast<int>(std::floor((y + _padding) / _cellSize));

    col = std::max(0, std::min(col, static_cast<int>(_numCols) - 1));
    row = std::max(0, std::min(row, static_cast<int>(_numRows) - 1));

    return static_cast<size_t>(row) * _numCols + static_cast<size_t>(col);
}

std::vector<size_t> SpatialGrid::getCellIndices(const math::FRect& bounds) const {
    std::vector<size_t> indices;

    float left = bounds.getLeft();
    float top = bounds.getTop();
    float right = left + bounds.getWidth();
    float bottom = top + bounds.getHeight();

    int minCol = static_cast<int>(std::floor((left + _padding) / _cellSize));
    int maxCol = static_cast<int>(std::floor((right + _padding) / _cellSize));
    int minRow = static_cast<int>(std::floor((top + _padding) / _cellSize));
    int maxRow = static_cast<int>(std::floor((bottom + _padding) / _cellSize));

    minCol = std::max(0, minCol);
    maxCol = std::min(maxCol, static_cast<int>(_numCols) - 1);
    minRow = std::max(0, minRow);
    maxRow = std::min(maxRow, static_cast<int>(_numRows) - 1);

    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            indices.push_back(static_cast<size_t>(row) * _numCols + static_cast<size_t>(col));
        }
    }

    return indices;
}

}
