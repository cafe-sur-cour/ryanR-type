---
sidebar_position: 3
---

# Storage & Persistence

This document describes how game state and user configuration are persisted, formats used and recommended operational practices.

## Scope
- Local client files: `saves/` (`settings.json`, `keybinds.json`, `accessibility.json`)
- Server persistence: (if used) snapshots, match records, leaderboards

## Client save files
- `settings.json`: UI and runtime configuration (resolution, audio levels)
- `keybinds.json`: mapping of input actions to keys
- `accessibility.json`: accessibility toggles (contrast, color filters, UI scale)

Provide JSON schema examples in this repo so tools and editors can validate files.

Example `settings.json` schema (excerpt):

```json
{
  "version": 1,
  "graphics": { "resolution": "1280x720", "vsync": true },
  "audio": { "master": 0.8, "music": 0.6, "effects": 0.9 }
}
```

## Schemas & Examples

You can validate and inspect the save files using the provided schemas and examples below.

- Schemas:
  - [settings schema](./schemas/settings.schema.json)
  - [keybinds schema](./schemas/keybinds.schema.json)
  - [accessibility schema](./schemas/accessibility.schema.json)

- Examples (derived from `saves/`):
  - [settings example](./examples/settings.example.json)
  - [keybinds example](./examples/keybinds.example.json)
  - [accessibility example](./examples/accessibility.example.json)

Validation quickstart (requires `ajv-cli`):

```bash
# Install once (nodejs required)
npm install -g ajv-cli

# Validate a save file against the schema
ajv validate -s documentation/docusaurus/docs/STORAGE/schemas/settings.schema.json -d saves/settings.json --errors=text
```
