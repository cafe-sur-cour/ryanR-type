---
sidebar_position: 4
---

# Accessibility

This project includes accessibility options. This page documents supported features and how to configure them.

## Features
- Color-blind shaders: `deuteranopia`, `protanopia`, `tritanopia`, `highcontrast`. Toggleable in options.
- UI scale: adjustable via `accessibility.json` (values: `1.0`, `1.25`, `1.5`, ...)
- Keyboard-only controls: ensure all menu actions have keyboard bindings.
- Controller mode available.
- Key remapping.

## Configuration
Example `accessibility.json`:

```json
{
  "colorFilter": "deuteranopia",
  "uiScale": 1.25,
  "highContrast": false
}
```

## Testing
- Add a11y checks to QA: keyboard navigation, UI scale regression, shader toggles validation.
