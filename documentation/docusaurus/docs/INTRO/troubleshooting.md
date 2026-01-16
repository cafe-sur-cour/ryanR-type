---
sidebar_position: 4
---

# Troubleshooting Guide

Common issues and their solutions when using Ryan R-Type.

## 🔌 Connection Issues

### Client Cannot Connect to Server

#### Symptom
```
Error: Connection refused
Failed to connect to 127.0.0.1:4242
```

#### Causes and Solutions

**1. Server Not Running**
```bash
# Start the server first
./r-type_server

# Verify it's running
ps aux | grep r-type_server  # Linux/macOS
tasklist | findstr r-type_server  # Windows
```

**2. Wrong IP or Port**
```bash
# Server
./r-type_server -p 4242 -i 0.0.0.0

# Client must match
./r-type_client -p 4242 -i <server_ip>
```

**3. Firewall Blocking Connection**
```bash
# Linux: Allow UDP port
sudo ufw allow 4242/udp

# Windows: Add firewall rule
netsh advfirewall firewall add rule name="R-Type" dir=in action=allow protocol=UDP localport=4242

# macOS: System Preferences → Security & Privacy → Firewall → Options
```

**4. Network Configuration**
```bash
# Test connectivity
ping <server_ip>

# Check if port is reachable
nc -vz -u <server_ip> 4242  # Linux/macOS
Test-NetConnection -ComputerName <server_ip> -Port 4242  # PowerShell
```

### High Latency / Lag

#### Symptom
- Delayed player movements
- Rubber-banding effects
- Packet loss warnings

#### Solutions

**1. Reduce Server TPS**
```bash
# Lower tick rate for slower networks
./r-type_server -tps 15
```

**2. Check Network Quality**
```bash
# Test latency
ping <server_ip>

# Check packet loss
ping -c 100 <server_ip>  # Linux/macOS
ping -n 100 <server_ip>  # Windows
```

**3. Close Bandwidth-Heavy Applications**
- Stop downloads/uploads
- Close streaming services
- Disable background updates

**4. Use Wired Connection**
- Ethernet cable instead of Wi-Fi
- Reduces latency and packet loss

### Timeout / Disconnection

#### Symptom
```
Player disconnected: timeout
Connection lost
```

#### Solutions

**1. Increase Timeout Value** (requires code modification)
```cpp
// In server configuration
constexpr int TIMEOUT_SECONDS = 10;  // Increase if needed
```

**2. Check Network Stability**
```bash
# Monitor connection
ping -t <server_ip>  # Windows
ping <server_ip>  # Linux/macOS (Ctrl+C to stop)
```

**3. Disable Power Saving**
- Prevent network adapter from sleeping
- Disable Wi-Fi power management

## 🖥️ Server Issues

### Port Already in Use

#### Symptom
```
Error: Address already in use
bind: Address already in use
```

#### Solutions

**1. Use Different Port**
```bash
./r-type_server -p 8080
```

**2. Find and Kill Process Using Port**
```bash
# Linux/macOS
lsof -ti:4242 | xargs kill -9

# Windows
netstat -ano | findstr :4242
taskkill /PID <PID> /F
```

**3. Wait for Port Release**
```bash
# Ports in TIME_WAIT state will be released automatically
# Wait 60-120 seconds or restart system
```

### HTTP Server Port Conflict (5173)

#### Symptom
```
Error: Port 5173 is reserved for HTTP server
Cannot bind to port 5173
```

#### Solution
```bash
# Use any port except 5173
./r-type_server -p 4242
./r-type_server -p 8080
# But NOT -p 5173
```

### Server Crashes on Startup

#### Symptom
```
Segmentation fault
Server terminated unexpectedly
```

#### Solutions

**1. Enable Debug Mode**
```bash
./r-type_server -d
# Check logs for detailed error information
```

**2. Check Configuration Files**
```bash
# Verify JSON files are valid
ls configs/
# Ensure all required config files exist
```

**3. Verify Assets**
```bash
# Run from project root directory
cd /path/to/R-Type
./r-type_server
```

**4. Check Permissions**
```bash
# Ensure executable has permissions
chmod +x r-type_server

# Check directory permissions
ls -la configs/ saves/
```

## 🎮 Client Issues

### Graphics Not Displaying

#### Symptom
- Black screen
- Missing textures
- No sprites visible

#### Solutions

**1. Update Graphics Drivers**
```bash
# Linux: Update system
sudo apt update && sudo apt upgrade

# macOS: Update via App Store
# Windows: Update via Device Manager or vendor website
```

**2. Check OpenGL Support**
```bash
# Linux
glxinfo | grep "OpenGL version"

# Should be 3.3 or higher
```

**3. Run from Correct Directory**
```bash
# Assets are loaded relative to executable
cd /path/to/R-Type
./r-type_client
```

### Low FPS / Performance Issues

#### Symptom
- Game runs slowly
- Stuttering or freezing
- FPS below 30

#### Solutions

**1. Close Background Applications**
```bash
# Check system resources
top  # Linux/macOS
Task Manager  # Windows
```

**2. Reduce Graphics Settings** (if configurable)
- Lower resolution
- Disable post-processing effects
- Reduce particle effects

**3. Check System Requirements**
- Verify CPU/RAM meets minimum requirements
- Ensure no thermal throttling

**4. Update System**
```bash
# Linux
sudo apt update && sudo apt upgrade

# macOS
softwareupdate --list
softwareupdate --install --all
```

### Audio Issues

#### Symptom
- No sound
- Crackling audio
- Audio desync

#### Solutions

**1. Check Volume Settings**
```bash
# Verify system audio is not muted
# Check in-game audio settings (if available)
```

**2. Update Audio Drivers**
```bash
# Linux: Check PulseAudio/ALSA
pulseaudio --check
pulseaudio --start

# Install audio libraries if missing
sudo apt install libopenal1
```

**3. Check Audio Device**
```bash
# Linux: List audio devices
aplay -l

# Switch default device if needed
```

### Input Not Working

#### Symptom
- Keyboard/gamepad not responding
- Controls don't match expectations

#### Solutions

**1. Check Input Device**
```bash
# Linux: Test keyboard
xev  # X11
evtest  # Wayland

# Test gamepad
jstest /dev/input/js0
```

**2. Verify Keybindings**
```bash
# Check saved keybindings
cat saves/keybinds.json
```

**3. Reset to Default**
```bash
# Remove custom keybinds
rm saves/keybinds.json
# Game will recreate with defaults
```

## 🔧 Compilation Issues

### Build Fails with CMake Errors

#### Symptom
```
CMake Error: Could not find a package configuration file
```

#### Solutions

**1. Install Dependencies**
```bash
./scripts/install_dependencies.sh
```

**2. Clean Build Directory**
```bash
./scripts/clean_project.sh
./scripts/compile_project.sh
```

**3. Update vcpkg**
```bash
cd vcpkg
git pull
./bootstrap-vcpkg.sh  # Linux/macOS
bootstrap-vcpkg.bat  # Windows
```

### Compiler Errors

#### Symptom
```
error: 'concept' does not name a type
C++20 features not available
```

#### Solutions

**1. Check Compiler Version**
```bash
# Requires GCC 11+, Clang 12+, or MSVC 2019+
g++ --version
clang++ --version
```

**2. Update Compiler**
```bash
# Ubuntu
sudo apt install gcc-11 g++-11

# Set as default
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100
```

**3. Specify Compiler**
```bash
export CC=/usr/bin/gcc-11
export CXX=/usr/bin/g++-11
./scripts/compile_project.sh
```

### vcpkg Dependency Installation Fails

#### Symptom
```
Error: Failed to download package
vcpkg install failed
```

#### Solutions

**1. Check Internet Connection**
```bash
ping github.com
```

**2. Clear vcpkg Cache**
```bash
rm -rf ~/.vcpkg
./scripts/install_dependencies.sh
```

**3. Manual vcpkg Bootstrap**
```bash
cd vcpkg
./bootstrap-vcpkg.sh --disableMetrics
./vcpkg integrate install
```

## 📁 File Permission Issues

### Permission Denied on Scripts

#### Symptom
```
bash: ./scripts/compile_project.sh: Permission denied
```

#### Solution
```bash
# Make all scripts executable
chmod +x ./scripts/*.sh

# Or individually
chmod +x ./scripts/install_dependencies.sh
chmod +x ./scripts/compile_project.sh
```

### Cannot Write to Save Files

#### Symptom
```
Error: Cannot write to saves/settings.json
Permission denied
```

#### Solutions

**1. Check Directory Permissions**
```bash
ls -la saves/
chmod 755 saves/
```

**2. Run with Proper User**
```bash
# Don't run as root unless necessary
./r-type_client
```

**3. Verify Disk Space**
```bash
df -h  # Check available space
```

## 🐛 Debug Mode

Enable detailed logging to diagnose issues:

```bash
# Server with debug output
./r-type_server -d

# Client with debug output
./r-type_client -d
```

Debug logs include:
- Network packet details
- Entity creation/destruction
- System execution times
- Error stack traces

## 📞 Getting Additional Help

If your issue isn't covered here:

1. **Check Documentation**
   - [Online docs](https://albanrss.github.io/doc-rtype/)
   - [Installation guide](./installation.md)
   - [Server documentation](../SERVER/server-intro.md)

2. **Search Existing Issues**
   - [GitHub Issues](https://github.com/AlbanRSS/doc-rtype/issues)
   - Use search to find similar problems

3. **Create New Issue**
   Include:
   - Operating system and version
   - Compiler version (for build issues)
   - Complete error message
   - Steps to reproduce
   - Debug logs (if applicable)

4. **Ask the Community**
   - Project discussions
   - Stack Overflow (tag: r-type)

---

**Pro Tip:** Always run with `-d` flag when reporting bugs to get detailed logs!
