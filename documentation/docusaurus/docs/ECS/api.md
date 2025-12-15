---
sidebar_position: 2
---

# ECS API Reference

This page documents common operations for the project's Entity-Component-System (ECS).

## Core operations
- `createEntity()` → returns `EntityId`
- `destroyEntity(EntityId)`
- `addComponent<Entity, ComponentType>(entity, component)`
- `removeComponent<ComponentType>(entity)`
- `getComponent<ComponentType>(entity)` → optional / pointer

## Queries and Systems
- `view<Components...>()` returns iterable of entities matching all `Components`.
- Systems should operate on `views` and avoid mutating entity lists while iterating; use command buffers to enqueue create/destroy operations.

## Network sync components
- Components used for network sync must include a `dirty` flag and a small `version` integer to allow delta encoding and reconciliation.

## Threading & Safety
- ECS reads can run in parallel for disjoint component sets. Writes must be serialized or use job/command-buffer patterns.

## Serialization
- Provide `serialize(entity)`/`deserialize(data)` for snapshotting and network broadcast.
