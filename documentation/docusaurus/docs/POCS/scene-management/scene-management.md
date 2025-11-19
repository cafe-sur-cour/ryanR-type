# Scene Management Documentation

This document covers our Game State Machine (GSM) implementation for managing different game scenes and states.

## Overview

The Scene Management system provides a clean architecture for handling different game states (Menu, Game, Pause, etc.) with integrated ECS support. It allows for proper state transitions, resource management, and modular game logic organization.

## Architecture

### Core Components
- **GameStateMachine**: Central manager for state transitions
- **GameState**: Abstract base class for all game states
- **State Classes**: MenuState, GamePlayState, etc.
- **ECS Integration**: Each state can manage its own entities and systems

### Key Features
- Clean state lifecycle (enter/update/render/exit)
- Automatic resource cleanup between states
- ECS integration for entity management per state
- Easy extensibility for new game states

## Implementation

### Basic Usage
```cpp
GameStateMachine gsm;

// Start with menu
gsm.changeState(std::make_shared<MenuState>());

// Main game loop
while (running) {
    gsm.update();
    gsm.render();
}
```

### State Structure
Each state implements:
- `enter()`: Initialize state resources and entities
- `update()`: Handle state logic and transitions
- `render()`: Draw state-specific content
- `exit()`: Cleanup resources

## Benefits

- **Modularity**: Each state is self-contained
- **Maintainability**: Clear separation of game logic
- **Extensibility**: Easy to add new states
- **Resource Management**: Automatic cleanup prevents memory leaks

## Integration with ECS

The GSM works seamlessly with our Component-Driven ECS:
- States can create/destroy entities as needed
- Systems are managed per state
- Entity filtering with tags for performance
- Clean transitions between different game contexts
