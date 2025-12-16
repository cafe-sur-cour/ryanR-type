---
sidebar_position: 4
---

# Accessibility

This document provides a concise, practical accessibility reference for the project. It lists the accessibility features present in the codebase, maps them to common disability categories, provides recommended configuration keys, gives developer integration points, and contains a QA checklist and a suggested roadmap for improvements.

## Quick summary

- Color-blind and high-contrast shaders exist in `assets/shaders/`.
- Saved accessibility values are present in `saves/accessibility.json`.
- Input remapping and controller support are implemented in `common/InputMapping/` and `client/initResourcesManager/GraphicalInputProvider.cpp`.
- Audio systems with separable channels exist at `client/systems/audio/`.

## Supported features (what the repository already provides)

- Color-blind shaders: `assets/shaders/deuteranopia.frag`, `protanopia.frag`, `tritanopia.frag`, `highcontrast.frag`.
- Saved accessibility data: `saves/accessibility.json` (contains `brightnessValue`, `colorBlindnessState`, `highContrastEnabled`).
- Key remapping: `common/InputMapping/` (types: `RemappableAction`, `RemappableKeyBinding`, manager: `InputMappingManager`).
- Controller/gamepad support: initialization and provider code in `client/initResourcesManager/GraphicalInputProvider.cpp`.
- Audio channelization: `client/systems/audio/MusicSystem.cpp` and `SoundSystem.cpp` allow separate handling of music and sound effects.

## Accessibility recommendations by disability category

### 1) Physical & Motor Disabilities

- Key remapping and customizable input bindings.
- Controller and gamepad support.
- Hold-to-activate options and adjustable input sensitivity to avoid accidental activation.

### 2) Audio & Visual Disabilities

- Color‑blind shaders and high‑contrast filters.
- Adjustable brightness and contrast values.

### 3) Mental & Cognitive Disabilities

- Adjustable UI scaling (and simplified layouts where appropriate).


```json
{
  "colorFilter": "off",             // off|deuteranopia|protanopia|tritanopia|highcontrast
  "brightness": 1.0,                 // number, multiplies scene brightness
  "highContrast": false,             // boolean
  "uiScale": 1.0,                    // number, UI scale multiplier
  "subtitles": true,                 // boolean
  "subtitleSize": 1.0,               // number, relative scale
  "reduceMotion": false,             // boolean
  "audio": {
    "musicVolume": 0.8,
    "sfxVolume": 0.9,
    "uiVolume": 1.0,
    "monoAudio": false
  },
  "controls": {
    "remapFile": "saves/keybinds.json"
  }
}
```