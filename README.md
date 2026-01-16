# Ryan R-Type

**Ryan R-Type** is a modern networked multiplayer space shooter game inspired by the classic R-Type arcade game. Built with a client-server architecture in C++20, it features real-time multiplayer gameplay, a custom Entity-Component-System (ECS), and advanced networking protocols.

## 🚀 Features

- 🎮 **Multiplayer Support**: Up to 4 players in cooperative gameplay
- 🌐 **Real-time Networking**: UDP-based communication with client-server architecture
- 🎨 **Modern Graphics**: SFML-powered rendering with dynamic effects
- 🔧 **Configurable Server**: Customizable port, IP, and tick rate
- 🕹️ **Custom ECS**: High-performance entity management system
- 🎯 **Lobby System**: Create and join game lobbies with unique codes
- 📊 **HTTP API**: RESTful API for game statistics and management
- 🔒 **Cross-platform**: Supports Windows, Linux, and macOS

## 📋 System Requirements

### Minimum Requirements
- **OS**: Windows 10+, Linux (Ubuntu 20.04+), macOS 11+
- **Compiler**: GCC 11+, Clang 12+, or MSVC 2019+
- **CMake**: Version 3.23 or higher
- **RAM**: 512 MB
- **Network**: Internet connection for multiplayer

### Development Requirements
- **C++ Compiler** with C++20 support
- **CMake**: 3.23+
- **vcpkg**: Package manager for dependencies
- **Git**: Version control

## ⚡ Quick Start

### Running Pre-built Binaries

```bash
# Start the server (default: 127.0.0.1:4242)
./r-type_server

# In another terminal, start the client
./r-type_client
```

### Multiplayer Setup

```bash
# Server on a specific IP and port
./r-type_server -p 8080 -i 0.0.0.0

# Clients connect to the server
./r-type_client -p 8080 -i <server_ip>
./r-type_client -p 8080 -i <server_ip>
```

## Installing Ryan R-Type

### Via Releases (Recommended)

Download the latest pre-built binaries from our [releases page](https://github.com/AlbanRSS/doc-rtype/releases).

**Installation steps:**

1. Download the appropriate archive for your platform:
   - **Windows**: `r-type-windows-x64.zip`
   - **Linux**: `r-type-linux-x64.tar.gz`
   - **macOS**: `r-type-macos-x64.tar.gz`

2. Extract the archive:
   ```bash
   # Linux/macOS
   tar -xzf r-type-*.tar.gz
   cd r-type
   
   # Windows: Use Windows Explorer or
   unzip r-type-windows-x64.zip
   cd r-type
   ```

3. Run the executables (see [Quick Start](#-quick-start) section)

### Via cloning

If you want to install our project by cloning it, you will need to follow these steps after cloning, and inside the repository:

#### For unix systems
```bash
./scripts/install_dependencies.sh
./scripts/compile_project.sh
```

#### For windows
```cmd
.\scripts\install_dependencies.bat
.\scripts\compile_project.bat
```

### Running the Program

After compiling the project, you can run the server and client executables.

#### Running the Server
```bash
./r-type_server -p <port> -i <ip_address>
```

Arguments:
- `-p <port>`: Specify the port number (default: 4242) (optional)
- `-i <ip_address>`: Specify the IP address to bind to (default: 127.0.0.1) (optional)
- `-tps <tps>`: Specify the TPS (ticks per second) (default: 20) (optional)
- `-d`: Enable debug mode (optional)
- `-h`: Display help message (optional)

Note: Port 5173 is reserved for HTTP server.

Example:
```bash
./r-type_server -p 4242 -i 127.0.0.1 -tps 20
```

#### Running the Client
```bash
./r-type_client [options]
```

Arguments:
- `-p <port>`: Specify the port of the server (default: 4242) (optional)
- `-i <ip_address>`: Specify the IP address of the server (default: 127.0.0.1) (optional)
- `-d`: Enable debug mode (optional)
- `-h`: Display help message (optional)

Example:
```bash
./r-type_client -p 4242 -i 127.0.0.1
```

## 🔧 Troubleshooting

### Common Issues

#### Server: Port Already in Use
```bash
# Error: Address already in use
# Solution: Use a different port
./r-type_server -p 8080 -i 127.0.0.1
```

#### Client: Connection Refused
```bash
# Possible causes:
# 1. Server is not running
# 2. Wrong IP or port
# 3. Firewall blocking connection

# Verify server is running on the correct port
./r-type_server -p 4242 -i 0.0.0.0

# Connect with matching port
./r-type_client -p 4242 -i <server_ip>
```

#### Compilation Errors
```bash
# If vcpkg dependencies fail to install
./scripts/install_dependencies.sh

# Clean build directory and rebuild
./scripts/clean_project.sh
./scripts/compile_project.sh
```

#### HTTP Server Port Conflict
Port 5173 is reserved for the HTTP API server. If you need to run the game server on a different port:
```bash
./r-type_server -p 4243  # Use any port except 5173
```

#### Permission Denied on Scripts
```bash
# Unix systems: Make scripts executable
chmod +x ./scripts/*.sh
```

### Performance Issues

**High Latency:**
- Ensure stable network connection
- Try reducing TPS: `./r-type_server -tps 15`
- Check firewall/router settings

**Low FPS:**
- Update graphics drivers
- Close resource-intensive applications
- Check system meets minimum requirements

### Debug Mode

Enable debug mode for detailed logging:
```bash
# Server
./r-type_server -d

# Client
./r-type_client -d
```

### Getting Help

If you encounter issues not covered here:
1. Check the [documentation](https://albanrss.github.io/doc-rtype/)
2. Search [existing issues](https://github.com/AlbanRSS/doc-rtype/issues)
3. Create a new issue with:
   - Your OS and version
   - Steps to reproduce
   - Error messages/logs
   - Expected vs actual behavior

## 🤝 Contributing

For contribution guidelines, including prerequisites and commit conventions, please refer to [HOWTOCONTRIBUTE.md](HOWTOCONTRIBUTE.md).

## Documentation

### Docusaurus

The **Docusaurus** documentation contains the entirety of the project documentation, including protocol specifications, server/client information, contribution guidelines, and usage instructions.

📖 **Access the online documentation**: https://albanrss.github.io/doc-rtype/

#### Running Documentation Locally

To run the documentation locally, navigate to the `/documentation` folder and run the following:

``` bash
cd documentation/docusaurus/
npm install
npm start
```
Then you can find the documentation at http://localhost:3000/


#### Prerequisites

To run the Docusaurus documentation locally:
- **Node.js**: Version 18.0 or higher
- **npm**: Version 8.0 or higher

The Docusaurus library will be automatically installed when running `npm install`.

For more information, see the [Docusaurus installation guide](https://docusaurus.io/docs/installation).


### Doxygen

The **Doxygen** module generates PDF documentation and diagrams of the project architecture and logic.

To generate the documentation, run:

```bash
./scripts/generate_doc.sh
```

The generated documentation will be available in `documentation/doc/`.

#### Prerequisites

To run the Doxygen generation script, you need:
- **Doxygen**: Version 1.9.0 or higher
- **LaTeX**: For PDF generation (pdflatex)
- **Graphviz**: For generating diagrams (optional but recommended)

Installation guides:
- [Doxygen Installation](https://www.doxygen.nl/manual/install.html)
- [LaTeX Installation](https://www.latex-project.org/get/)
- [Graphviz Installation](https://graphviz.org/download/)

**Make the script executable:**
```bash
chmod +x ./scripts/generate_doc.sh
```
