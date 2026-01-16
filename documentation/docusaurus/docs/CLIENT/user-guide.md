---
sidebar_position: 2
---

# Client User Guide

Complete guide for playing and using the R-Type game client.

## Installation

See the [Installation Guide](../INTRO/installation.md) for detailed setup instructions.

**Quick Start:**
```bash
# Download and run
./r-type_client -n YourName
```

## Main Menu

When you launch the client, you'll see the main menu with several options:

### Menu Options

1. **Play**: Start or join a game
2. **Settings**: Configure game options
3. **Credits**: View development team
4. **Quit**: Exit the game

## Playing the Game

### Creating a Lobby

1. Click **"Play"** from the main menu
2. Select **"Create Lobby"**
3. Share the generated 8-character lobby code with friends
4. Wait for players to join (up to 4 players)
5. Click **"Start Game"** when ready

**As Lobby Master:**
- You control when the game starts
- First player to create the lobby is the master
- Can kick players (if implemented)

### Joining a Lobby

1. Click **"Play"** from the main menu
2. Select **"Join Lobby"**
3. Enter the 8-character lobby code
4. Wait for the lobby master to start

**Tips:**
- Lobby codes are case-insensitive
- Codes contain only letters and numbers (no 0, O, I, L to avoid confusion)
- Codes expire after game completion

## Controls

### Default Keyboard Controls

| Action | Key |
|--------|-----|
| Move Up | `W` or `↑` |
| Move Down | `S` or `↓` |
| Move Left | `A` or `←` |
| Move Right | `D` or `→` |
| Shoot | `Space` |
| Special Attack | `Shift` |
| Pause | `Escape` or `P` |

### Gamepad Controls

| Action | Button |
|--------|--------|
| Move | Left Stick or D-Pad |
| Shoot | A / Cross |
| Special Attack | B / Circle |
| Pause | Start |

**Gamepad Support:**
- Xbox controllers
- PlayStation controllers
- Generic USB gamepads

### Remapping Controls

1. Go to **Settings** → **Controls**
2. Click on the action you want to remap
3. Press the new key/button
4. Changes are saved automatically

**Keybind File:** `saves/keybinds.json`

## Gameplay

### Objective

### Health System

- **Health Bar**: Displayed at the top of the screen
- **Lives**: Start with 3 lives (configurable)
- **Respawn**: Lose a life when health reaches zero
- **Game Over**: When all lives are lost

### Power-Ups

Power-ups are placed in levels (see `configs/entities/powerUp/`) and trigger server-side scripts on pickup. Common pickups:

- `force` (Force Pod): attaches to the player as a companion. Levels 1–4 change the player's projectile prefab and, at higher levels, the fire rate and special projectiles (level2 → `magnet`, level3 → `bombShot`, level4 → `beamShoot` and fire rate 0.3). Interaction functions: `OnInteract`, `ActivateOrDeactivateForce`, `addForceLevel`. Only one `force` of type `force` may be equipped per player.
- `flyingForce`: small companion shooter that follows the player. Up to 2 can be equipped. Provides continuous fire using `basic_shot` (configured by `ShootingStatsComponent` in the prefab).
- `powerUpAddForceLevel`: consumes itself to call `addForceLevel` on the player (increments force level, caps at 4).
- `powerUpAddLife`: grants an extra life via the `AddLife` interaction mapping.
- `powerUpSpeed`: instantly increases player speed by +200 (see `powerUpSpeed.lua`).
- `powerUpChargeTime`: reduces the player's charge/reload time by 10% (min 0.1s) when picked up.

Notes:
- Power-up definitions and visuals are in `configs/entities/powerUp/*.json` and associated scripts in `configs/scripts/`.
- Pickup behavior (instant consume vs attach) is implemented server-side via the `ScriptingComponent`.

### Force Pod (details)

The Force Pod family (`force`, `flyingForce`) behaves as follows:
- `force`: attachable companion. When attached it follows the player and modifies shooting behavior according to its `level`. Can be activated/deactivated (see in-game control). Script: `configs/scripts/force.lua`.
- `flyingForce`: autonomous companion shooter. Automatically positions relative to the player and fires using its `ProjectilePrefabComponent`. Script: `configs/scripts/flyingForce.lua`.

Controls:
- Auto-follow by default.
- Hold `Shift` to lock position (if supported by UI).

### Scoring

Points are awarded for:
- Destroying enemies: 100-1000 points
- Boss defeat: 10,000+ points
- Power-up collection: 500 points
- Combo multiplier: Up to 4x

**High Scores:**
- Saved locally in `saves/scores.json`
- Displayed on main menu
- Track personal bests

## Multiplayer

### Cooperative Play

- **Up to 4 players** per game
- Shared score pool
- Respawn at checkpoints
- Friendly fire: OFF (configurable)

### Communication

- **Text chat**: Press `T` to open chat
- **Quick messages**: Press `F1-F4` for preset messages
  - F1: "Help!"
  - F2: "Follow me"
  - F3: "Nice!"
  - F4: "Sorry"

### Connection Quality

Indicator in top-right corner:
- 🟢 **Green**: Good (&lt;50ms)
- 🟡 **Yellow**: Fair (50-100ms)
- 🔴 **Red**: Poor (&gt;100ms)
- ⚫ **Gray**: Disconnected

**Network Issues:**
- Client uses prediction to smooth gameplay
- May see rubber-banding with high ping
- Server is authoritative (prevents cheating)

## Settings

### Graphics

- **Resolution**: Window size selection
- **Fullscreen**: Toggle fullscreen mode
- **VSync**: Vertical synchronization
- **FPS Limit**: Cap frame rate (30, 60, 120, unlimited)

### Audio

- **Master Volume**: Overall volume (0-100%)
- **Music Volume**: Background music
- **SFX Volume**: Sound effects
- **Mute**: Quick mute toggle

### Accessibility

See [Accessibility Features](#accessibility-features) for detailed options.

- **Color Blind Modes**
- **High Contrast Mode**
- **UI Scaling**
- **Alternative Damage Indicators**

### Network

- **Server IP**: Default connection address
- **Server Port**: Default connection port
- **Auto-reconnect**: Attempt to reconnect on disconnect
- **Interpolation**: Client-side prediction amount

## Accessibility Features

R-Type includes comprehensive accessibility options.

### Color Blind Modes

**Available Modes:**
- Normal (Default)
- Deuteranopia (Red-Green)
- Protanopia (Red-Green)
- Tritanopia (Blue-Yellow)

**How to Enable:**
1. Settings → Accessibility
2. Select color blind mode
3. Shader applied in real-time

### High Contrast Mode

Increases visibility of game elements:
- Brighter sprites
- Enhanced outlines
- Clearer UI elements

### UI Scaling

Adjust interface size:
- **Small**: 80% scale
- **Normal**: 100% scale (default)
- **Large**: 125% scale
- **Extra Large**: 150% scale

### Alternative Health Indicators

- **Numeric**: Show exact HP numbers
- **Audio Cues**: Low health warning sound
- **Screen Flash**: Health loss indicator

### Configurable Controls

- Full key remapping
- Mouse support for menu navigation
- Gamepad with customizable dead zones

### Configuration File

Settings saved in: `saves/accessibility.json`

```json
{
  "colorBlindMode": "deuteranopia",
  "highContrast": true,
  "uiScale": 1.25,
  "showNumericHealth": true,
  "audioHealthWarning": true
}
```

## Pause Menu

Press `Escape` or `P` during gameplay:

- **Resume**: Continue playing
- **Settings**: Adjust options mid-game
- **Controls**: View control scheme
- **Quit to Menu**: Return to main menu
- **Quit Game**: Exit application

**Note:** In multiplayer, game continues while you're paused.

## Performance Optimization

### Low FPS Issues

If experiencing low frame rate:

1. **Lower Resolution**: Settings → Graphics → Resolution
2. **Disable VSync**: May improve responsiveness
3. **Close Background Apps**: Free up system resources
4. **Update Drivers**: Ensure graphics drivers are current

### High Ping

To reduce network latency:

1. Use wired Ethernet connection
2. Close bandwidth-heavy applications
3. Connect to geographically closer servers
4. Check firewall isn't blocking UDP port

### System Monitor

Press `F3` to toggle debug overlay (if enabled):
- FPS counter
- Ping to server
- Entity count
- Memory usage

## Troubleshooting

### Game Won't Start

```bash
# Check log files
cat logs/client.log

# Verify OpenGL support
glxinfo | grep "OpenGL version"  # Linux

# Run with debug flag
./r-type_client -d
```

### Cannot Connect to Server

1. Verify server is running
2. Check IP and port are correct
3. Disable firewall temporarily
4. Try localhost: `./r-type_client -i 127.0.0.1`

### Graphics Issues

- **Black screen**: Update graphics drivers
- **Missing textures**: Verify assets folder exists
- **Flickering**: Try disabling VSync

### Audio Issues

- **No sound**: Check system volume and game settings
- **Crackling**: Update audio drivers
- **Desync**: Restart the client

### Lobby Issues

- **Invalid lobby code**: Codes are case-sensitive, check for typos
- **Lobby full**: Maximum 4 players per lobby
- **Cannot create lobby**: Server may be at capacity

For more help, see [Troubleshooting Guide](../INTRO/troubleshooting.md).

## Advanced Features

### Replay System

Games are automatically recorded:

**Location:** `saves/replays/`

**Playing Replays:**
1. Main Menu → Replays
2. Select a saved replay
3. Use controls to pause/rewind

**Replay Controls:**
- `Space`: Pause/Resume
- `←/→`: Rewind/Fast-forward
- `Escape`: Exit replay

### Statistics

View your game statistics:
- Total games played
- Win/loss ratio
- Highest score
- Total enemies defeated
- Total playtime

**Access:** Main Menu → Statistics

### Profiles

Create multiple player profiles:
- Separate settings per profile
- Individual high scores
- Custom keybindings

**Manage:** Main Menu → Profiles

## Tips & Strategies

### Beginner Tips

1. **Stay Mobile**: Constant movement helps avoid enemy fire
2. **Power-Up Priority**: Focus on weapon upgrades early
3. **Force Pod Usage**: Position it to cover your weak spots
4. **Boss Patterns**: Learn and memorize attack patterns
5. **Multiplayer**: Communicate with team for better coordination

### Advanced Strategies

1. **Grazing**: Stay close to bullets for bonus points (if enabled)
2. **Combo System**: Chain kills without taking damage
3. **Resource Management**: Save special attacks for bosses
4. **Position Optimization**: Learn safe spots on each stage
5. **Force Pod Tech**: Advanced positioning techniques

### Enemy Types (examples)

Below are common enemy prefabs used in levels (see `configs/entities/enemies/`):

- **Bat**
  - Health: 50
  - Damage (contact): 25
  - Behavior: vertical follow; shoots bursts (3 shots, spread ~10°) using `bat_basic_shot`.
  - Score: ~100
  - Strategy: avoid melee contact, strafe to dodge spread shots, prioritize when near player.

- **Canon**
  - Health: 50
  - Behavior: stationary or border-following turret; fires single powerful projectiles (fireRate 0.6, projectileSpeed 700).
  - Score: ~150
  - Strategy: use cover and timing to dodge attack animation; destroy quickly before sustained fire.

- **Snake Head** (`snake_head`)
  - Health: 200
  - Behavior: spawns and controls snake segments; scripted coordinated movement (`snake_behavior.lua`).
  - Score: ~500
  - Strategy: target the head to disable spawn control, avoid being trapped by segments.

- **Basic Enemy**
  - Generic prefab used for simple waves. Health, damage and behavior vary by prefab configuration.

### Bosses

Boss encounters are scripted and multi-phase. Two example bosses (configs in `configs/entities/boss/`):

- **Boss 1** (`boss1`) — Hilda-Berg style blimp
  - Health: 2000
  - Behavior: multi-mode fight (figure-eight movement, normal shooting mode, charge/ram mode). Periodically summons smaller enemies and uses different animation states for attacks. See `configs/scripts/boss1.lua` for phase/mode logic.
  - Strategy: learn pattern cycles — avoid charge windows, focus fire after telegraphed animations, exploit summon downtime.

- **Boss 2** (`boss2`) — Multi-part body + head
  - Health: composite (body + head parts). The body spawns a controllable head part that executes attack patterns (`boss2.lua` + `boss2head.lua`).
  - Strategy: prioritize head/parts as appropriate; destroying parts often reduces attack capability.

Notes:
- Exact values (health, damage, projectile stats) live in the prefab JSON files and their scripts: `configs/entities/enemies/*.json` and `configs/scripts/*.lua`.
- Use the Level Editor (`LevelEditor` UI) to inspect prefab fields (`PowerUps`, `Waves`) and preview enemy spawn behavior.

## FAQ

**Q: Can I play offline?**
A: No, R-Type requires a server connection. You can run a local server.

**Q: How many players can join?**
A: Up to 4 players per lobby.

**Q: Are saves shared between devices?**
A: No, saves are local to each installation.

**Q: Can I mod the game?**
A: Not officially supported, but asset files are accessible.

**Q: Is there controller vibration?**
A: Currently not implemented.

**Q: What platforms are supported?**
A: Windows 10+, Linux (Ubuntu 20.04+), macOS 11+

## Keyboard Shortcuts Reference

| Key | Action |
|-----|--------|
| `F1-F4` | Quick chat messages |
| `F3` | Toggle debug info |
| `F11` | Toggle fullscreen |
| `T` | Open text chat |
| `P` | Pause |
| `Escape` | Pause menu / Back |
| `Enter` | Confirm |
| `Tab` | Scoreboard |

## Credits & Support

### Development Team

See in-game credits for full team list.

### Support

- **Documentation**: https://albanrss.github.io/doc-rtype/
- **GitHub Issues**: Report bugs and request features
- **Community**: Join discussions on GitHub

### Version

Check current version: Main Menu → About

---

Enjoy playing R-Type! 🚀
