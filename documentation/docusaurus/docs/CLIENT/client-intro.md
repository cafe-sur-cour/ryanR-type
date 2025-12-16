---
sidebar_position: 1
---

# R-Type — Client Overview

This page gives a short, practical introduction to the R‑Type client component: what it contains, how to build and run it for development, and where to look for common features (input, rendering, audio, UI).

## Purpose

The client implements the graphical and audio front-end for the game: rendering, input handling, local state interpolation, UI, and audio playback. It connects to the server for multiplayer state and uses the project's common libraries for ECS, resource management and packets.

## Important client folders and files

- `client/` — main client source tree
	- `main.cpp`, `Core.cpp/.hpp` — application entry and lifecycle
	- `initResourcesManager/GraphicalInputProvider.cpp` — keyboard/mouse/gamepad initialization
	- `systems/` — client-side ECS systems (rendering, audio, input handling)
	- `components/` — rendering components (sprites, animations, text, health bars)
	- `ui/` — UI elements, navigation and managers

- `assets/` — shared assets used by the client (shaders, sprites, sounds, fonts)
	- `assets/shaders/` — color-blind and high-contrast shaders (e.g. `deuteranopia.frag`)

- `saves/` — persisted settings and user data (including `accessibility.json`, `keybinds.json`)

## Key features and where to find them

- Input remapping and mapping structures: `common/InputMapping/` (`InputMapping.hpp`, `InputMappingManager.hpp`, `InputAction.hpp`).
- Rendering systems: `client/systems/rendering/` (game zone, parallax, hitboxes, animations).
- Audio: `client/systems/audio/MusicSystem.cpp`, `SoundSystem.cpp`.
- Network/packets: `client/packet/` and `common/packet/` for serialization and default handlers.
