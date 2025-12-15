---
sidebar_position: 1
---

# Algorithms reference

Reference documentation for core algorithms used in R-Type: interpolation, collision detection, spatial partitioning and delta encoding.

## Interpolation & Client Prediction
- Linear interpolation (LERP) for position correction:


- Extrapolation for prediction: use last velocity sample to predict until server correction arrives.

## Spatial partitioning (SpatialGrid)
- Use a uniform grid (cell size = entity average size * 2) for O(1) expected insertion and O(k) neighbor queries where k is local density.

Pseudocode: insert(entity): cell = cellFor(entity.pos); cells[cell].push(entity)

## Collision detection
- Broad phase: query entities in same/adjacent grid cells
- Narrow phase: AABB or circle intersection tests depending on component type

## Delta Encoding
- Send only changed entity fields with a per-entity `dirty` flag. Use fixed-point scaled integers for positions to reduce size (e.g., scale by 1000 → 3 decimals).

## Bit-packing & Serialization
- Pack booleans and small enums into single bytes. Document field bit-layout per message in the UDP protocol spec.

## Complexity notes
- Physics + broad-phase: O(n + m) where n entities and m collisions reported.
